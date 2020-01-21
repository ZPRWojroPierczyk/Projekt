/**
 * @file HttpSession.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief HttpSession class' declaration
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 * @note This code was in large measue base on the Vinnie Falco's
 *       speach given on the CppCon 2018. His examples regarding
 *       boost::beast uasge are available on the git repositery
 *       on https://github.com/vinniefalco/CppCon2018
 * 
 */

#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include <chrono>
#include <cstdlib>
#include <functional>
#include <memory>
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/beast.hpp>

#include "Server.h"
#include "RequestHandler.h"
#include "View.h"
#include "Controller.h"

class HttpSessionTest;

/**
 * @brief Class representing a single HTTP session
 * 
 * Servers requests acquired from the client using RequestHandler class
 * to parse, interpret request and send back a response.
 * 
 */
class HttpSession : public std::enable_shared_from_this<HttpSession>
{
// Constructors & Destructors
public:
    
    /**
     * @brief Construct a new Http Session:: Http Session object
     * 
     * @param server Reference to the Server object owning session
     * @param client_id Identifier of the client initializing session
     * @param socket Boost:asio::ip::tcp::socket associated with the session
     * 
     * @note HttpSession instance should be created by the shared pointer and run()
     *       method should be called before the end of the creating scope. This
     *       approach to creation delegates session's life-time responsibility
     *       to the object itself, which is desired behaviour.
     */
    HttpSession(Server& server,
                const std::string& client_id,
                boost::asio::ip::tcp::socket&& socket);

// Interface
public:

    /**
     * @brief Run the session
     * 
     * @note run() method creates another shared_ptr on the HttpSession 
     *       (first was created by Listener::__on_accept()) which, in turn,
     *       is again removed at the end of the scope.
     * 
     *       At this time it's __onRead() method's responsibility to prolong
     *       object's lifetime. Method's call decides if object's should be
     *       maintained or deleted.
     * 
     * @see Listener::__on_accept() (Listener.cc)
     */
    void run();

// Private Friends
private:

    friend class HttpSessionTest;

// Private members
private:
    /// Client's ID
    const std::string __clientID;

    /// Reference to the Server object
    Server& __server;

    /// Socket associated with the connection
    boost::asio::ip::tcp::socket __socket;
    /// Buffer used in async_read() and async_write() operations
    boost::beast::flat_buffer __buffer;    
    /// HTTP request from the client
    boost::beast::http::request<boost::beast::http::string_body> __req;

    /// Timeout measuring session's timeout
    boost::asio::steady_timer __sessionTimeoutTimer;

    /// Handler responsible for communication with MVC instance
    RequestHandler __handler;

// Private member methods
private:

    /**
     * @brief Method called after getting a new request from the client
     * @param err_code : Error code from the async_read()
     */
    void __onRead(boost::system::error_code err_code, std::size_t);

    /**
     * @brief Method called after writing a response to the client
     * 
     * @param err_code :  Error code of the async_write()
     * @param close : True if connection is to be closed
     */
    void __onWrite(boost::system::error_code err_code, std::size_t, bool close);

    /**
     * @brief Reports a failure
     * 
     * @param err_code Reported error code
     * @param what Reason of the failure
     */
    void __fail(const boost::system::error_code& err_code, char const* what);

    /**
     * @brief Closes the socket finishing the tcp connetcion.
     */
    void __closeConnection();

};

#endif
