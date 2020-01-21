/**
 * @file Listener.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Listener class' methods definitions
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 * @note This code was in large measue base on the Vinnie Falco's
 *       speach given on the CppCon 2018. His examples regarding
 *       boost::beast uasge are available on the git repositery
 *       on https://github.com/vinniefalco/CppCon2018
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

Server::Listener::Listener(Server& server) :
    __server(server),
    __acceptor(server.__context),
    __socket(server.__context)
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

void Server::Listener::__on_accept(const error_code& err_code){

    /* --- If critical error occured (e.g. SIGINT), return --- */
    if(err_code){
        if(__fail(err_code, "Error occured during acceptance of the new client!"))
            return;
    }
    /* --- Else, create HttpSession to manage session --- */
    else {
        auto client_id = __socket.remote_endpoint().address().to_string();

        // Register client to the if they are new
        __server.__join(client_id);

        // Create a new session for this client basing on the record in clients table
        std::make_shared<HttpSession>(
                __server,
                client_id,
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

bool Server::Listener::__fail(const error_code& err_code, char const* what){
    
    // Don't report on canceled operations
    if(err_code == boost::asio::error::operation_aborted)
        return true;

    std::cerr << what << ": " << err_code.message() << "\n";
    return false;
}