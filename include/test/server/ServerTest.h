/**
 * @file ServerTest.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Class used to test private interface of Server class
 * @version 0.1
 * @date 2020-01-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <string>
#include "Server.h"

class ServerTest
{
// Public types
public:
    /// Application's instance
    using instance = std::pair<std::shared_ptr<Controller>, std::shared_ptr<View>>;

    /// Type representing a single client
    using client = std::pair<std::shared_ptr<boost::asio::steady_timer>,
                              instance>;

    /// Set of clients
    using clientsMap = std::unordered_map<std::string, client>;

// Constructor
public:
    ServerTest(const std::string& configFile);

//Interface
public:
    /// Start server running
    void run();

    // Server's state manipulation
    void __stop();
    // Server's configuration
    void __loadConfig(const std::string& configFile);

    // Clients' list manipulation
    bool __join (const std::string& clientID);
    bool __leave (const std::string& clientID);

    // Internal clients management
    void __clean();

    // Get reference to the clients table
    clientsMap& getClients();

    // Utilities to get Server's private fields
    boost::asio::io_context&
    __getContext();

// Public members
public:
    Server __server;

};