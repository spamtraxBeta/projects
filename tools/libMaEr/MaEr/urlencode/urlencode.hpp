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

#ifndef URLENCODE_HPP_
#define URLENCODE_HPP_

#include <string>

namespace MaEr
{

    /**
     * Based on: http://stackoverflow.com/questions/3589936/c-urlencode-library-unicode-capable
     */
    std::string UrlEncode(const std::string & url);
    std::wstring UrlEncode(const std::wstring & url);

} /* namespace MaEr */

#endif /* URLENCODE_HPP_ */
