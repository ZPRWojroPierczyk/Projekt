/**
 * @file ServerOptions.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief ServerOptions class' method definition
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <fstream>
#include <boost/asio.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include "ServerOptions.h"

namespace asio = boost::asio;
namespace po = boost::program_options;

#include <iostream>

/**
 * @brief Loads options used to establish a server (IP, port, ...)
 */
void ServerOptions::loadOptions(){

#ifdef ROOT
#ifdef BOOST_MSVC
    std::string config_file(std::string(ROOT) + "\\config\\http_server.conf");
#else
    std::string config_file(std::string(ROOT) + "/config/http_server.conf");
#endif
#else
    #ifdef BOOST_MSVC
    std::string config_file("C:\\http_server.conf");
#else
    std::string config_file("/temp/http_server.conf");
#endif
#endif

    // Prepare options set
    po::options_description opt("Server options");
    // NOTE : ROOT constant is defined by g++ call. It is absolute path to the
    //        projects root folder (calculated via SConstruct script)
    opt.add_options()
        // IP address
        ("ip", po::value<std::string>()->default_value("0.0.0.0"), "Server's ip address")
        // Port number
        ("port", po::value<int>()->default_value(8000), "Server's port number")
        // Absolute Path to the folder containing static files
        (
         "doc_root",
#ifdef ROOT
#ifdef BOOST_MSVC
        po::value<std::string>()->default_value(std::string(ROOT) + "\\web\\simple_client"),
#else
        po::value<std::string>()->default_value(std::string(ROOT) + "/web/simple_client"),
#endif     
#else
#ifdef BOOST_MSVC
         po::value<std::string>()->default_value(std::string("C:\\")),
#else
        po::value<std::string>()->default_value(std::string("/tmp")),
#endif
#endif
         "Path to the static files root folder"
        );

    // Open config_file and read options values
    std::ifstream ifs(config_file.c_str());
    po::variables_map vm;
    po::store(po::parse_config_file(ifs, opt), vm);
    notify(vm);

    // Save read values
    ipAddress = vm["ip"].as<std::string>();
    portNumber = vm["port"].as<int>();
    docRoot = vm["doc_root"].as<std::string>();
}