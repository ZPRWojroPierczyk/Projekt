#include <fstream>
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include "ServerOptions.h"

namespace asio = boost::asio;
namespace po = boost::program_options;

#include <iostream>

/**
 * @brief 
 * 
 */
void ServerOptions::loadOptions(){
    std::string config_file;

    // Prepare options set
    po::options_description opt("Server options");
    // NOTE : ROOT constant is defined by g++ call. It is absolute path to the
    //        projects root folder (calculated via SConstruct script)
    opt.add_options()
        ("ip", po::value<std::string>()->default_value("0.0.0.0"), "Server's ip address")
        ("port", po::value<int>()->default_value(8000), "Server's port number")
        (
         "doc_root",
#ifdef ROOT
         po::value<std::string>(&config_file)->default_value(std::string(ROOT) + "/web/simple_client"),
#else
         po::value<std::string>(&config_file)->default_value(std::string("/usr/local")),
#endif
         "Path to the static files root folder"
        )
    ;

    std::ifstream ifs(config_file.c_str());
    po::variables_map vm;
    po::store(po::parse_config_file(ifs, opt), vm);
    notify(vm);

    std::cout << config_file << std::endl;

    ip_address = vm["ip"].as<std::string>();
    port_number = vm["port"].as<int>();
    doc_root = vm["doc_root"].as<std::string>();
}