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

#include <iostream>
#include "Listener.h"
#include "HttpSession.h"

using tcp = boost::asio::ip::tcp;
using error_code = boost::system::error_code;

/*--------------------------------------------------------------------------------*/
/*-------------------------------- Constructors ----------------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Construct a new Listener:: Listener object
 * 
 * @param ioc : Associated io_context (usually the one used hold by server)
 * @param endpoint : Endpoint socket to listen on (usually server's socket)
 * @param server : Server to interact with 
 * 
 * @note Listener instance should be created by the shared pointer and run()
 *       method should be called before the end of the creating scope. This
 *       approach to creation delegates listener's life-time responsibility
 *       to the object itself, which is desired behaviour.
 * 
 * @see Server.h
 */
Listener::Listener(boost::asio::io_context& context,
                   const tcp::endpoint& endpoint,
                   Server& server) :
    __acceptor(context),
    __socket(context),
    __server(server)
{
    error_code err_code;

    // Open the acceptor
    __acceptor.open(endpoint.protocol(), err_code);
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
    __acceptor.bind(endpoint, err_code);
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
void Listener::run(){
    __acceptor.async_accept(
        __socket,
        [self = shared_from_this()](error_code err_code)
        {
            self->__on_accept(err_code);
        });
}


/*--------------------------------------------------------------------------------*/
/*----------------------------- Private member methods ---------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Handle a connection
 * @param err_code : Reported error code
 * 
 * @note HttpSession objects are create with shared_ptr in the same
 *       way Listener object was created. For details about the concept
 *       look at Server::run(), Listener::run()
 * 
 * @see Listener::run()
 * @see Server::run()
 */
void Listener::__on_accept(const error_code& err_code){
    if(err_code){
        if(__fail(err_code, "accept"))
            return;
    }
    else {
        // Register client to the if they are new
        __server.join(__socket.remote_endpoint());

        // Launch a new session for this connection
        std::make_shared<HttpSession>(
            std::move(__socket),
            __server
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
bool Listener::__fail(const error_code& err_code, char const* what){
    
    // Don't report on canceled operations
    if(err_code == boost::asio::error::operation_aborted)
        return true;

    std::cerr << what << ": " << err_code.message() << "\n";
    return false;
}