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
#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include <boost/asio/signal_set.hpp>

#include "Listener.h"
#include "Model.h"

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
Server::Server(const std::string& configFile) :
    __context(),
    __cleanTimer(__context),
    __clients()
{
    // Initial configuration loading
    try{
        __loadConfig(configFile);
    } catch (...){
        throw;
    }
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Initializes tcp listener to listen on the server's port.
 *        Methods blocks the thread up to the moment of sending
 *        SIGINT or SIGTERM signal to it.
 * 
 * @note Listener's initializations is executed via shared_ptr which
 *       is deleted at the end of the scope. It's Listener's run()
 *       function responsibility to create another shared_ptr to prolong
 *       object's lifetime.
 */
void Server::run(){
    
    /* --- Make listener listen on the port --- */
    std::make_shared<Listener>(
        *this
    )->run();

    /* --- Run cleaning timer --- */
    __cleanTimer.expires_after(__sessionTimeout);
    __cleanTimer.async_wait(boost::bind(&Server::__clean, this, boost::asio::placeholders::error));

    /* --- Capture SIGINT and SIGTERM to perform a clean shutdown --- */
    asio::signal_set signals(__context, SIGINT, SIGTERM);
    signals.async_wait(
        [&](const boost::system::error_code&, int)
        {
            // Stop the server. This will cause run()
            // to return immediately, eventually destroying the
            // io_context and any remaining handlers in it.
            __stop();
        }
    );

    /* --- Run this thread as a __context's worker --- */
    __context.run();
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Private member methods ---------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Stops server (if running). Perform all actions required to
 *        perform clean server's close. Informs io_contexts associated
 *        with clients about server's termination. Clears clients'
 *        table.
 */
void Server::__stop(){

    /**
     *  @note Erease all clients from the map. This action destroys:
     * 
     *        - unique_ptr to the steady_timer measuring timeout
     *          which results in controlled timer object's destruction
     * 
     *        - shared_ptr to View and Controller instances. Another
     *          pointer is kept by the thread servicing http session
     *          and it will be destroyed when thread finishes execution
    */
    __clients.clear();

    /*--- Stop io_context local to the server's thread --- */
    __context.stop();
}



/**
 * @brief Looks for client in the actual clients table. If client found
 *        method resets timeout for this client. Otherwise it creates
 *        app instance for the client
 * 
 * @param clientID : Client's ID (IP address)
 * 
 * @return true : Client was added to the table. 
 * @return false : Client was in the table. Timeout was reset.
 */
bool Server::__join(const std::string& clientID){

    bool clientAdded = true;

    /* -- Specified client's session exists -- */
    if(__clients.count(clientID) != 0){

        // Reset timeout for the client
        __clients[clientID].first->expires_after(__clientTimeout);

        clientAdded = false;
    } 
    /* -- New client to register -- */
    else{

        /* --- Create record's elements --- */
        auto timer = std::make_unique<boost::asio::steady_timer>(__context, __clientTimeout);
        auto model = std::make_shared<Model>(Model(clientID));
        auto view = std::make_shared<View>(model, __docRoot);
        auto controller = std::make_shared<Controller>(model);
        
        /* --- Inser the new record ---*/
        __clients[clientID] = client(
            std::move(timer),
            std::move(instance(controller, view))
        );
    }

    /* -- Set handler to the timeout timer -- */
    __clients[clientID].first->async_wait(
        [this, clientID](const boost::system::error_code& ec)
        {
            
            // If timer was just refreshed return without unregistering client
            if(ec == boost::asio::error::operation_aborted)
                return;
                
            // If timer expired unregister client by leave()
            else{
                __leave(clientID);
                return;
            } 
        }
    );

    return clientAdded;

}



/**
 * @brief Unregisters clients session from the table. Deallocates
 *        instance of the app assigned to the client
 * 
 * @param clientID : Client's ID (IP address)
 * 
 * @returns true : Client was ereased
 * @returns false : Client with the given ID doesn't exist
 */
bool Server::__leave(const std::string& clientID){
  
    if(__clients.count(clientID) == 0)
        return false;
    else{
        __clients[clientID].second.first = nullptr;
        __clients[clientID].second.second = nullptr;
        auto record = __clients.extract(clientID);
        record.key() = "None";
        __clients.insert(std::move(record));
        return true;
    }  
}



/**
 * @brief Loads configuration from the file given with a path
 *        passed in argument. To load parameters server have to be
 *        stopped. Otherwise methods returns imediately.
 * 
 * @param configFile : Path to the config file
 */
void Server::__loadConfig(const std::string& configFile){

    // Prepare options set
    po::options_description opt("Server options");
    opt.add_options()
        // IP address
        ("ip", po::value<std::string>(), "Server's ip address")
        // Port number
        ("port", po::value<unsigned short>(), "Server's port number")
        // Absolute Path to the folder containing static files
        ("doc_root", po::value<std::string>(), "Path to the static files root folder")
        // Client's timeout. Time that should pass (without receiving any request from the client)
        // to close instance of the simulation associated with a client.
        ("client_timeout_min", po::value<short>()->default_value(30), "Client's timeout [min].")
        // Session's timeout. Time that should pass (without receiving any request from the client)
        // to close the session (and the thread associated with it).
        ("session_timeout_s", po::value<short>()->default_value(30), "Session's timeout [s].");

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
        auto clientTimeout = vm["client_timeout_min"].as<short>();
        auto sessionTimeout = vm["session_timeout_s"].as<short>();

        // Port correctness
        if(port <1024 || port > 65535)
            throw po::invalid_option_value("Server configuration: Invalid port number");

        // Check duaration corectness
        if (clientTimeout > 0)
            __clientTimeout = std::chrono::minutes(clientTimeout);
        else
            throw po::invalid_option_value("Server configuration: Timeout cannot be negative!");

        // Check duaration corectness
        if (sessionTimeout > 0)
            __sessionTimeout = std::chrono::seconds(sessionTimeout);
        else
            throw po::invalid_option_value("Server configuration: Timeout cannot be negative!");

        // Doc root existance
        struct stat info;
        if(stat(vm["doc_root"].as<std::string>().c_str(), &info) !=0)
            throw std::runtime_error("");
        
        __endpoint = asio::ip::tcp::endpoint(address, port);
        __docRoot = vm["doc_root"].as<std::string>();

    } catch (po::invalid_option_value &ex){
        throw;
    } catch(std::runtime_error &ex){
        throw po::invalid_option_value(std::string("Pointed doc root does not exists!"));
    } catch (std::exception &ex){
        throw po::invalid_option_value(std::string("Server configuration: Invalid ip address!"));
    }

    // Inform every View in active clients set that __docRoot changed
    for (auto it = __clients.begin(); it != __clients.end(); ++it)    {
        it->second.second.second->setDocRoot(__docRoot);
    }
}



/**
 * @brief Removes expired records from the clients table
 * @param errCode : Error code delivered when timer expires
 */
void Server::__clean(const boost::system::error_code& errCode){

    // If timer was just refreshed return without unregistering client
    if(errCode == boost::asio::error::operation_aborted)
        return;
        
    // If timer expired unregister client by leave()
    else{
        __clients.erase("None");
        __cleanTimer.expires_from_now(__sessionTimeout);
        __cleanTimer.async_wait(boost::bind(&Server::__clean, this, boost::asio::placeholders::error));
        return;
    }
}