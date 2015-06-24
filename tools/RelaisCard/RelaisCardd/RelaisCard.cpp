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

#include "RelaisCard.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <MaEr/log/logHelper.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::cin;

#include "../Common/RelaisError.hpp"
#include "TimeoutReader.hpp"

RelaisCard::RelaisCard(boost::asio::serial_port & serialPort, boost::uint8_t cardAddress) :
    m_serialPort(serialPort)
    , m_cardAddress(cardAddress)
{
    logDeclaration();
}


RelaisCard::~RelaisCard()
{
}

void RelaisCard::noop(RelaisFrame & response)
{
    logDeclaration();

    RelaisFrame request(RelaisFrame::cmd_noop, m_cardAddress, 0);
    exec(request, response);
}

uint8_t RelaisCard::getPort(RelaisFrame & response)
{
    logDeclaration();

    RelaisFrame request(RelaisFrame::cmd_getPort, m_cardAddress, 0);
    exec(request, response);

    m_portValue = response.data();
    return m_portValue;
}

void RelaisCard::setPort(boost::uint8_t value, RelaisFrame & response)
{
    logDeclaration();

    RelaisFrame request(RelaisFrame::cmd_setPort, m_cardAddress, value);
    
    // set value
    exec(request, response);

    // read value from port
    RelaisFrame tempResponse;
    uint8_t currentState = this->getPort(tempResponse);

    // check if new state was set successfully
    if (currentState != value)
    {
        throw RelaisError("Failed to set port value", request, response);
    }
}

void RelaisCard::refresh()
{
    logDeclaration();

    RelaisFrame response;
    this->setPort(m_portValue, response);
}

boost::uint8_t RelaisCard::getOption(RelaisFrame & response)
{
    logDeclaration();

    RelaisFrame request(RelaisFrame::cmd_getOption, m_cardAddress, 0);
    exec(request, response);

    m_optionValue = response.data();
    return m_optionValue;
}

void RelaisCard::setOption(boost::uint8_t value, RelaisFrame & response)
{
    logDeclaration();

    RelaisFrame request(RelaisFrame::cmd_setOption, m_cardAddress, value);

    // set value
    exec(request, response);

    // read value from port
    RelaisFrame tempResponse;
    uint8_t currentOption = this->getPort(tempResponse);

    // check if new state was set successfully
    if (currentOption != value)
    {
        throw RelaisError("Failed to set option value", request, response);
    }
}

void RelaisCard::setSingle(boost::uint8_t value, RelaisFrame & response)
{
    logDeclaration();

    RelaisFrame request(RelaisFrame::cmd_setSingle, m_cardAddress, value);

    // set value
    exec(request, response);

    uint8_t expectedPortState = m_portValue | value;

    // read value from port
    RelaisFrame tempResponse;
    uint8_t currentState = this->getPort(tempResponse);


    // check if new state was set successfully
    if (currentState != expectedPortState)
    {
        throw RelaisError("Failed to set single port value", request, response);
    }
}

void RelaisCard::delSingle(boost::uint8_t value, RelaisFrame & response)
{
    logDeclaration();

    RelaisFrame request(RelaisFrame::cmd_delSingle, m_cardAddress, value);

    // set value
    exec(request, response);

    uint8_t expectedPortState = m_portValue & static_cast<uint8_t>(~value);

    // read value from port
    RelaisFrame tempResponse;
    uint8_t currentState = this->getPort(tempResponse);
    
    // check if new state was set successfully
    if (currentState != expectedPortState)
    {
        throw RelaisError("Failed to del single port value", request, response);
    }
}

void RelaisCard::toggle(boost::uint8_t value, RelaisFrame & response)
{
    logDeclaration();

    RelaisFrame request(RelaisFrame::cmd_toggle, m_cardAddress, value);

    // set value
    exec(request, response);

    uint8_t expectedPortState = m_portValue ^ value;

    // read value from port
    RelaisFrame tempResponse;
    uint8_t currentState = this->getPort(tempResponse);

    // check if new state was set successfully
    if (currentState != expectedPortState)
    {
        throw RelaisError("Failed to del single port value", request, response);
    }
}

RelaisFrame RelaisCard::exec(RelaisFrame request)
{
    logDeclaration();

    RelaisFrame response = request;

    RelaisFrame::command_t command = request.cmd();

    switch (command)
    {
        case RelaisFrame::cmd_noop:
        {
            this->noop(response);
        }break;

        case RelaisFrame::cmd_setup:
        {
            // ignore
            logInfo() << "Ignored 'setup' command: "<<request;
        }break;

        case RelaisFrame::cmd_getPort:
        {
            uint8_t state = this->getPort(response);
        }break;

        case RelaisFrame::cmd_setPort:
        {
            this->setPort(request.data(), response);
        }break;

        case RelaisFrame::cmd_getOption:
        {
            uint8_t option = this->getOption(response);
        }break;

        case RelaisFrame::cmd_setOption:
        {
            this->setOption(request.data(), response);
        }break;

        case RelaisFrame::cmd_setSingle:
        {
            this->setSingle(request.data(), response);
        }break;

        case RelaisFrame::cmd_delSingle:
        {
            this->delSingle(request.data(), response);
        }break;

        case RelaisFrame::cmd_toggle:
        {
            this->toggle(request.data(), response);
        }break;

        default:
        {}
    }

    return response;

}

void RelaisCard::exec(RelaisFrame & request, RelaisFrame & response)
{
    logDeclaration();

    logDebug() << "Writing "<<request;
    boost::asio::write(m_serialPort, request.buffer());

    int waitTimeMs = 1000;
    TimeoutReader<boost::asio::serial_port> tr(m_serialPort, waitTimeMs);

    // read response
    boost::asio::mutable_buffers_1 bb = response.buffer();
    bool result = tr.read(bb);
    //bool result = tr.read(response.buffer());
    if (result == true){logDebug()<<"Received: "<<response;}
    else{logTrace()<<"Timeout, nothing received within "<< waitTimeMs << " ms";}
    
    logTrace() << "Waiting for echo";
    RelaisFrame requestEcho; 
    if (tr.read(requestEcho.buffer()) == true)
    {logDebug() << "Received echo: " << response;}
    

    if (result == false)
    {
        throw RelaisError("No response to request", request, response);
    }

    if (response.checksumOk() == false)
    {
        throw RelaisError("Invalid checksum received", request, response);
    }       
        
    else if (static_cast<boost::uint8_t>(response.cmd()) != static_cast<boost::uint8_t>(~request.cmd()))
    {// response from card, just version info
        throw RelaisError("Invalid response received, received cmd is not bitwise inverted request.", request, response);
    }

    else
    {
        // ok 
        logTrace() << "Response " << response << " to request " << request << " is ok";
    }
  
}

