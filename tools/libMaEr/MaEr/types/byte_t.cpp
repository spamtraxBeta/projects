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

#include "byte_t.hpp"

#include <string>
#include <sstream>
#include <ctype.h>

namespace MaEr
{

std::istream & operator>> (std::istream & in, byte_t & valArg)
{
    std::string str;
    in >> str;

    try
    {
        if (str.length() == 0)
        {
            std::stringstream ss;
            ss<<"No input was given";
            throw std::runtime_error(ss.str());
        }

        // check if string contains numbers only
        for (std::string::const_iterator iter = str.begin(); iter != str.end(); ++iter)
        {
            if (*iter < '0' || *iter > '9')
            {
                std::stringstream ss;
                ss<<"Not a number: "<<str;
                throw std::runtime_error(ss.str());
            }
        }

        if (str.length() == 8)
        {
            uint8_t input = 0;

            for (std::string::iterator i = str.begin(); i != str.end(); ++i)
            {
                input = input << 1;

                if (*i == '1')
                {
                    input = input | 0x01;
                }
                else if (*i == '0')
                {

                }
                else
                {
                     //throw boost::program_options::validation_error(boost::program_options::validation_error::invalid_option_value, "Not a binary value");
                    std::stringstream ss;
                    ss<<"Not a binary number: "<<str;
                    throw std::runtime_error(ss.str());
                }
            }

            valArg.setValue(input);
        }
        else
        {
            std::stringstream ss;

            ss.unsetf(std::ios_base::basefield);
            ss << str;

            int i;
            ss >> i;

            if (i > 0xFF)
            {
                std::stringstream ssErr;
                ssErr << "Value '" << i << "' does not fit into a byte";

                std::istreambuf_iterator<char> eos;
                std::string msg(std::istreambuf_iterator<char>(ssErr), eos);

                throw std::out_of_range(msg.c_str());
            }
            else
            {
                valArg.setValue(static_cast<uint8_t>(i));
            }
        }
    }
    catch (...)
    {
        // restore stream object
        for (std::string::const_reverse_iterator iter = str.rbegin(); iter != str.rend(); ++iter)
        {
            in.putback(*iter);
        }
        throw;
    }

    return in;
}

std::ostream & operator << (std::ostream & out, const byte_t & valArg)
{
    out << static_cast<unsigned int>(valArg.value());
    return out;
}

} // end namespace MaEr

