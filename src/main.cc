/**
 * @file main.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Main function definition
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 */

#include <string>
#include <chrono>
#include <boost/program_options.hpp>

#include "Server.h"

namespace po = boost::program_options;

/**
 * main() purpose is to parse command-line arguments (if present) 
 * and run HTTP server that manages app.
 *
 * @note ROOT constant is defined by g++ call. It is absolute path to the
 *       projects root folder (calculated via SConstruct script)
 */
int main(int argc, char* argv[]){

    /* --- Load command-line parameters --- */

    po::options_description desc("Server options");
    desc.add_options()
        ("timeout", po::value<unsigned short>()->default_value(30), "Client's timeout [min].")
        ("config", po::value<std::string>()->default_value(std::string(ROOT) + "/config/http_server.conf"),
                    "Path to the config file.");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm); 

    /* --- Initialize HTTP server --- */
    
    Server server(
        std::chrono::minutes(vm["timeout"].as<unsigned short>()),
        vm["config"].as<std::string>()
    );
    
    // Returns on SIGINT (ctrl+C) or SIGTERM (kill)
    server.run();
    
    return EXIT_SUCCESS;
}
