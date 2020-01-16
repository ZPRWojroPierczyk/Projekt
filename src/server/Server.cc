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

Server::Server(const std::string& config_file) :
    __clients(),
    __context()
{
    // Initial configuration loading
    try{
        __loadConfig(config_file);
    } catch (...){
        throw;
    }
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

void Server::run(){
    
    /* --- Make listener listen on the port --- */
    std::make_shared<Listener>(
        *this
    )->run();

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

void Server::__stop(){

    /**
     *  @note Erease all clients from the map. This action destroys:
     * 
     *        - shared_ptr to the steady_timer measuring timeout
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

bool Server::__join(const std::string& client_id){

    bool client_added = true;

    /* -- Specified client's session exists -- */
    if(__clients.count(client_id) != 0){

        client_added = false;
    } 
    /* -- New client to register -- */
    else{

        /* --- Create record's elements --- */
        auto timer = std::make_shared<boost::asio::steady_timer>(__context, __clientTimeout);
        auto model = std::make_shared<Model>(client_id);
        auto view = std::make_shared<View>(model);
        auto controller = std::make_shared<Controller>(model);
        
        /* --- Inser the new record ---*/
        __clients[client_id] = Client(
            std::move(timer),
            std::move(Instance(controller, view))
        );
    }

    return client_added;
}

bool Server::__leave(const std::string& client_id){
  
    if(__clients.count(client_id) == 0)
        return false;
    else{
        __clients[client_id].second.first = nullptr;
        __clients[client_id].second.second = nullptr;
        auto record = __clients.extract(client_id);
        record.key() = "None";
        __clients.insert(std::move(record));
        return true;
    }  
}

void Server::__loadConfig(const std::string& config_file){

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
    std::ifstream file(config_file.c_str());
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
        auto client_timeout = vm["client_timeout_min"].as<short>();
        auto session_timeout = vm["session_timeout_s"].as<short>();

        // Port correctness
        if(port <1024 || port > 65535)
            throw po::invalid_option_value("port number");

        // Check duaration corectness
        if (client_timeout > 0)
            __clientTimeout = std::chrono::minutes(client_timeout);
        else
            throw po::invalid_option_value("client imeout");

        // Check duaration corectness
        if (session_timeout > 0)
            __sessionTimeout = std::chrono::seconds(session_timeout);
        else
            throw po::invalid_option_value("session timeout");

        // Doc root existance
        struct stat info;
        if(stat(vm["doc_root"].as<std::string>().c_str(), &info) !=0)
            throw std::runtime_error("");
        
        __endpoint = asio::ip::tcp::endpoint(address, port);
        __docRoot = vm["doc_root"].as<std::string>();

    } catch (po::invalid_option_value &ex){
        throw;
    } catch(std::runtime_error &ex){
        throw po::invalid_option_value(std::string("documents root!"));
    } catch (std::exception &ex){
        throw po::invalid_option_value(std::string("ip address"));
    }
}

void Server::__clean(){
    __clients.erase("None");
    return;
}

const std::pair<std::shared_ptr<Controller>, std::shared_ptr<View>>&
Server::__getInstance(const std::string& client_id){
    return __clients[client_id].second;
}

const std::shared_ptr<boost::asio::steady_timer>&
Server::__getTimeoutTimer(const std::string& client_id){
    return __clients[client_id].first;
}