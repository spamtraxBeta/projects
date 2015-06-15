/*
 * trace.hpp
 *
 *  Created on: 18.12.2013
 *      Author: martin
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
