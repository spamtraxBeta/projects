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

#include "RelaisFrame.hpp"

std::ostream & operator << (std::ostream & out, const RelaisFrame & valArg)
{
    out << "RelaisFrame{cmd = "<<valArg.cmd()<<"; address = "<<valArg.address()<<"; data = "<<valArg.data()<<"; checkSum = " << valArg.checksum()<< "} ";
    return out;
}

