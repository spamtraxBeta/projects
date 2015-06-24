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

// RelaisCard.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/interprocess/smart_ptr/shared_ptr.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/smart_ptr/deleter.hpp>

#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>


#include "../Common/InterprocessComm.hpp"
#include <MaEr/log/logHelper.hpp>
#include "CommandLine.hpp"

#include <iostream>
using namespace std;
namespace binter = boost::interprocess;




std::string getRandomString(int length = 8)
{
    std::string chars(
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890"
        //"!@#$%^&*()"
        //"`~-_=+[{]{\\|;:'\",<.>/? "
        );
    boost::random::random_device rng;
    boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);

    std::stringstream ss;


    for (int i = 0; i < length; ++i)
    {
        ss << chars[index_dist(rng)];
    }

    std::string randomString;
    std::getline(ss, randomString);

    return randomString;

}


int main(int argc, const char * argv[])
{
    MaEr::LogHelper::init();

    logDeclaration();

    int returnValue = 1;

    std::string shmName = getRandomString(8);

    try
    {
    #if 0
        _TCHAR * argv[] = 
        {
            L"exe",
            L"-c",
            L"setPort",
            L"-v",
            L"0xFF"
        };

        argc = sizeof(argv) / sizeof(_TCHAR *);
    #endif // #if 0

        CommandLine cmdLine(argc, argv);

        if (cmdLine.showVersionInfo())
        {
            cout << "Version of interprocess communication: " << INTERPROCESS_VERSION;
            return 0;
        }

        if (cmdLine.help())
        {
            cerr << cmdLine.helpText()<<endl;
            return -1;
        }

        if (cmdLine.logFile().empty() == false)
        {
            logTrace() << "Adding log file: " << cmdLine.logFile();
            MaEr::LogHelper::addFileLogger(argv[0], cmdLine.logFile());
        }


        //Remove shared memory on construction and destruction
        struct shm_remove
        {
            shm_remove(const std::string & name) : m_name(name) { logDeclaration(); logTrace() << "Removing shared memory with name '"<<name<<"'"; binter::shared_memory_object::remove(name.c_str()); }
            ~shm_remove(){ logDeclaration(); logTrace() << "Removing shared memory with name '" << m_name << "'"; binter::shared_memory_object::remove(m_name.c_str()); }

            private:
            std::string m_name;
        } remover(shmName);


        // create shared memory
        int shmSize = 1024;
        logTrace() << "Creating shared memory segment with name '" << shmName << "'; size: " << shmSize;
        binter::managed_shared_memory segment(binter::create_only, shmName.c_str(), shmSize);

        // create object
        RelaisFrame request(cmdLine.command(), cmdLine.address(), cmdLine.value());

        logTrace() << "Creating interprocess object in shared memory with name '" << INTERPROCESS_OBJECT_NAME << "'";
        InterprocessComm * ic = segment.construct<InterprocessComm>(INTERPROCESS_OBJECT_NAME)(request);

        logTrace() << "Opening message queue with name '" << MESSAGE_QUEUE_NAME << "'";
        binter::message_queue mq
        (
            binter::open_only               //only open
            , MESSAGE_QUEUE_NAME     //name
        );
    
        logTrace() << "Sending name of shared memory to remote process: '" << shmName << "'";
        mq.send(shmName.c_str(), shmName.length() + 1, 0);

        logTrace() << "Locking mutex in shared object";
        binter::scoped_lock<binter::interprocess_mutex> lock(ic->mutex());

        logTrace() << "Locking mutex in shared object again and waiting until remote process unlocks it";
        int waitTimeMs = 5000;
        boost::posix_time::ptime abs_time = boost::get_system_time() + boost::posix_time::millisec(waitTimeMs);
        if (ic->mutex().timed_lock(abs_time) == false)
        {
            logError() << "Mutex was not unlocked within " << waitTimeMs << "ms. Is RelaisCardd running?";
        }
        else
        {// unlocked => data updated by daemon
            logTrace() << "Mutex unlocked by remote process. Result: "<<*ic;

            if (ic->execResult() != InterprocessComm::Success)
            {
                logError() << ic->getMsg();
            }
            else
            {
                //if (ic->data().commandHasData() == true)
                {
                    uint8_t response = ic->data().data();
                    cout <<"data: 0x"<<hex<< static_cast<unsigned int>(response)<<endl;
                }

                // no error
                returnValue = 0;
            }
        }

        logTrace() << "Destroying shared object with name '" << INTERPROCESS_OBJECT_NAME << "'";
        segment.destroy<InterprocessComm>(INTERPROCESS_OBJECT_NAME);

    }
    catch (binter::interprocess_exception & ex)
    {
        logFatal() << ex.what();
        binter::message_queue::remove(MESSAGE_QUEUE_NAME);
        return -1;
    }
    catch (std::exception & ex)
    {
        logFatal() << ex.what();
        return -1;
    }

    return returnValue;
}

    
