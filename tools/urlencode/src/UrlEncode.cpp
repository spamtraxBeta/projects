/*
 * UrlEncode.cpp
 *
 *  Created on: 24.12.2013
 *      Author: martin
 */

#include "UrlEncode.hpp"

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
