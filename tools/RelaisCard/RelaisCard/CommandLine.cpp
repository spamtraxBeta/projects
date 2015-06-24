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

#include "CommandLine.hpp"


#include <boost/program_options.hpp>
#include <istream>
#include <ostream>
#include <string>
#include <sstream>


namespace bsys = boost::system;
namespace po = boost::program_options;
namespace basio = boost::asio;


std::istream & operator >> (std::istream & in, RelaisFrame::command_t & command)
{
	std::string token;
	in >> token;


	if (token == "noop")
	{
        command = RelaisFrame::cmd_noop;
	}
	//else if (token == "setup")
	//{
 //       command = RelaisFrame::cmd_setup;
 //   }
    else if (token == "getPort")
    {
        command = RelaisFrame::cmd_getPort;
    }
    else if (token == "setPort")
    {
        command = RelaisFrame::cmd_setPort;
    }
    else if (token == "getOption")
    {
        command = RelaisFrame::cmd_getOption;
    }
    else if (token == "setOption")
    {
        command = RelaisFrame::cmd_setOption;
    }
    else if (token == "setSingle")
    {
        command = RelaisFrame::cmd_setSingle;
    }
    else if (token == "delSingle")
    {
        command = RelaisFrame::cmd_delSingle;
    }
    else if (token == "toggle")
    {
        command = RelaisFrame::cmd_toggle;
    }
	else
	{
		throw boost::program_options::validation_error(boost::program_options::validation_error::invalid_option_value, "Unknown command");
	}

	return in;
}



CommandLine::CommandLine(int argc, const char * argv[]) :
m_help(false)
, m_command(RelaisFrame::cmd_noop)
, m_value(0)
, m_address(1)
, m_showVersionInfo(false)
, m_logFile("Relaiscardls.log")
//, m_logFile("%Y-%m-%d %H_%M_%S (%6N).log")
{
	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
        ("help,h", "Print this help message.")
        ("version", "Display version information")
        ("command,c", po::value(&m_command)/*->default_value(m_command)*/, "Command to be executed. Possible values: 'noop', 'getPort', 'setPort', 'getOption', 'setOption', 'setSingle', 'delSingle', 'toggle'.")
        ("value,v", po::value(&m_value)->default_value(m_value), "Value for command. Use prefix '0x' for hex value, a sequence of 8 ones or zeros (for example '10010110') is interpreted as binary value.")
        ("address,a", po::value(&m_address)->default_value(m_address), "Address of relais card.")
        ("logFile,l", po::value(&m_logFile)->implicit_value(m_logFile)->default_value(m_logFile), "Write log file")
		;


	po::positional_options_description p;
	//p.add("command", 1);
    //p.add("value", 1);

	po::variables_map vm;

	po::store
	(
		po::command_line_parser(argc, argv)
		.options(desc)
		.positional(p)
		.run(),
		vm
	);

	po::notify(vm);

	// store entire help text in a string
	// (http://stackoverflow.com/questions/3203452/how-to-read-entire-stream-into-a-stdstring)
	std::stringstream ss;
    //std::ostream & ost = ss;
    ss << m_address;
	ss << desc;
	std::istreambuf_iterator<char> eos;
	m_helpText = std::string(std::istreambuf_iterator<char>(ss), eos);

	if (vm.count("help") || vm.count("command") == 0)
	{
		m_help = true;
	}

    if (vm.count("version"))
    {
        m_showVersionInfo = true;
    }

    if (RelaisFrame::commandHasData(m_command) == true && vm.count("value") == 0)
    {
        throw::std::runtime_error("Comamnd requires value");
    }
}


CommandLine::~CommandLine()
{
}

