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

#include <algorithm>
#include <fstream>
#include <ios>
#include <memory>
#include <boost/program_options.hpp>
#include <boost/asio/signal_set.hpp>

#include "Server.h"
#include "Listener.h"

namespace asio = boost::asio;
namespace po = boost::program_options;

/*--------------------------------------------------------------------------------*/
/*------------------------ Constructors and destructors --------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Construct a new HTTP Server. Initializes it with
 *        a given config file.
 * @param context : io_context of the server
 * @param timeout : client's connection timeout. If client doesn't
 *                interact with server within this time, application's
 *                instance is deleted.
 * @param configFile : path to the configuration file
 * @throw boost::program_options::invalid_option_value : If failed to load
 *        configuration from a given file.
 */
Server::Server(const std::chrono::minutes& timeout,
               const std::string& configFile) :
    __context(),
    __clients(),
    __timeout(timeout)
{
    // Initial configuration loading
    try{
        loadConfig(configFile);
    } catch (std::exception& e){
        throw po::invalid_option_value(e.what());
    }
}



/**
 * @brief Destroy the Server and all instances of the app
 *        that have been created with 'new'.
 */
Server::~Server(){
    //Call destructors
    for(auto iter = __clients.begin(); iter != __clients.end(); iter++){
        // Timer instances
          delete iter->second.first;
        // Applitaction instanced
        delete iter->second.second.first;
        delete iter->second.second.second;
    }
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Initializes tcp listener to listen on the server's port.
 * 
 * @note Listener's initializations is executed via shared_ptr which
 *       is deleted at the end of the scope. It's Listener's run()
 *       function responsibility to create another shared_ptr to prolong
 *       object's lifetime.
 */
void Server::run(){

    /* --- Make listener listen on the port --- */

    std::make_shared<Listener>(
        __context,
        __endpoint,
        std::shared_ptr<Server>(this)
    )->run();

    /* --- Capture SIGINT and SIGTERM to perform a clean shutdown --- */

    asio::signal_set signals(__context, SIGINT, SIGTERM);
    signals.async_wait(
        [&](boost::system::error_code const&, int)
        {
            // Stop the io_context. This will cause run()
            // to return immediately, eventually destroying the
            // io_context and any remaining handlers in it.
            __context.stop();
        }
    );

    /* --- Run this thread as a __context's worker --- */

    __context.run();

    return;
}



/**
 * @brief Looks for client in the actual sessions' list. If client found
 *        method resets timeout for this client. Otherwise it creates
 *        app instance for the client
 * 
 * @param client : Client's endpoint (socket)
 * @return true : Client was on the actual list. Timeout was reset.
 * @return false : Client was added to the sessions list. New instance of the
 *                 app has been created.
 */
bool Server::join(const asio::ip::tcp::endpoint& client){
    
    bool isOnList = false;

    /* -- Specified client's session exists -- */
    if(__clients.count(client) != 0){
        // Reset timeout for the client
        __clients[client].first->expires_after(__timeout);
        isOnList = true;
    } 
    /* -- New client to register -- */
    else{
        // Create a new client on the list initializing it's timeout
        // timer and creating it's own (Controlle, View) pair
        __clients[client] = session(
            new boost::asio::steady_timer(__context, __timeout),
            app(new Controller, new View)
        );
    }

    /* -- Set handler to the timeout timer -- */
    __clients[client].first->async_wait(
        [this, &client](const boost::system::error_code& ec)
        {
            // If timer was canceled return without unregistering client
            if(ec == boost::asio::error::operation_aborted)
                return;
            // If timer expired unregister client by leave()
            else
                leave(client);
        }
    );

    return isOnList;
}



/**
 * @brief Unregisters clients session from the list. Deallocates
 *        instance of the app assigned to the client
 * 
 * @param client : Client to unregister
 */
void Server::leave(const asio::ip::tcp::endpoint& client){
    // Delete client's timeout timer
    delete __clients[client].first;
    // Deallocate instance of the app
    delete __clients[client].second.first;
    delete __clients[client].second.second;
    // Remove client's record from the list
    __clients.erase(client);
}



/**
 * @returns std::string path to the folder containing static web files
 */
std::string Server::getDocRoot() { return __docRoot; }



/**
 * @brief Loads configuration from the file given with a path
 *        passed in argument
 * @param configFile : Path to the config file
 */
void Server::loadConfig(const std::string& configFile){

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
        
        __endpoint = asio::ip::tcp::endpoint(address, port);
        __docRoot = vm["doc_root"].as<std::string>();

    } catch (po::invalid_option_value &ex){
        throw po::invalid_option_value(std::string("Server configuration: Invalid port number!"));
    } catch (std::exception &ex){
        throw po::invalid_option_value(std::string("Server configuration: Invalid ip address!"));
    }
}