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

#include "logHelper.hpp"

#include <iostream>

#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>

#include <boost/log/sources/logger.hpp>

#include <boost/log/support/date_time.hpp>

#include <boost/filesystem.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

namespace MaEr
{

void LogHelper::init()
{

    logging::add_console_log(std::clog, keywords::format = "%TimeStamp%: %Message%");

//    logging::add_console_log(std::cout, boost::log::keywords::format = expr::stream
//            << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")
//            << " [" << expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S")
//            //<< "] [" << expr::format_named_scope("Scope", keywords::format = "%n (%f:%l)")
//            << "] [" << expr::format_named_scope("Scope", keywords::format = "%f:%l; %n)")
//            << "] <" << expr::attr< SeverityLevel >("Severity")
//            << "> " << expr::message);

//    boost::shared_ptr< boost::log::core > core = boost::log::core::get();
//    core->add_global_attribute("LineID", boost::log::attributes::counter< unsigned int >(1));
//    //core->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());
//    //core->add_global_attribute("Scope", boost::log::attributes::named_scope());
//    core->add_thread_attribute("Scope", attrs::named_scope());

    logging::add_common_attributes();


}

void LogHelper::deinit()
{
    logging::core::get()->remove_all_sinks();
}

void LogHelper::addFileLogger
(
    const std::string & fileName,
    const std::string & logDirectory,
    const int maxSizePerLogFile,
    const int maxTotalSizeOfAllLogFiles,
    const int minFreeDiskSpace
)
{
// http://www.boost.org/doc/libs/1_54_0/libs/log/doc/html/log/detailed/sink_backends.html

    boost::shared_ptr< logging::core > core = logging::core::get();

    boost::shared_ptr< sinks::text_file_backend > backend =
        boost::make_shared< sinks::text_file_backend >
        (
            keywords::file_name = fileName.c_str(),
            keywords::rotation_size = maxSizePerLogFile,
            keywords::time_based_rotation = sinks::file::rotation_at_time_point(12, 0, 0)
        );


    // Wrap it into the frontend and register in the core.
    // The backend requires synchronization in the frontend.
    typedef sinks::synchronous_sink< sinks::text_file_backend > sink_t;
    boost::shared_ptr< sink_t > sink(new sink_t(backend));


    // init_file_collecting
    sink->locked_backend()->set_file_collector(sinks::file::make_collector(
        keywords::target = logDirectory,                // the target directory 
        keywords::max_size = maxTotalSizeOfAllLogFiles, // maximum total size of the stored files, in bytes
        keywords::min_free_space = minFreeDiskSpace     // minimum free space on the drive, in bytes 
        ));


    // Upon restart, scan the directory for files matching the file_name pattern
    sink->locked_backend()->scan_for_files();


    sink->set_formatter
        (

//        expr::stream
//        << "[" << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f") << "]"
//        << "\t<" << expr::attr< SeverityLevel >("Severity") << ">"
//        << "\t" << expr::message
//        << "\t\t[" << expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S.%f") << "]"
//        << "\t[" << expr::format_named_scope("Scope", keywords::format = "%n (%f:%l)")


        //expr::stream
        //<< expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d, %H:%M:%S.%f")
        //<< " [" << expr::format_date_time< attrs::timer::value_type >("Uptime", "%O:%M:%S")
        //<< "] [" << expr::format_named_scope("Scope", keywords::format = "%n (%f:%l)")
        ////<< "] [" << expr::format_named_scope("Scope", keywords::format = "%n ")
        //<< "] <" << expr::attr< severity_level >("Severity")
        //<< "> " << expr::message

        expr::format("<log id=\"%1%\" severity=\"%2%\" timestamp=\"%3%\" callStack=\"%4%\">%5%</log>")
        % expr::attr< unsigned int >("RecordID")                                // %1
        % expr::attr< SeverityLevel >("Severity")                               // %2
        % expr::attr< boost::posix_time::ptime >("TimeStamp")                   // %3
        //% expr::format_named_scope("Scope", keywords::format = "func=\"%n\" file=\"%f\" line=\"%l\")")    // %4
        % expr::format_named_scope("Scope", keywords::format = "(%n:%l)")    // %4
        % expr::xml_decor[expr::stream << expr::smessage] // %5
        );

    //boost::shared_ptr< boost::log::core > core = boost::log::core::get();
    core->add_global_attribute("RecordID", boost::log::attributes::counter< unsigned int >(1));
    core->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());
    core->add_thread_attribute("Scope", attrs::named_scope());
    core->add_sink(sink);


}

} // end namespace MaEr
