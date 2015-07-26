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

#ifndef asioTimeoutReader_hpp
#define asioTimeoutReader_hpp

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <MaEr/unusedvariable.hpp>

#include <iostream>

namespace MaEr
{
namespace BoostHelper
{
/**
 * No timeout can be set when using boost:asio for reading
 * from a port.
 * http://stackoverflow.com/questions/13126776/asioread-with-timeout
 * shows a solution:
 *  1) start a timer before reading,
 *  2) cancel reading if timer expires before data is received
 */
template <class port_t>
class asioTimeoutReader
{
public:
    /**
     * @brief TimeoutReader
     * @param port: Port to read data from
     * @param timeoutMs: Max time to wait for incoming data
     */
    asioTimeoutReader(port_t & port, boost::int64_t timeoutMs) :
        m_ioService(port.get_io_service())
        , m_timer(m_ioService, boost::posix_time::milliseconds(timeoutMs))
        , m_port(port)
        , m_dataReceived(false)
    {

    }

    ~asioTimeoutReader()
    {

    }

    /**
     * @brief read: Reads data from port given in constructor,
     *              waits max time defined in constructor.
     * @param buffer: Received data will be stored in this buffer
     * @return true - data has been received
     *         false - timeout while waiting for data
     */
    bool read(boost::asio::mutable_buffers_1 buffer)
    {
        m_dataReceived = false;
     
        m_ioService.reset();

        m_timer.async_wait(boost::bind(&asioTimeoutReader::onTimeout, this));
        
        boost::asio::async_read
        (
            m_port,
            buffer,
            boost::bind
            (
                &asioTimeoutReader::handle_read,
                this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
            )
        );
       
        m_ioService.run();
        return m_dataReceived;
    }

private:

    void onTimeout()
    {
        if (m_dataReceived == false)
        {
            m_port.cancel();
        }
    }



    void handle_read(const boost::system::error_code & ec, std::size_t numberBytes)
    {
        MaEr::unused_variable(numberBytes, "Result is stored in a boost::asio::mutable_buffers_1 which already contains the size");

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

} // end namespace BoostHelper
} // end namespace MaEr

#endif // asioTimeoutReader_hpp
