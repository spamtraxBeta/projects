/*
 * CToggle.cpp
 *
 *  Created on: 01.01.2014
 *      Author: martin
 */

#include "toggle.hpp"

#include <exception>
#include <iostream>
#include <sstream>

#include "../trace/trace.hpp"

namespace MaEr
{

using namespace std;

CToggle::CToggle(boost::asio::io_service & io_service):
    m_ioService(io_service)
    , m_timer(io_service, boost::posix_time::milliseconds(0))
    , e_ignoredChars("\\s")
    , e_invalidChars("[^-_0-9]")
    , e_startWithNumber("^[0-9]+")
    , e_startWithHi("^-")
    , e_startWithLo("^_")
{

}

CToggle::~CToggle()
{

}

void CToggle::add(unsigned int timeBaseMs, const std::string & input)
{
    std::string parsed = this->parse(input);
    std::list<timeStruct> timingList = createTimerList(timeBaseMs, parsed);


    //    -_---__--


    for (std::list<timeStruct>::iterator i = timingList.begin(); i != timingList.end(); ++i)
    {
        toggle(i->level, i->duration);
        m_timer.expires_from_now(boost::posix_time::millisec(i->duration));
        m_timer.wait();
    }
}


void CToggle::append(std::string & dst, int count, tenLevel level)
{
    char c = (level == nenLevel_lo) ? '_' : '-';

    for (int i = 0; i < count; ++i)
    {
        dst += c;
    }
}

std::string CToggle::parse(std::string input)
{
    std::string result;

    // remove ignored characters
    input = regex_replace(input, e_ignoredChars, "", boost::match_default | boost::format_perl);

    if (regex_search(input, e_invalidChars, boost::match_perl | boost::format_perl) == true)
    {
        TRACE_THROW2("Input contains invalid characters", input);
    }

    // replace invalid characters
    //input = regex_replace(input, e_invalidChars, "", boost::match_default | boost::format_perl);

    tenLevel currentLevel = nenLevel_last;


    while (input.length() > 0)
    {
        if (regex_search(input, e_startWithNumber, boost::match_perl | boost::format_perl) == true)
        {
            if (currentLevel == nenLevel_last)
            {
                TRACE_THROW2("input has invalid format, should start with hi or lo", input);
            }

            //cout<<"Starts with number"<<endl;

            int count = 0;
            stringstream ss(input);
            ss>>count;

            if (count > 0)
            {
                append(result, count - 1, currentLevel);
            }
            else
            {
                 TRACE_THROW2("invalid number detected, should be > 0:", count);
            }


            input = regex_replace(input, e_startWithNumber, "", boost::match_default | boost::format_perl);
        }
        else if (regex_search(input, e_startWithHi, boost::match_perl | boost::format_perl) == true)
        {
            //cout<<"Starts with hi"<<endl;
            currentLevel = nenLevel_hi;
            append(result, 1, currentLevel);
            input = regex_replace(input, e_startWithHi, "", boost::match_default | boost::format_perl);
        }
        else if (regex_search(input, e_startWithLo, boost::match_perl | boost::format_perl) == true)
        {
            //cout<<"Starts with lo"<<endl;
            currentLevel = nenLevel_lo;
            append(result, 1, currentLevel);
            input = regex_replace(input, e_startWithLo, "", boost::match_default | boost::format_perl);
        }
        else
        {
            TRACE_THROW2("Invalid input", input);
        }
    }

    return result;
}

std::list<CToggle::timeStruct> CToggle::createTimerList(unsigned int timeBaseMs, const std::string & parsedInput)
{
    std::list<timeStruct> result;

    if (parsedInput.empty())
    {
        return result;
    }

    tenLevel currentLevel = nenLevel_last;
    unsigned int currentDuration = 1;

    std::string::const_iterator i = parsedInput.begin();

    if (*i == hiChar)
    {
        currentLevel = nenLevel_hi;
    }
    else if (*i == loChar)
    {
        currentLevel = nenLevel_lo;
    }
    else
    {
        throw std::runtime_error("invalid character detected");
    }

    ++i;

    for (; i != parsedInput.end(); ++i)
    {
        switch (*i)
        {
            case hiChar:
            {
                if (currentLevel == nenLevel_hi)
                {
                    ++currentDuration;
                }
                else
                {
                    result.push_back(timeStruct(currentLevel, currentDuration * timeBaseMs));
                    currentDuration = 1;
                    currentLevel = nenLevel_hi;
                }
            }break;

            case loChar:
            {
                if (currentLevel == nenLevel_hi)
                {// state change detected
                    result.push_back(timeStruct(currentLevel, currentDuration * timeBaseMs));
                    currentDuration = 1;
                    currentLevel = nenLevel_lo;
                }
                else
                {
                    ++currentDuration;
                }
            }break;

            default:
            {
                throw std::runtime_error("Invalid character detected");
            }
        }
    }

    result.push_back(timeStruct(currentLevel, currentDuration * timeBaseMs));

    return result;
}

} /* namespace MaEr */
