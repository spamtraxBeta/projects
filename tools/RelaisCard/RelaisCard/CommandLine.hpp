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
#include "../Common//RelaisFrame.hpp"
#include <MaEr/types/byte_t.hpp>

/**
 * @brief The CommandLine class takes
 */
class CommandLine
{
public:
	CommandLine(int argc, const char * argv[]);
	~CommandLine();


	bool help()const
	{return m_help;}

	std::string helpText()const
	{return m_helpText;}

    bool showVersionInfo()const
    {return m_showVersionInfo;}

    RelaisFrame::command_t command()const
    {return m_command;}

    uint8_t value()const
    {return m_value.value();}

    uint8_t address()const
    {return m_address.value();}


    const std::string & logFile()const
    {return m_logFile;}

private:



	bool m_help;

    RelaisFrame::command_t m_command;
    MaEr::byte_t m_value;
    MaEr::byte_t m_address;
    bool m_showVersionInfo;
    std::string m_logFile;

	std::string m_helpText;

};

#endif // CommandLine_hpp


