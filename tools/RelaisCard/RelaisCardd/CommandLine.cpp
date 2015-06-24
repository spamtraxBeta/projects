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
#include <boost/make_shared.hpp>

#include <MaEr/log/logHelper.hpp>

#include <istream>
#include <ostream>
#include <string>
#include <sstream>


boost::shared_ptr<CommandLine> CommandLine::m_instance;


namespace bsys = boost::system;
namespace po = boost::program_options;
namespace basio = boost::asio;

class Workaround
{
public:

	enum parity
	{
		parity_none = boost::asio::serial_port::parity::none,
		parity_odd = boost::asio::serial_port::parity::odd,
		parity_even = boost::asio::serial_port::parity::even
	};

	enum stopBits
	{
		stopBits_one = boost::asio::serial_port_base::stop_bits::one,
		stopBits_onepointfive = boost::asio::serial_port_base::stop_bits::onepointfive,
		stopBits_two = boost::asio::serial_port_base::stop_bits::two
	};


	enum flowControl
	{
		flowControl_none,
		flowControl_software,
		flowcontrol_hardware
	};

};

std::istream& operator>>(std::istream & in, Workaround::parity & parity)
{
	std::string token;
	in >> token;


	if (token == "none")
	{
		//parity = boost::asio::serial_port::parity(boost::asio::serial_port::parity::none);
		parity = Workaround::parity_none;
	}
	else if (token == "odd")
	{
		parity = (Workaround::parity_odd);
	}
	else if (token == "even")
	{
		parity = (Workaround::parity_even);
	}
	else
	{
		throw boost::program_options::validation_error(boost::program_options::validation_error::invalid_option_value, "Invalid parity value");
	}

	return in;
}

std::istream& operator>>(std::istream & in, Workaround::stopBits & sb)
{
	std::string token;
	in >> token;

	//enum type { one, onepointfive, two };

	if (token == "one")
	{
		//parity = boost::asio::serial_port::parity(boost::asio::serial_port::parity::none);
		sb = Workaround::stopBits_one;
	}
	else if (token == "onepointfive")
	{
		sb = Workaround::stopBits_onepointfive;
	}
	else if (token == "two")
	{
		sb = Workaround::stopBits_two;
	}
	else
	{
		throw boost::program_options::validation_error(boost::program_options::validation_error::invalid_option_value, "Invalid parity value");
	}

	return in;
}

std::istream& operator>>(std::istream & in, Workaround::flowControl & sb)
{
	std::string token;
	in >> token;

	if (token == "none")
	{
		//parity = boost::asio::serial_port::parity(boost::asio::serial_port::parity::none);
		sb = Workaround::flowControl_none;
	}
	else if (token == "software")
	{
		sb = Workaround::flowControl_software;
	}
	else if (token == "hardware")
	{
		sb = Workaround::flowcontrol_hardware;
	}
	else
	{
		throw boost::program_options::validation_error(boost::program_options::validation_error::invalid_option_value, "Invalid parity value");
	}

	return in;
}


void CommandLine::create(int argc, const char * argv[])
{
    BOOST_LOG_FUNCTION();

    if (!m_instance)
    {
        //m_instance = boost::make_shared<CommandLine>(argc, argv);
        m_instance = boost::shared_ptr<CommandLine>(new CommandLine(argc, argv));
    }
}

CommandLine & CommandLine::instance()
{
    BOOST_LOG_FUNCTION();

    if (!m_instance)
    {
        throw std::runtime_error("No instance of 'CommmandLine' available. Call 'create()' first");
    }
    else
    {
        return (*m_instance);
    }
}

CommandLine::CommandLine(int argc, const char * argv[]) :
	m_port("\\\\.\\COM16")
	, m_baudRate(19200)
	, m_parity(basio::serial_port_base::parity::none)
	, m_characterSize(8)
	, m_stopBits(basio::serial_port_base::stop_bits::one)
	, m_flowControl(basio::serial_port_base::flow_control::none)
    , m_initialPortState(0x00)
    , m_hideConsole(false)
	, m_help(false)
    , m_logFile("%Y-%m-%d %H_%M_%S (%6N).log")
    , m_showVersionInfo(false)
{
	unsigned int baudRate = m_baudRate.value();
	Workaround::parity parity = static_cast<Workaround::parity>(m_parity.value());
	unsigned int characterSize = m_characterSize.value();
    Workaround::stopBits sbit = static_cast<Workaround::stopBits>(m_stopBits.value());
    Workaround::flowControl flowControl = static_cast<Workaround::flowControl>(m_flowControl.value());

	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "Show help message")
        ("version", "Display version information")
		("port", po::value(&m_port)->default_value(m_port), "The port to be used")
		("baudRate,b", po::value<Workaround::parity>(&parity), "Set baud rate")
		("parity,p", po::value(&parity)->default_value(parity), "Set parity. Possible values: 'none', 'odd', 'even'")
		("characterSize,c", po::value(&characterSize)->default_value(characterSize), "Set charactersize")
		("stopBits,s", po::value(&sbit)->default_value(sbit), "Number of stop bits. Possible values: 'one', 'onepointfive', 'two'")
		("flowControl,f", po::value(&flowControl)->default_value(flowControl), "Set flow control. Possible values: 'none', 'software', 'hardware'")
        ("init,i", po::value(&m_initialPortState)->default_value(m_initialPortState), "Relais card(s) will be initialized with this value")
        ("noConsole,n", po::value(&m_hideConsole)->implicit_value(true)->default_value(m_hideConsole), "Hide console window")
        ("logFile,l", po::value(&m_logFile)->implicit_value(m_logFile)->default_value(m_logFile), "Write log file")
		;


	po::positional_options_description p;
	p.add("port", -1);

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
	ss << desc;
	std::istreambuf_iterator<char> eos;
	m_helpText = std::string(std::istreambuf_iterator<char>(ss), eos);

	if (vm.count("help"))
	{
		m_help = true;
	}

    if (vm.count("version"))
    {
        m_showVersionInfo = true;
    }

	m_baudRate = basio::serial_port_base::baud_rate(baudRate);
	m_parity = basio::serial_port_base::parity(static_cast<basio::serial_port_base::parity::type>(parity));
	m_characterSize = basio::serial_port_base::character_size(characterSize);
	m_stopBits = basio::serial_port_base::stop_bits(static_cast<basio::serial_port_base::stop_bits::type>(sbit));
	m_flowControl = basio::serial_port_base::flow_control(static_cast<basio::serial_port_base::flow_control::type>(flowControl));

}


CommandLine::~CommandLine()
{
    //BOOST_LOG_FUNCTION();
}

