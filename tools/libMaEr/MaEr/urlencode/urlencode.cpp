/**
*
* Copyright  2015
* Martin Ertl (spamtraxBeta@abwesend.de)
*
* This file is part of "libMaEr".
*
* libMaEr is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* libMaEr is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with libMaEr.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "urlencode.hpp"

#include <boost/function_output_iterator.hpp>
#include <boost/bind.hpp>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <iterator>
#include <iomanip>




namespace MaEr
{


std::string UrlEncode(const std::string & url)
{
    std::ostringstream result;
    for (std::string::const_iterator iter = url.begin(); iter != url.end(); ++iter)
    {
        if (isalnum(*iter))
        {
            result<<*iter;
        }
        else
        {
            result
                << '%'
                << std::setw(2)
                << std::setfill<typename std::string::value_type>('0')
                << std::hex
                << std::uppercase
                <<int(static_cast<unsigned char>(*iter));
        }
    }

    return result.str();
}

} // end namespace MaEr

