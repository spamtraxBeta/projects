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

#include "InterprocessComm.hpp"


InterprocessComm::InterprocessComm(RelaisFrame & request) :
    m_data(request)
    , m_firstAddress(0)
    , m_lastAddress(0)
    , m_execResult(Success)
    , m_version(INTERPROCESS_VERSION)
{
    m_msg[0] = 0;
}


InterprocessComm::~InterprocessComm()
{
}

void InterprocessComm::setMsg(const std::string & msg)
{
    unsigned int limit = maxMsgLength - 1;

    limit = (msg.size() < limit) ? msg.size() : limit;

    for (unsigned int i = 0; i < limit; ++i)
    {
        m_msg[i] = msg[i];
    }

    m_msg[limit] = 0;
}

std::ostream & operator<<(std::ostream & out, const InterprocessComm & valArg)
{
    out << "InterprocessComm{m_data = " << valArg.m_data << "; m_firstAddress = " << valArg.m_firstAddress << "; m_lastAddress = " << valArg.m_lastAddress << "; m_execResult = " << valArg.m_execResult << "; maxMsgLength = " << valArg.maxMsgLength << "; m_msg = " << valArg.m_msg << "} ";
    return out;
}
