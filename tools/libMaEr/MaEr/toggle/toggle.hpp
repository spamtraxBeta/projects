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

#ifndef CTOGGLE_HPP_
#define CTOGGLE_HPP_

#pragma GCC diagnostic push

//#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"

#include <boost/regex.hpp>
#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#pragma GCC diagnostic pop

#include <string>
#include <list>


namespace MaEr
{

/**
 * @brief The CToggle class provides a timer based toggle functionality
 */
class CToggle
{
public:

    /**
     * @brief The tenLevel enum defines the state (low or high)
     */
    enum tenLevel
    {
        nenLevel_lo,
        nenLevel_hi,
        nenLevel_last
    };
    typedef boost::signals2::signal<void (tenLevel, int)> sig_t;


    /**
     * @brief CToggle
     * @param io_service: the signal 'toggle' will be sent from within this io_service
     */
    CToggle(boost::asio::io_service & io_service);
    virtual ~CToggle();

    /**
     * @brief add adds a new pattern
     * @param timeBaseMs: time base in milliseconds
     * @param pattern: the toggle pattern
     *      example1: -_- sends 'toggle' signal high, low, high; each transition lasts timeBaseMs milliseconds
     *      example1: -3_2- sends 'toggle' signal high: 3 times timeBaseMs, low: 2 times timeBaseMs, high
     */
    void add(unsigned int timeBaseMs, const std::string & pattern);

    /**
     * @brief toggle signal
     */
    sig_t toggle;

private:
	boost::asio::io_service & m_ioService;
	boost::asio::deadline_timer m_timer;
	std::list<std::string> m_jobQueue;

	boost::regex e_ignoredChars;
	boost::regex e_invalidChars;
	boost::regex e_startWithNumber;
	boost::regex e_startWithHi;
	boost::regex e_startWithLo;



    static const char hiChar = '-';
    static const char loChar = '_';

    std::string parse(std::string input);
    void append(std::string & dst, int count, tenLevel level);

    struct timeStruct
    {
        timeStruct(tenLevel l, int d):
          level(l)
        , duration(d)
        {}
        tenLevel level;
        unsigned int duration;
    };

    std::list<timeStruct> createTimerList(unsigned int timeBaseMs, const std::string & parsedInput);
};

} /* namespace MaEr */

#endif /* CTOGGLE_HPP_ */
