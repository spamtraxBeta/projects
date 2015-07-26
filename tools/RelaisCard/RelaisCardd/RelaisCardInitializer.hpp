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

#ifndef RelaisCardInitializer_hpp
#define RelaisCardInitializer_hpp

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <vector>

#include "CommandLineD.hpp"
#include "RelaisCard.hpp"


class RelaisCardInitializer
{
public:
    RelaisCardInitializer(boost::asio::io_service & ioService, const CommandLineD & cmdLine, uint8_t cardAddress = 0);
    ~RelaisCardInitializer();

    boost::uint8_t firstCardAddress()const
    {return m_firstCardAddress;}
    
    boost::uint8_t lastCardAddress()const
    {return m_lastCardAddress;}

    int numberCards()const
    {return m_cards.size();}
    RelaisCard & at(uint8_t index);
    RelaisCard & operator[](boost::uint8_t index);

    void reinit();


private:
    const CommandLineD & m_cmdLine;
    boost::asio::io_service & m_ioService;
    boost::shared_ptr<boost::asio::serial_port> m_serialPort;
    uint8_t m_firstCardAddress;
    uint8_t m_lastCardAddress;

    std::vector<boost::shared_ptr<RelaisCard> > m_cards;

    void openPort();
    void setup(boost::asio::serial_port & serialPort, const uint8_t & firstAddress, uint8_t & lastAddress) const;
};

#endif // RelaisCardInitializer_hpp

