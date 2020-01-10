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
#include <stdexcept>
#include <sys/types.h>
#include <sys/stat.h>
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
    __clients()
{
    // Initial configuration loading
    try{
        loadConfig(configFile);
    } catch (...){
        throw;
    }
    
    // Check duaration corectness
    if (timeout > std::chrono::minutes(0))
        __timeout = timeout;
    else
        throw std::invalid_argument("Timeout cannot be negative!");
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
        *this
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
    
    bool clientAdded = true;

    /* -- Specified client's session exists -- */
    if(__clients.count(client) != 0){
        // Reset timeout for the client
        __clients[client].first->expires_after(__timeout);
        clientAdded = false;
    } 
    /* -- New client to register -- */
    else{
        // Create a new client on the list initializing it's timeout
        // timer and creating it's own (Controlle, View) pair
        __clients[client] = session(
            new boost::asio::steady_timer(__context, __timeout),
            // TODO#CPP: We should initializa MVC Triplet at now
            //           and pass pointers to the Model to View
            //           Controller, and Model and View pointers
            //           to Controller
            //
            // View should hold path to the actual web-stuff folder
            //
            // Model should hold client's ID, (e.g. socket converted to string)
            // to be able to distinguish loading between creating
            // new app's instance and loading old, save content
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

    return clientAdded;
}



/**
 * @brief Unregisters clients session from the list. Deallocates
 *        instance of the app assigned to the client
 * 
 * @param client : Client to unregister
 */
bool Server::leave(const asio::ip::tcp::endpoint& client){
    return __clients.erase(client);
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
        
        // Port correctness
        if(port <1024 || port > 65535)
            throw po::invalid_option_value("");
        
        // Doc root existance
        struct stat info;
        if(stat(vm["doc_root"].as<std::string>().c_str(), &info) !=0)
            throw std::runtime_error("");
        
        __endpoint = asio::ip::tcp::endpoint(address, port);
        __docRoot = vm["doc_root"].as<std::string>();

    } catch (po::invalid_option_value &ex){
        throw po::invalid_option_value(std::string("Server configuration: Invalid port number!"));
    } catch(std::runtime_error &ex){
        throw po::invalid_option_value(std::string("Pointed doc root does not exists!"));
    } catch (std::exception &ex){
        throw po::invalid_option_value(std::string("Server configuration: Invalid ip address!"));
    }

    // TODO#CPP: If parameters was succesfully loaded we should inform every View()
    //           instance about a new web-stuff's folder localization
}