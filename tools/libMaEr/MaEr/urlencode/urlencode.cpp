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

std::string encimpl(std::string::value_type v)
{
    if (isalnum(v))
    {
        return std::string()+v;
    }
    else
    {
        std::ostringstream enc;
        enc
            << '%'
            << std::setw(2)
            << std::setfill('0')
            << std::hex
            << std::uppercase
            <<int(static_cast<unsigned char>(v));
        return enc.str();
    }
}

std::wstring wencimpl(std::wstring::value_type v)
{
    if (isalnum(v))
    {
        return std::wstring()+v;
    }
    else
    {
        std::wostringstream enc;
        enc
            << '%'
            << std::setw(2)
            << std::setfill<wchar_t>('0')
            << std::hex
            << std::uppercase
            <<int(static_cast<unsigned char>(v));
        return enc.str();
    }
}

std::string UrlEncode(const std::string & url)
{
    // store the modified query string
    std::string qstr;

    std::transform
    (
        url.begin(),
        url.end(),
        // Append the transform result to qstr
        boost::make_function_output_iterator
        (
            boost::bind
            (
                static_cast
                <
                    std::string& (std::string::*)(const std::string&)
                >
                (&std::string::append),
                &qstr,
                _1
            )
        ),
        encimpl
    );
    return qstr;
}


std::wstring UrlEncode(const std::wstring & url)
{
    // store the modified query string
    std::wstring qstr;

    std::transform
    (
        url.begin(),
        url.end(),
        // Append the transform result to qstr
        boost::make_function_output_iterator
        (
            boost::bind
            (
                static_cast
                <
                    std::wstring& (std::wstring::*)(const std::wstring&)
                >
                (&std::wstring::append),
                &qstr,
                _1
            )
        ),
        wencimpl
    );
    return qstr;
}

} /* namespace MaEr */
