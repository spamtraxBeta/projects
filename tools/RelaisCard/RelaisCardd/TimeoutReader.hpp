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

#ifndef TimeoutReader_hpp
#define TimeoutReader_hpp

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <iostream>


template <class port_t>
class TimeoutReader
{
public:
    //typedef boost::asio::serial_port port_t;

    TimeoutReader(port_t & port, boost::int64_t timeoutMs) :
        m_ioService(port.get_io_service())
        , m_timer(m_ioService, boost::posix_time::milliseconds(timeoutMs))
        , m_port(port)
        , m_dataReceived(false)
    {

    }
    ~TimeoutReader()
    {

    }

    bool read(boost::asio::mutable_buffers_1 buffer)
    {
        m_dataReceived = false;
        //std::cerr << "enter read()" << std::endl;
     
        m_ioService.reset();

        m_timer.async_wait(boost::bind(&TimeoutReader::onTimeout, this));
        
        boost::asio::async_read
        (
            m_port,
            buffer,
            boost::bind
            (
                &TimeoutReader::handle_read,
                this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
            )
        );

       
        m_ioService.run();

        //std::cerr << "exit read() = " << m_dataReceived << std::endl;
        return m_dataReceived;
    }

private:

    void onTimeout()
    {
        //std::cerr << "Timeout"<<std::endl;
        //m_dataReceived = false;

        if (m_dataReceived == false)
        {
            m_port.cancel();
        }
        
    }

    void handle_read(const boost::system::error_code & ec, std::size_t numberBytes)
    {
        //std::cerr << "handle_read; ec=" << ec <<"; numberBytes = "<<numberBytes<<std::endl;
        if (ec)
        {
            // handle error
        }
        else
        {
            // do something
            m_timer.cancel();
            m_dataReceived = true;
        }
    }

    boost::asio::io_service & m_ioService;
    boost::asio::deadline_timer m_timer;

    port_t & m_port;

    bool m_dataReceived;
};


#endif // TimeoutReader_hpp
