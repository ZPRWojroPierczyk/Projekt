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

#include <atomic>
#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <tuple>
#include <boost/asio.hpp>

#include "View.h"
#include "Controller.h"

class Listener;
class ServerTest;
class ListenerTest;

/**
 * @brief HTTP Server responsible for managing active client's list,
 *        application instances and port listeners. 
 * 
 * @see   Listener.h
 * 
 * @note Class is not thread-save in this version, so a single thread
 *        should be asigned to call of run() method and no other thread
 *        should access Server object at that time.
 */
class Server
{
// Constructors & Destructors
public:
    explicit Server(const std::string& configFile);

// Friends
public:
    friend class Listener;

// Interface
public:
    void run();

// Private Friends
private:
    friend class ServerTest;
    friend class ListenerTest;

// Private types
private:
    /// Application's instance
    using instance = std::pair<std::shared_ptr<Controller>, std::shared_ptr<View>>;

    /// Type representing a single client
    using client = std::pair<std::unique_ptr<boost::asio::steady_timer>,
                              instance>;

    /// Set of clients
    using clientsMap = std::unordered_map<std::string, client>;

// Private classes
private:
    /// Listener class used to accept new connections
    class Listener;

// Private members
private:
    /// Context of the server
    boost::asio::io_context __context;
    /// Server socket's local endpoint
    boost::asio::ip::tcp::endpoint __endpoint;

    /// Client's timeout
    std::chrono::minutes __clientTimeout;
    /// Session's timeout
    std::chrono::seconds __sessionTimeout;
    /// Timer responsible for cleaning "None" records in __clients table
    boost::asio::steady_timer   __cleanTimer;

    /// Absolute path to the folder containing static files
    std::string __docRoot;

    /// Table of the active client's
    clientsMap __clients;

// Private member methods
private:
    // Server's state manipulation
    void __stop();
    // Server's configuration
    void __loadConfig(const std::string& configFile);

    // Clients' list manipulation
    bool __join (const std::string& clientID);
    bool __leave (const std::string& clientID);

    /// Internal clients management
    void __clean(const boost::system::error_code& errCode);
};

#endif
