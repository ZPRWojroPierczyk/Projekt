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

#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <boost/asio.hpp>

#include "View.h"
#include "Controller.h"
#include "Utilities.h"

class HttpSession;

/**
 * @brief HTTP Server responsible for managing client's sessions,
 *        application instances and port listeners.
 * 
 * @see   Listener.h
 * 
 * @note boost::hash in __clients member is used, since gcc 
 *       doesn't compile unordered_set with asio entopoint as
 *       a key
 */
class Server
{
// Constructors & Destructors
public:
    explicit Server(const std::chrono::minutes& timeout,
                    const std::string& configFile);

// Interface
public:
    void run();
    bool join (const boost::asio::ip::tcp::endpoint& client);
    bool leave (const boost::asio::ip::tcp::endpoint& client);
    void loadConfig(const std::string& configFile);
    std::string getDocRoot();

// Private types
private:
    /// Application's instance
    using app = std::pair<std::unique_ptr<Controller>, std::unique_ptr<View>>;
    /// Client's identificator (socket)
    using clientID = boost::asio::ip::tcp::endpoint;
    /// Type representing a single client
    using session = std::pair<std::unique_ptr<boost::asio::steady_timer>, app>;
    /// Set of clients
    using clientsList = std::unordered_map<clientID, session, ClientHash>;

// Private members
private:
    /// Context of the server
    boost::asio::io_context __context;
    /// Server's socket endpoint
    boost::asio::ip::tcp::endpoint __endpoint;
    /// Absolute path to the folder containing static files
    std::string __docRoot;

    /// Map of the active client's connections
    clientsList __clients;
    /// Server's timeout
    std::chrono::minutes __timeout;
};

#endif
