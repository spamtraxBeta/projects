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
#include <boost/program_options.hpp>


namespace MaEr
{

std::istream & operator>> (std::istream & in, byte_t & valArg)
{
    std::string str;
    in >> str;


    if (str.length() == 8)
    {
        valArg.m_value = 0;

        for (std::string::iterator i = str.begin(); i != str.end(); ++i)
        {
            valArg.m_value = valArg.m_value << 1;

            if (*i == '1')
            {
                valArg.m_value = valArg.m_value | 0x01;
            }
            else if (*i == '0')
            {

            }
            else
            {
                throw boost::program_options::validation_error(boost::program_options::validation_error::invalid_option_value, "Not a binary value");
            }
        }
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

        valArg.m_value = static_cast<uint8_t>(i);
    }

    //in >> i;


    return in;
}

std::ostream & operator << (std::ostream & out, const byte_t & valArg)
{
    out << static_cast<unsigned int>(valArg.m_value);
    return out;
}

} // end namespace MaEr

