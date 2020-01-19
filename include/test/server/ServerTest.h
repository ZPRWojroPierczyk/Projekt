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

// Constructors & Destructors
public:

    ServerTest(const std::string& configFile);

// Public members
public:

    Server server;
    clientsMap& __clients;
    boost::asio::io_context& __context;

//Interface
public:

    void run();
    void __stop();
    void __loadConfig(const std::string& configFile);
    bool __join (const std::string& clientID);
    bool __leave (const std::string& clientID);
    void __clean();
    const std::shared_ptr<Controller>&
    __getController(const std::string& clientID);
    const std::shared_ptr<View>&
    __getView(const std::string& clientID);
    const std::shared_ptr<boost::asio::steady_timer>&
    __getTimeoutTimer(const std::string& clientID);

};