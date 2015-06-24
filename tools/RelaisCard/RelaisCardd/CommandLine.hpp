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

#ifndef CommandLine_hpp
#define CommandLine_hpp

#include <string>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include <MaEr/types/byte_t.hpp>

class CommandLine : boost::noncopyable
{
public:

    static void create(int argc, const char  * argv[]);
    static CommandLine & instance();

    ~CommandLine();

	bool help()const
	{return m_help;}

	std::string helpText()const
	{return m_helpText;}

    bool showVersionInfo()const
    {return m_showVersionInfo;}

	std::string port()const
	{return m_port;}

	boost::asio::serial_port_base::baud_rate baudRate()const
	{ return m_baudRate; }

	boost::asio::serial_port_base::parity parity()const
	{ return m_parity; }

	boost::asio::serial_port_base::character_size characterSize()const
	{return  m_characterSize;}

	boost::asio::serial_port_base::stop_bits stopBits()const
	{ return m_stopBits; }

	boost::asio::serial_port_base::flow_control flowControl()const
    {return m_flowControl; }

    uint8_t initialPortState() const
    {return m_initialPortState.value();}

    bool hideConsole()const
    {return m_hideConsole;}

    const std::string & logFile()const
    {return m_logFile;}

private:
    static boost::shared_ptr<CommandLine> m_instance;

	std::string m_port;
	boost::asio::serial_port_base::baud_rate m_baudRate;
	boost::asio::serial_port_base::parity m_parity;
	boost::asio::serial_port_base::character_size m_characterSize;
	boost::asio::serial_port_base::stop_bits m_stopBits;
	boost::asio::serial_port_base::flow_control m_flowControl;
    MaEr::byte_t m_initialPortState;
    bool m_hideConsole;
	bool m_help;
    std::string m_logFile;
    bool m_showVersionInfo;

	std::string m_helpText;

    CommandLine(int argc, const char * argv[]);
};

#endif // CommandLine_hpp


