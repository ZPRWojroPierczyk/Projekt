/**
 * @file SharedState.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief SharedState class' methods definitions
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <fstream>
#include <ios>
#include <memory>
#include <boost/program_options.hpp>
#include "Server.h"
#include "Listener.h"

namespace asio = boost::asio;
namespace po = boost::program_options;

/*--------------------------------------------------------------------------------*/
/*-------------------------------- Constructors ----------------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Construct a new HTTP Server. Initializes it with
 *        a given config file.
 * @param context io_context of the server
 * @param timeout client's connection timeout. If client doesn't
 *                interact with server within this time, application's
 *                instance is deleted.
 * @param configFile path to the configuration file
 * @throw 
 */
Server::Server(boost::asio::io_context* context,
               boost::posix_time::ptime timeout,
               const std::string configFile) :
    __context(context),
    __sessions(),
    __timeout(timeout)
{
    loadConfig(configFile);
}


/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

// TODO
void Server::run(){

}

// TODO
bool Server::join(const asio::ip::tcp::endpoint& client){

}
// TODO
void Server::leave(const asio::ip::tcp::endpoint& client){

}






/**
 * @brief Loads configuration from the file given with a path
 *        passed in argument
 * @param configFile 
 * @returns true 
 * @returns false 
 */
bool Server::loadConfig(const std::string configFile){

    // Prepare options set
    po::options_description opt("Server options");
    opt.add_options()
        // IP address
        ("ip", po::value<std::string>(), "Server's ip address")
        // Port number
        ("port", po::value<unsigned short>(), "Server's port number")
        // Absolute Path to the folder containing static files
        ("doc_root", po::value<std::string>(), "Path to the static files root folder");

    // Open config_file and read options values
    std::ifstream file(configFile.c_str());
    if(file.fail()){
        throw std::ios_base::failure("Server configuration file cannot be read!");
    }
    
    // Parse configuration file
    po::variables_map vm;
    try{ 
        po::store(po::parse_config_file(file, opt), vm);
        notify(vm);
    } catch (std::exception &ex){
        throw po::invalid_option_value(std::string("Server configuration:") + ex.what());
    }

    // Load configuration - validate config file
    try{
        auto address = asio::ip::make_address(vm["ip"].as<std::string>().c_str());
        auto port = vm["port"].as<unsigned short>();
        
        if(port <1024 || port > 65535)
            throw po::invalid_option_value("");
        
        __socket = asio::ip::tcp::endpoint(address, port);
        __docRoot = vm["doc_root"].as<std::string>();

    } catch (po::invalid_option_value &ex){
        throw po::invalid_option_value(std::string("Server configuration: Invalid port number!"));
    } catch (std::exception &ex){
        throw po::invalid_option_value(std::string("Server configuration: Invalid ip address!"));
    }
}