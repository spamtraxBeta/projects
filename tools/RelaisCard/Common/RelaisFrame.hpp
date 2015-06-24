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

#ifndef RelaisFrame_hpp
#define RelaisFrame_hpp

#include <boost/integer.hpp>
#include <boost/asio.hpp>
#include <vector>

#include <MaEr/types/byte_t.hpp>

class RelaisFrame
{
public:
    enum command_t
    {
        cmd_noop = 0,
        cmd_setup = 1,
        cmd_getPort = 2,
        cmd_setPort = 3,
        cmd_getOption = 4,
        cmd_setOption = 5,
        cmd_setSingle = 6,
        cmd_delSingle = 7,
        cmd_toggle = 8
    };

    RelaisFrame()
    {
        m_data[posCommand] = 0;
        m_data[posAddress] = 0;
        m_data[posData] = 0;
        m_data[posChecksum] = 0;

        this->updateChecksum();
    }

    RelaisFrame(command_t command, boost::uint8_t address, boost::uint8_t data)
    {
        m_data[posCommand] = command;
        m_data[posAddress] = address;
        m_data[posData] = data;

        this->updateChecksum();
    }

    RelaisFrame(const boost::uint8_t * data, boost::uint8_t dataSize)
    {
        if (dataSize != bufferSize)
        {
            throw std::runtime_error("invalid data size");
        }

        m_data[posCommand] = data[posCommand];
        m_data[posAddress] = data[posAddress];
        m_data[posData] = data[posData];
        m_data[posChecksum] = data[posChecksum];
    }

    RelaisFrame(const RelaisFrame & rhs)
    {
        m_data[posCommand] = rhs.m_data[posCommand];
        m_data[posAddress] = rhs.m_data[posAddress];
        m_data[posData] = rhs.m_data[posData];
        m_data[posChecksum] = rhs.m_data[posChecksum];
    }

    inline boost::asio::mutable_buffers_1 buffer()
    {     
        boost::asio::mutable_buffers_1 b = boost::asio::buffer(m_data, bufferSize);
        return b;
    }


    bool checksumOk()const
    {
        bool ok = (m_data[posCommand] ^ m_data[posAddress] ^ m_data[posData]) == m_data[posChecksum];
        return ok;
    }

    command_t cmd()const
    {
        return static_cast<command_t>(m_data[posCommand]);
    }

    MaEr::byte_t address()const
    {
        return m_data[posAddress];
    }

    MaEr::byte_t data()const
    {
        return m_data[posData];
    }
    void setData(uint8_t value)
    {
        m_data[posData] = value;
        this->updateChecksum();
    }
    MaEr::byte_t checksum()const
    {
        return m_data[posChecksum];
    }

    void clear(boost::uint8_t pattern = 0x00)
    {
        for (int i = 0; i < bufferSize; ++i)
        {
            m_data[i] = pattern;
        }
    }


    static bool commandHasData(command_t commmand)
    {
        bool result = false;

        switch (commmand)
        {
            case RelaisFrame::cmd_setPort:
            case RelaisFrame::cmd_setOption:
            case RelaisFrame::cmd_setSingle:
            case RelaisFrame::cmd_delSingle:
            {
                result = true;
            }break;

            default:
            {
                result = false;
            }
        }

        return result;
    }
    bool commandHasData() const
    {
        bool result = commandHasData(this->cmd());
        return result;
    }

private:
    static const int bufferSize = 4;
    boost::uint8_t m_data[bufferSize];

    //std::vector<uint8_t> m_data;
    
    enum pos_t
    {
        posCommand = 0,
        posAddress = 1,
        posData = 2,
        posChecksum = 3
    };


    void updateChecksum()
    {
        m_data[posChecksum] = m_data[posCommand] ^ m_data[posAddress] ^ m_data[posData];
    }

    friend std::ostream & operator<<(std::ostream & out, const RelaisFrame & valArg);

};

#endif // RelaisFrame_hpp
