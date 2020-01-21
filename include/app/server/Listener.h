/**
 * @file Listener.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Listener class' declaration
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 * @note This code was in large measue base on the Vinnie Falco's
 *       speach given on the CppCon 2018. His examples regarding
 *       boost::beast uasge are available on the git repositery
 *       on https://github.com/vinniefalco/CppCon2018
 */

#ifndef HTTP_LISTENER_H
#define HTTP_LISTENER_H

#include <memory>
#include <string>
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>

#include "Server.h"

class ListenerTest;

/**
 * @brief Listener is responsible for listening on the given socket
 *        and creating a new sessions when a new request aproaches.
 */
class Server::Listener : public std::enable_shared_from_this<Listener>
{
// Constructors & Destructors
public:

    /**
     * @brief Construct a new Server::Listener:: Listener object
     * 
     * @param server Reference to the server 
     * 
     * @note Listener instance should be created by the shared pointer and run()
     *       method should be called before the end of the creating scope. This
     *       approach to creation delegates listener's life-time responsibility
     *       to the object itself, which is desired behaviour.
     * 
     * @see Server.h
     */
    Listener(Server& server);

// Interface
public:

    /**
     * @brief Starts listening to the port
     * 
     * @note run() method creates another shared_ptr on the Listener 
     *       (first was created by Server::run) which, in turn, is again
     *       removed at the end of the scope.
     * 
     *       At this time it's __on_accept() method's responsibility to prolong
     *       object's lifetime. Method's call decides if object's should be
     *       maintained or deleted.
     * 
     * @see Server::run() (Server.cc)
     */
    void run();

// Private Friends
private:
    /// Class used to public private interface for unit tests
    friend class ListenerTest;

// Private members
private:

    /// Reference to the Server containing crucial informations about the server
    Server& __server;

    /// Object responsible for listening on the port and accepting new connections
    boost::asio::ip::tcp::acceptor __acceptor;

    /// Socket used by acceptor to connect to a new client
    boost::asio::ip::tcp::socket __socket;

// Private member methods
private:

    /**
     * @brief Handle a connection. Initializes new HttpSession and runs it
     *        in a new thread. Thread is detached, so it's lifetime depends
     *        on the session demands.
     *
     * @note HttpSession objects are create with shared_ptr in the same
     *       way Listener object was created. For details about the concept
     *       look at Server::run(), Server::Listener::run()
     *
     * @param err_code Reported error code
     * 
     * @see HttpSession
     * @see Server::Listener::run()
     * @see Server::run()
     */
    void __on_accept(const boost::system::error_code& err_code);

    /**
     * @brief Reporta a failure
     * 
     * @param err_code Reported error code
     * @param what Reason of the failure
     * 
     * @returns true If error was critical and Listener should be destroyed
     *               (e.g. closing server by ctrl+C, or calling close() on the __socket)
     * @returns false If error was not critical
     */
    bool __fail(const boost::system::error_code& err_code, char const* what);

};

#endif
