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

#ifndef RelaisError_hpp
#define RelaisError_hpp

#include <stdexcept>
#include "RelaisFrame.hpp"

class RelaisError : public std::runtime_error
{
public:
    RelaisError(const char * msg, RelaisFrame & request, RelaisFrame & response) :
        runtime_error(msg)
        , m_request(request)
        , m_response(response)
    {

    }

    const RelaisFrame & request()const
    {return m_request;}

    const RelaisFrame & response()const
    {return m_response;}

private:
    RelaisFrame m_request;
    RelaisFrame m_response;
};

#endif // RelaisError_hpp
