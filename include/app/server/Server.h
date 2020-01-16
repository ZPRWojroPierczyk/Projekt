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
class HttpSession;

class ServerTest;
class ListenerTest;

/**
 * @brief HTTP Server responsible for managing active client's list,
 *        application instances and port listeners. 
 * 
 * @note Class is not thread-save in this version, so a single thread
 *        should be asigned to call of run() method and no other thread
 *        should access Server object at that time.
 * 
 * @see   Listener.h
 * 
 */
class Server
{
// Friends
public:

    /**
     * @brief Listener needs acces to Server's methods to simplify
     *        communication Listener and client's table 
     * 
     */
    friend class Listener;
    
    /**
     * @brief  HttpSession needs access to Server's methods
     *         and fields to simplify communication between
     *         HttpSession and client's table.
     */
    friend class HttpSession;

// Constructors & Destructors
public:

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
    explicit Server(const std::string& configFile);

// Interface
public:

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
    void run();

// Private Friends
private:

    /// Class used to public private interface for unit tests
    friend class ServerTest;

    /// Class used to public Listener's private interface for unit tests
    friend class ListenerTest;

// Private types
private:
    /// Application's instance
    using instance = std::pair<std::shared_ptr<Controller>, std::shared_ptr<View>>;

    /// Type representing a single client record in the table
    using client = std::pair<std::shared_ptr<boost::asio::steady_timer>,
                              instance>;

    /// Table of clients
    using clientsMap = std::unordered_map<std::string, client>;

// Private classes
private:
    /// Listener class used to accept new connections
    class Listener;

// Private members
private:

    /// Table of the active client's
    clientsMap __clients;

    /// Absolute path to the folder containing static files
    std::string __docRoot;


    /* --- Networking members --- */

    /// Context of the server
    boost::asio::io_context __context;

    /// Server socket's local endpoint
    boost::asio::ip::tcp::endpoint __endpoint;


    /* --- Timeouts --- */

    /// Client's timeout
    std::chrono::minutes __clientTimeout;
    
    /// Session's timeout
    std::chrono::seconds __sessionTimeout;


// Private member methods
private:

    /**
     * @brief Stops server (if running). Perform all actions required to
     *        perform clean server's close. Informs io_contexts associated
     *        with clients about server's termination. Clears clients'
     *        table.
     */
    void __stop();

    /**
     * @brief Loads configuration from the file given with a path
     *        passed in argument. To load parameters server have to be
     *        stopped. Otherwise methods returns imediately.
     * 
     * @param configFile : Path to the config file
     */
    void __loadConfig(const std::string& configFile);

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
    bool __join (const std::string& clientID);

    /**
     * @brief Unregisters clients session from the table. Deallocates
     *        instance of the app assigned to the client
     * 
     * @param clientID : Client's ID (IP address)
     * 
     * @returns true : Client was ereased
     * @returns false : Client with the given ID doesn't exist
     */
    bool __leave (const std::string& clientID);


    /**
     * @brief Removes expired record from the clients table
     */
    void __clean();

    /** 
     * @param clientID 
     * @return Instance of the app assigned to the specified client
     */
    const std::pair<std::shared_ptr<Controller>, std::shared_ptr<View>>&
    __getInstance(const std::string& clientID);

    /**
     * @param clientID 
     * @return Pointer to the client's timeout timer
     */
    const std::shared_ptr<boost::asio::steady_timer>&
    __getTimeoutTimer(const std::string& clientID);

};

#endif
