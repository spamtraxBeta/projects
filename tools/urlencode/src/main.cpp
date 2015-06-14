//============================================================================
// Name        : urlEncode.cpp
// Author      : Martin Ertl
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include <boost/program_options.hpp>

#include "UrlEncode.hpp"


using namespace std;
namespace po = boost::program_options;


int main(int ac, char * av[])
{
    // Declare the supported options.
    po::options_description desc("URL encodes a text. Reads data from STDIN or from --text option and writes result to STDOUT");
    desc.add_options()
        ("help,h", "Show this help message")
        ("text,t", po::value<std::string>(), "The text to be encoded. If this option is missing input will be read from STDIN.")
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
    //po::store(po::parse_command_line(ac, av, desc), vm);

    if (vm.count("help"))
    {
        cout << desc << "\n";
        return 0;
    }

    std::string input;

    if (vm.count("text"))
    {
        input = vm["text"].as<std::string>();
    }
    else
    {
        // http://stackoverflow.com/questions/3203452/how-to-read-entire-stream-into-a-stdstring
        std::istreambuf_iterator<char> eos;
        std::string tmp(std::istreambuf_iterator<char>(cin), eos);
        input = tmp;
    }

    cout<<MaEr::UrlEncode(input);
    //wcout<<MaEr::UrlEncode(L"Hello");

    return 0;
}

