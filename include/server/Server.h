/**
 * @file SharedState.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief SharedState class' declaration
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef SHARED_STATE_H
#define SHARED_STATE_H

#include <memory>
#include <string>
#include <unordered_set>
#include <utility>
#include <boost/functional/hash.hpp>
#include <boost/asio.hpp>
#include "ServerConfig.h"

class HttpSession;

/**
 *
 */
class Server{

// Constructors
public:
    explicit Server(boost::asio::io_context* context,
                    boost::posix_time::ptime timeout,
                    const std::string configFile);

// Interface
public:
    void run();
    bool join (const boost::asio::ip::tcp::endpoint& client);
    void leave (const boost::asio::ip::tcp::endpoint& client);
    bool loadConfig(const std::string& configFile);

// Private members
private:
    /// Type representing a single client
    using session = std::pair<boost::asio::ip::tcp::endpoint, boost::asio::steady_timer>;

    /// Context of the server
    boost::asio::io_context* __context;

    /// IP address of the server
    boost::asio::ip::tcp::endpoint __socket;
    /// Absolute path to the folder containing static files
    std::string __docRoot;
    
    /* 
    * NOTE: boost::hash is used, since on the gcc unordered_set
    *       cannot contain std::pair
    */

    /// List of the active client's connections
    std::unordered_set<session, boost::hash<session>> __sessions;
    /// Server's timeout
    boost::posix_time::ptime __timeout;
};

#endif
