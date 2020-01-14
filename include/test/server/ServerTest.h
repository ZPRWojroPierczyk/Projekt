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

class ServerTest{
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

    /// Internal clients management
    void __clean(const boost::system::error_code& errCode);

// Private members
private:
    Server server;

};