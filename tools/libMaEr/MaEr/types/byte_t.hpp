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

#ifndef byte_t_hpp
#define byte_t_hpp

#include <cstdint>
#include <iostream>
#include <ostream>
#include <boost/program_options.hpp>

namespace MaEr
{

class byte_t
{
public:

    byte_t() :
        m_value(0)
    {}

    byte_t(const uint8_t & arg) :
        m_value(arg)
    {}

    byte_t(const byte_t & arg) :
        m_value(arg.m_value)
    {}

    uint8_t value()const
    {
        return m_value;
    }

    void setValue(uint8_t value)
    {
        m_value = value;
    }

    operator uint8_t() const
    { return m_value; }



private:
    uint8_t m_value;

};

/**
 * @brief operator >> read a string from this stream and interpret as integer value or if its
 * length is exactly 8 and containint just 0 and 1 => interpret as binary value
 * @param in
 * @param valArg
 * @return
 */
std::istream & operator>>(std::istream & in, byte_t & valArg);

std::ostream & operator<<(std::ostream & out, const byte_t & valArg);

} // end namespace MaEr

#endif // byte_t_hpp
