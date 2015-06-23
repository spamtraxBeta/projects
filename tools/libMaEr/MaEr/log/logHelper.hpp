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

#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/sources/logger.hpp>

#define logDeclaration() \
    BOOST_LOG_FUNCTION(); \
    boost::log::sources::severity_logger< MaEr::LogHelper::SeverityLevel > sevLogger01; \
    sevLogger01.add_attribute("Uptime", boost::log::attributes::timer());

#define logTrace()\
    BOOST_LOG_SEV(sevLogger01, MaEr::LogHelper::trace)<<"["<<__LINE__<<"]\t"

#define logDebug()\
    BOOST_LOG_SEV(sevLogger01, MaEr::LogHelper::debug)<<"["<<__LINE__<<"]\t"

#define logInfo()\
    BOOST_LOG_SEV(sevLogger01, MaEr::LogHelper::info)<<"["<<__LINE__<<"]\t"

#define logWarning()\
    BOOST_LOG_SEV(sevLogger01, MaEr::LogHelper::warning)<<"["<<__LINE__<<"]\t"

#define logError()\
    BOOST_LOG_SEV(sevLogger01, MaEr::LogHelper::error)<<"["<<__LINE__<<"]\t"

#define logFatal()\
    BOOST_LOG_SEV(sevLogger01, MaEr::LogHelper::fatal)<<"["<<__LINE__<<"]\t"

namespace MaEr
{
    class LogHelper
    {
    public:
        enum SeverityLevel
        {
            trace,
            debug,
            info,
            warning,
            error,
            fatal
        };


        static void init();
        static void deinit();
        static void addFileLogger
        (
            const std::string & fileName,
            const std::string & logDirectory = "log",
            const int maxSizePerLogFile = 1 * 1024 * 1024,
            const int maxTotalSizeOfAllLogFiles = 16 * 1024 * 1024,
            const int minFreeDiskSpace = 100 * 1024 * 1024
        );


    private:
        LogHelper();
        LogHelper(const LogHelper & rhs);
        LogHelper & operator=(const LogHelper & rhs);
    };




    // The formatting logic for the severity level
    template< typename CharT, typename TraitsT >
    inline std::basic_ostream< CharT, TraitsT > & operator<< 
    (
        std::basic_ostream< CharT, TraitsT > & strm,
        LogHelper::SeverityLevel lvl
    )
    {
        static const char* const str[] =
        {
            "trace",
            "debug",
            "info",
            "warning",
            "error",
            "fatal"
        };

        if (static_cast< std::size_t >(lvl) < (sizeof(str) / sizeof(*str)))
            strm << str[lvl];
        else
            strm << static_cast< int >(lvl);
        return strm;
    }
}

#endif // LOG_HPP

