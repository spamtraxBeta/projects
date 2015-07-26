/******************************************************************************
* This file is part of RelaisCard.                                            *
*                                                                             *
* RelaisCard is free software : you can redistribute it and / or modify       *
* it under the terms of the GNU General Public License as published by        *
* the Free Software Foundation, either version 3 of the License, or           *
* (at your option) any later version.                                         *
*                                                                             *
* RelaisCard is distributed in the hope that it will be useful,               *
* but WITHOUT ANY WARRANTY; without even the implied warranty of              *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the                 *
* GNU General Public License for more details.                                *
*                                                                             *
* You should have received a copy of the GNU General Public License           *
* along with RelaisCard.If not, see <http://www.gnu.org/licenses/>.           *
*******************************************************************************/

// comport.cpp : Definiert den Einstiegspunkt f r die Konsolenanwendung.
//

#ifdef WIN32

//http://lists.boost.org/boost-users/2008/07/37824.php
#define WIN32_LEAN_AND_MEAN 

#include <windows.h>
#endif


#include <string>
#include <iostream>
#include <sstream>

#include <boost/asio.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/thread/thread_time.hpp>

#include <MaEr/log/logHelper.hpp>

#include "CommandLineD.hpp"
#include "RelaisCardInitializer.hpp"

#include "../Common/InterprocessComm.hpp"
#include "../Common/RelaisError.hpp"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;

namespace binter = boost::interprocess;


