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

#include "RelaisCardInitializer.hpp"

#include <boost/make_shared.hpp>
#include <MaEr/log/logHelper.hpp>
#include <MaEr/boostHelpers/asioTimeoutReader.hpp>

#include "../Common/RelaisFrame.hpp"
#include "../Common/RelaisError.hpp"

RelaisCardInitializer::RelaisCardInitializer(boost::asio::io_service & ioService, const CommandLineD & cmdLine, uint8_t cardAddress) :
    m_cmdLine(cmdLine)
    , m_ioService(ioService)
    , m_firstCardAddress(cardAddress)
    , m_lastCardAddress(cardAddress)
{
    logDeclaration();

    this->openPort();
    this->setup(*m_serialPort, m_firstCardAddress, m_lastCardAddress);

    for (uint8_t i = m_firstCardAddress; i <= m_lastCardAddress; ++i)
    {
        

        boost::shared_ptr<RelaisCard> rc = boost::make_shared<RelaisCard>(*m_serialPort, i);
        m_cards.push_back(rc);
        RelaisFrame response;
        uint8_t initialPortValue = m_cmdLine.initialPortState();

        logTrace() << "Setting port of  card '" << i << "' to " << initialPortValue;
        rc->setPort(initialPortValue, response);
    }
}


RelaisCardInitializer::~RelaisCardInitializer()
{

}

void RelaisCardInitializer::openPort()
{
    logDeclaration();

    logTrace() << "Creating serial port object";
    m_serialPort = boost::make_shared<boost::asio::serial_port>(m_ioService);

    logInfo() << "Opening serial port '" << m_cmdLine.port() << "'";
    m_serialPort->open(m_cmdLine.port());

    logTrace() << "Setting baud rate = " << m_cmdLine.baudRate().value();
    m_serialPort->set_option(m_cmdLine.baudRate());

    logTrace() << "Setting parity = " << m_cmdLine.parity().value();
    m_serialPort->set_option(m_cmdLine.parity());

    logTrace() << "Setting character size = " << m_cmdLine.characterSize().value();
    m_serialPort->set_option(m_cmdLine.characterSize());

    logTrace() << "Setting stop bits = " << m_cmdLine.stopBits().value();
    m_serialPort->set_option(m_cmdLine.stopBits());

    logTrace() << "Setting flow control = " << m_cmdLine.flowControl().value();
    m_serialPort->set_option(m_cmdLine.flowControl());
}

void RelaisCardInitializer::setup(boost::asio::serial_port & serialPort, const uint8_t & firstAddress, uint8_t & lastAddress)const
{
    logDeclaration();

    RelaisFrame request(RelaisFrame::cmd_setup, firstAddress, 0);
    RelaisFrame response;

    // send init request
    logDebug() << "Sending " << request;
    boost::asio::write(serialPort, request.buffer());


    int msToWaitForResponse = 1000;
    MaEr::BoostHelper::asioTimeoutReader<boost::asio::serial_port> tr(serialPort, msToWaitForResponse);

    int nextFreeAddress = firstAddress;

    logTrace() << "Waiting for response";
    while (tr.read(response.buffer()))
    {
        logDebug() << "Response received: " << response;
        if (response.checksumOk() == false)
        {
            throw RelaisError("Invalid checksum received", request, response);
        }

        if (response.cmd() == static_cast<boost::uint8_t>(~request.cmd()))
        {// init response from first card, just version info
            logTrace() << "Ignoring response from which contains version info";
        }
        else if (response.cmd() == RelaisFrame::cmd_setup)
        {// response from last card
            nextFreeAddress = response.address();

            lastAddress = nextFreeAddress - 1;
            break;
        }
        else
        {
            logError() << "Response command is invalid. Expected response command: either complement of request or 'setup': "<<response;
            throw RelaisError("Invalid response received", request, response);
        }
    }


    if (nextFreeAddress == m_firstCardAddress)
    {
        logError() << "No response within " << msToWaitForResponse << "ms";
        throw std::runtime_error("No card detected.");
    }

}

RelaisCard & RelaisCardInitializer::at(boost::uint8_t index)
{
    boost::shared_ptr<RelaisCard> rc = m_cards[index];
    return *rc;
}

RelaisCard & RelaisCardInitializer::operator[](boost::uint8_t index)
{
    boost::shared_ptr<RelaisCard> rc = m_cards[index];
    return *rc;
}

void RelaisCardInitializer::reinit()
{
    logDeclaration();

    uint8_t firstCardAddress = m_firstCardAddress;
    uint8_t lastCardAddress = m_lastCardAddress;

    this->setup(*m_serialPort, firstCardAddress, lastCardAddress);

    if
    (
        (m_firstCardAddress != firstCardAddress)
        ||
        (m_lastCardAddress != lastCardAddress)
    )
    {
        logError() << "Card addresses have changed. First card address from " << m_firstCardAddress << " to " << firstCardAddress << "; last card address from " << m_lastCardAddress << " to " << lastCardAddress;
        throw std::runtime_error("Number of cards has changed which is not supported by this program");
    }
    else
    {// ok

        for (std::vector<boost::shared_ptr<RelaisCard> >::iterator iter = m_cards.begin(); iter != m_cards.end(); ++iter)
        {
            RelaisFrame response;

            boost::shared_ptr<RelaisCard> rc = *iter;

            rc->refresh();
        }
    }
}

