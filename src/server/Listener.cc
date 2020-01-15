/**
 * @file Listener.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Listener class' methods definitions
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <functional>
#include <iostream>
#include <thread>
#include "Listener.h"
#include "HttpSession.h"

using tcp = boost::asio::ip::tcp;
using error_code = boost::system::error_code;

/*--------------------------------------------------------------------------------*/
/*-------------------------------- Constructors ----------------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Construct a new Server::Listener:: Listener object
 * 
 * @param server : Reference to the server 
 * 
 * @note Listener instance should be created by the shared pointer and run()
 *       method should be called before the end of the creating scope. This
 *       approach to creation delegates listener's life-time responsibility
 *       to the object itself, which is desired behaviour.
 * 
 * @see Server.h
 */
Server::Listener::Listener(Server& server) :
    __acceptor(server.__context),
    __socket(server.__context),
    __server(server)
{
    error_code err_code;

    // Open the acceptor
    __acceptor.open(server.__endpoint.protocol(), err_code);
    if(err_code){
        __fail(err_code, "Cannot open listener.");
        return;
    }

    // Allow server's address reuse
    __acceptor.set_option(boost::asio::socket_base::reuse_address(true));
    if(err_code){
        __fail(err_code, "Cannot set listener's options.");
        return;
    }

    // Bind to the server address
    __acceptor.bind(server.__endpoint, err_code);
    if(err_code){
        __fail(err_code, "Cannot bind listener to the server's socket.");
        return;
    }

    // Initialize listening for connections
    __acceptor.listen(
        boost::asio::socket_base::max_listen_connections, err_code);
    if(err_code){
        __fail(err_code, "Cannot initialize listening.");
        return;
    }
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

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
void Server::Listener::run(){
    __acceptor.async_accept(
        __socket,
        [self = shared_from_this()](error_code err_code)
        {
            self->__on_accept(err_code);
        }
    );
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Private member methods ---------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Handle a connection. Initializes new HttpSession and runs it
 *        in a new thread. Thread is detached, so it's lifetime depends
 *        on the session demands.
 *
 * @note HttpSession objects are create with shared_ptr in the same
 *       way Listener object was created. For details about the concept
 *       look at Server::run(), Server::Listener::run()
 *
 * @param err_code : Reported error code
 * 
 * @see HttpSession
 * @see Server::Listener::run()
 * @see Server::run()
 */
void Server::Listener::__on_accept(const error_code& err_code){

    /* --- If critical error occured (e.g. SIGINT), return --- */
    if(err_code){
        if(__fail(err_code, "Error occured during acceptance of the new client!"))
            return;
    }
    /* --- Else, create HttpSession to manage session --- */
    else {
        auto clientID = __socket.remote_endpoint().address().to_string();

        // Register client to the if they are new
        __server.__join(clientID);
        


        // Create a new session for this client basing on the record in clients table
        std::make_shared<HttpSession>(
                __server,
                clientID,
                std::move(__socket)
        )->run();
    }

    // Accept another connection
    __acceptor.async_accept(
        __socket,
        [self = shared_from_this()](error_code err_code)
        {
            self->__on_accept(err_code);
        }
    );
}



/**
 * @brief Reporta a failure
 * 
 * @param : err_code Reported error code
 * @param : what Reason of the failure
 * 
 * @returns true : If error was critical and Listener should be destroyed
 *          (e.g. closing server by ctrl+C, or calling close() on the __socket)
 * @returns false : If error was not critical
 */
bool Server::Listener::__fail(const error_code& err_code, char const* what){
    
    // Don't report on canceled operations
    if(err_code == boost::asio::error::operation_aborted)
        return true;

    std::cerr << what << ": " << err_code.message() << "\n";
    return false;
}