/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char * argv[])
{
    MaEr::LogHelper::init();

    logDeclaration();
    
	try
	{
        // Read command line
        logTrace() << "Creating command line parser";
        CommandLineD::create(argc, argv);
        CommandLineD & cmdLine = CommandLineD::instance();

		if (cmdLine.help())
		{
            logTrace() << "Printing help text, then exit";
			cout << cmdLine.helpText() << endl;
			return 0;
		}

        if (cmdLine.showVersionInfo())
        {
            cout << "Version of interprocess communication: " << INTERPROCESS_VERSION;
            return 0;
        }

        if (cmdLine.logFile().empty() == false)
        {
            logTrace() << "Adding log file: " << cmdLine.logFile();
            MaEr::LogHelper::addFileLogger(argv[0], cmdLine.logFile());
        }

#ifdef WIN32
        if (cmdLine.hideConsole() == true)
        {
            logTrace() << "Hiding console window";
            FreeConsole();
        }
#endif
        
        // Setup message queue
        struct msgQueueRemover
        {
            msgQueueRemover(){ logDeclaration(); logTrace() << "Removing message queue: " << MESSAGE_QUEUE_NAME; binter::message_queue::remove(MESSAGE_QUEUE_NAME); }
            ~msgQueueRemover(){ logDeclaration(); logTrace() << "Removing message queue: " << MESSAGE_QUEUE_NAME; binter::message_queue::remove(MESSAGE_QUEUE_NAME); }
        }queueRemover;

        logTrace() << "Creating message queue with name '" << MESSAGE_QUEUE_NAME;
        binter::message_queue evtLoop
        (
            boost::interprocess::open_or_create // open_or_create
            , MESSAGE_QUEUE_NAME                // name
            , MESSAGE_QUEUE_MAX_NUM_MESSAGES    // max message number
            , MESSAGE_QUEUE_MAX_MESSAGE_SIZE    // max message size
        );
       
        logTrace() << "Creating ioService";
        boost::asio::io_service ioService;

        boost::shared_ptr<RelaisCardInitializer> rci = boost::make_shared<RelaisCardInitializer>(ioService, cmdLine);

        while (true)
        {
            unsigned int priority;
            binter::message_queue::size_type recvd_size;
            unsigned char receiveBuffer[MESSAGE_QUEUE_MAX_MESSAGE_SIZE];
            int waitingTimeMs = 1000;
            boost::posix_time::ptime receiveTimeout = boost::get_system_time() + boost::posix_time::millisec(waitingTimeMs);

            logTrace() << "Waiting for event";
            bool received = evtLoop.timed_receive(receiveBuffer, MESSAGE_QUEUE_MAX_MESSAGE_SIZE, recvd_size, priority, receiveTimeout);

            if (received == false)
            {// timeout => reinit
                logTrace() << "No event within '" << waitingTimeMs << "' ms";

                try
                {
                    rci->reinit();
                }
                catch (std::exception & ex)
                {
                    logError() << "Error detected while watchdog reinit: "<<ex.what()<<"; Trying to reinit";

                    try
                    {
                        rci = boost::make_shared<RelaisCardInitializer>(ioService, cmdLine);
                    }
                    catch (boost::system::system_error & systemError)
                    {
                        logError() << "Failed to reinit. What: " << systemError.what() << "; code: " << systemError.code();
                    }
                }
            }
            else
            {
                logTrace() << "Event received";

                const char * shmName = reinterpret_cast<const char *>(receiveBuffer);

                logTrace() << "Opening shared memory segment with name: " << shmName;
                binter::managed_shared_memory segment(binter::open_only, shmName);

                std::pair<InterprocessComm *, binter::managed_shared_memory::size_type> res;

                logTrace() << "Searching object with name: " << INTERPROCESS_OBJECT_NAME;
                res = segment.find<InterprocessComm>(INTERPROCESS_OBJECT_NAME);

                if (res.second == 1)
                {
                    InterprocessComm & ic = *(res.first);
                    logTrace() << "Found object with name '" << INTERPROCESS_OBJECT_NAME << "': " << ic;

                    RelaisFrame requestFrame = ic.data();
                    RelaisFrame responseFrame = requestFrame;

                    uint8_t address = requestFrame.address();

                    ic.setFirstAddress(rci->firstCardAddress());
                    ic.setLastAddress(rci->lastCardAddress());

                    if (ic.version() != INTERPROCESS_VERSION)
                    {
                        logError() << "Received frame has version number " << ic.version() << " but version " << INTERPROCESS_VERSION << " is expected";
                        ic.setExecResult(InterprocessComm::Error);
                        ic.setMsg("Incompatible version number of interprocess object detected");
                    }
                    else if (address == 0)
                    {
                        logInfo() << "Broadcast not supported by this application";
                        ic.setExecResult(InterprocessComm::OperationNotSupported);
                    }
                    else
                    {
                        bool errorDetected = false;

                        try
                        {
                            // 0: broadcast; 1: first, ...
                            // but in vector first card has index 0, so "- 1"

                            if (address > rci->numberCards())
                            {
                                logError() << "No card available with address '" << static_cast<int>(address) << "'";
                            }
                            else
                            {
                                RelaisCard & card = (*rci)[address - 1];

                                responseFrame = card.exec(requestFrame);
                                ic.setExecResult(InterprocessComm::Success);
                                ic.setData(responseFrame);
                            }
                        }
                        catch (RelaisError & ex)
                        {
                            logError() << ex.what();

                            ic.setExecResult(InterprocessComm::Error);
                            ic.setData(ex.response());
                            ic.setMsg(ex.what());

                            errorDetected = true;
                        }
                        catch (std::exception & ex)
                        {
                            logError() << ex.what();

                            ic.setExecResult(InterprocessComm::Error);
                            ic.setMsg(ex.what());

                            errorDetected = true;
                        }

                        if (errorDetected == true)
                        {
                            logError() << "Error detected, trying to reinit cards";

                            try
                            {
                                rci = boost::make_shared<RelaisCardInitializer>(ioService, cmdLine);
                            }
                            catch (boost::system::system_error & systemError)
                            {
                                logError() << "Failed to reinit. What: " << systemError.what() <<"; code: "<< systemError.code();
                            }
                            
                        }
                    }


                    logTrace() << "Unlocking mutex in shared object. Data: "<< ic;
                    res.first->mutex().unlock();
                }
                else
                {
                    logError() << "Shared memory '" << shmName << "' contains " << res.second <<" items with name '" << INTERPROCESS_OBJECT_NAME<< "'. Note: 1 element is expected";
                }
            }
        }

	}
    catch (RelaisError & error)
    {
        logFatal() << error.what() << "; request: " << error.request() << "; response: " << error.response();
        return 1;
    }
	catch (std::exception & error)
	{
        logFatal() << error.what();
		return 1;
    }
    catch (...)
    {
        logFatal() << "Unknown exception detected";
    }
    
	return 0;
}

