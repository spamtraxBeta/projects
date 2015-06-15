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

#ifndef TRACE_HPP_
#define TRACE_HPP_

#include <string>
#include <sstream>
#include <stdexcept>

#define TRACE_THROW(msg)\
{\
	std::stringstream ss;						\
	ss											\
	<<"   file: "<<__FILE__<<"\n" 				\
	<<"   line: "<<__LINE__<<"\n"				\
	<<"   func: "<<__PRETTY_FUNCTION__<<"\n"	\
	<<"   message: "<<msg;						\
												\
	std::string result;							\
	while (!ss.eof())							\
	{											\
		std::string tmp;						\
		std::getline(ss, tmp);					\
		result += tmp;							\
		result += "\n";							\
	}											\
	throw std::runtime_error(result.c_str());	\
}

#define TRACE_THROW2(msg, msg2)\
{\
	std::stringstream ss;						\
	ss											\
	<<"   file: "<<__FILE__<<"\n" 				\
	<<"   line: "<<__LINE__<<"\n"				\
	<<"   func: "<<__PRETTY_FUNCTION__<<"\n"	\
	<<"   message: "<<msg<<"\n"					\
	<<"   message2: "<<msg2;					\
												\
	std::string result;							\
	while (!ss.eof())							\
	{											\
		std::string tmp;						\
		std::getline(ss, tmp);					\
		result += tmp;							\
		result += "\n";							\
	}											\
	throw std::runtime_error(result.c_str());	\
}


#endif /* TRACE_HPP_ */
