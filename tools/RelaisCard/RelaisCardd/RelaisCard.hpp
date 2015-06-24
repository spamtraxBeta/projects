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

#ifndef RelaisCard_hpp
#define RelaisCard_hpp


#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/integer.hpp>

#include "../Common/RelaisFrame.hpp"



class RelaisCard
{
    friend class RelaisCardInitializer;


    void noop(RelaisFrame & response);

    boost::uint8_t getPort(RelaisFrame & response);

    boost::uint8_t getOption(RelaisFrame & response);
    void setOption(boost::uint8_t value, RelaisFrame & response);

    void setSingle(boost::uint8_t value, RelaisFrame & response);
    void delSingle(boost::uint8_t value, RelaisFrame & response);
    void toggle(boost::uint8_t value, RelaisFrame & response);

    void setPort(boost::uint8_t value, RelaisFrame & response);
    
    void refresh();


public:
    RelaisCard(boost::asio::serial_port & serialPort, boost::uint8_t cardAddress);
    ~RelaisCard();

    boost::uint8_t cardAddress()const
    {return m_cardAddress;}

    RelaisFrame exec(RelaisFrame request);



private:
    boost::asio::serial_port & m_serialPort;
    uint8_t m_cardAddress;
    uint8_t m_currentState;
    uint8_t m_portValue;
    uint8_t m_optionValue;

    void exec(RelaisFrame & request, RelaisFrame & response);



};

#endif // RelaisCard_hpp
