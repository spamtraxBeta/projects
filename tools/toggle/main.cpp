/**
* Writes 0 and 1 to stdout based on a pattern
* Copyright  2015
* Martin Ertl (ertl.martin@gmx.de)
*
* This file is part of "tools".
*
* toggle is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* toggle is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with toggle.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <boost/program_options.hpp>
#include <boost/asio.hpp>
#include <MaEr/toggle/toggle.hpp>


using namespace std;
namespace po = boost::program_options;


void onToggle(MaEr::CToggle::tenLevel level, int duration)
{
    if (level == MaEr::CToggle::nenLevel_hi)
    {
        cout<<1;
    }
    else
    {
        cout<<0;
    }

    cout<<endl;
}

int main(int ac, char * av[])
{
    // Declare the supported options.
    po::options_description desc("Takes a toggle pattern an writes 1 or 0 to STDOUT according to the pattern.");
    desc.add_options()
        ("help,h", "Show this help message")
        ("timebase,t", po::value<unsigned int>()->default_value(100), "Time in milliseconds for a single pattern.")
        ("pattern,p", po::value<std::string>(), "The toggle pattern. It is a string of dashes and underscores."
                                                "A dash means '1' while an underscore means '0'.\n"
                                                "The duration of each dash or underscore is defined by parameter 'timebase'.\n"
                                                "Example pattern: \"--_-_\".\n"
                                                "Assume the time base is set to '100ms' then '1' is output, after 200ms '0',"
                                                "after 100ms '1' after 100ms '0'\n\n"
                                                "Shortcuts: To make life a bit easier the pattern can be simplified by adding "
                                                "a number after a dash or underscore. The pattern above could be written as "
                                                "'-2_-_' instead. Other examples:\n"
                                                "'---__--___' '-3_2-2_3'\n"
                                                "'----------__________-_' '-10_10-_'\n"
         )
         ("repeat,r", po::value<unsigned int>()->default_value(1), "Repeat the pattern this number of times")
        ;

    po::positional_options_description p;
    p.add("text", -1);

    po::variables_map vm;

    try
    {
        po::store
        (
            po::command_line_parser(ac, av)
                .options(desc)
                .positional(p)
                .run(),
            vm
        );

        po::notify(vm);
    }
    catch (std::exception & ex)
    {
        cerr<<ex.what()<<endl;
        cerr<<desc<<endl;
        return -1;
    }


    if (vm.count("help"))
    {
        cout << desc << "\n";
        return 0;
    }


    unsigned int timebase = 0;
    std::string pattern;

    if (vm.count("timebase"))
    {
        timebase = vm["timebase"].as<unsigned int>();
    }

    if (vm.count("pattern"))
    {
        pattern = vm["pattern"].as<std::string>();
    }


    boost::asio::io_service io;

    MaEr::CToggle toggle(io);
    toggle.toggle.connect(onToggle);


    unsigned int repeat = vm["repeat"].as<unsigned int>();

    for (unsigned int i = 0; i< repeat; ++i)
    {
        toggle.add(timebase, pattern);
    }

    return 0;
}
