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

#ifndef InterprocesssComm_hpp
#define InterprocesssComm_hpp

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <string>
#include "RelaisFrame.hpp"
#include <MaEr/types/byte_t.hpp>

#define MESSAGE_QUEUE_NAME "msgqRelaisCardd"
#define MESSAGE_QUEUE_MAX_NUM_MESSAGES 100
#define MESSAGE_QUEUE_MAX_MESSAGE_SIZE 100
#define INTERPROCESS_OBJECT_NAME "InterprocessObject"
#define INTERPROCESS_VERSION 0x03

class InterprocessComm
{
public:


    enum tenExecResult
    {
        Success,
        OperationNotSupported,
        Error
    };

    InterprocessComm(RelaisFrame & data);
    ~InterprocessComm();

    inline boost::interprocess::interprocess_mutex & mutex()
    {return m_mutex;}

    inline const RelaisFrame & data()const
    {return m_data;}
    inline void setData(const RelaisFrame & data)
    {m_data = data;}

    MaEr::byte_t firstAddress()const
    {return m_firstAddress;}
    void setFirstAddress(MaEr::byte_t first)
    {m_firstAddress = first;}

    MaEr::byte_t lastAddress()const
    {return m_lastAddress;}
    void setLastAddress(MaEr::byte_t last)
    {m_lastAddress = last;}

    tenExecResult execResult()const
    {return m_execResult;}
    void setExecResult(tenExecResult result)
    {m_execResult = result;}

    std::string getMsg()const
    {return std::string(m_msg);}
    void setMsg(const std::string & msg);

    unsigned int version()const
    {return m_version;}

private:
    boost::interprocess::interprocess_mutex m_mutex;
    RelaisFrame m_data;
    MaEr::byte_t m_firstAddress;
    MaEr::byte_t m_lastAddress;
    tenExecResult m_execResult;
    unsigned int m_version;

    static const int maxMsgLength = 128;
    char m_msg[maxMsgLength];

    friend std::ostream & operator<<(std::ostream & out, const InterprocessComm & valArg);
    
};

#endif // InterprocesssComm_hpp
