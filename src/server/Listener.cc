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

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;
using error_code = boost::system::error_code;


/**
 * @brief Construct a new Listener:: Listener object
 * 
 * @param ioc Associated boost::asio::io_context
 * @param endpoint Endpoint socket to listen on
 * @param state Shared state of the application
 */
Listener::Listener(asio::io_context& ioc,
                   tcp::endpoint endpoint,
                   std::shared_ptr<SharedState> const& state)
    : acceptor_(ioc)
    , socket_(ioc)
    , state_(state)
{
    error_code err_code;

    // Open the acceptor
    acceptor_.open(endpoint.protocol(), err_code);
    if(err_code){
        fail(err_code, "open");
        return;
    }

    // Allow address reuse
    acceptor_.set_option(asio::socket_base::reuse_address(true));
    if(err_code){
        fail(err_code, "set_option");
        return;
    }

    // Bind to the server address
    acceptor_.bind(endpoint, err_code);
    if(err_code){
        fail(err_code, "bind");
        return;
    }

    // Start listening for connections
    acceptor_.listen(
        asio::socket_base::max_listen_connections, err_code);
    if(err_code){
        fail(err_code, "listen");
        return;
    }
}

/**
 * @brief Runs async_listen to the port
 * 
 */
void Listener::run(){
    // Start accepting a connection
    acceptor_.async_accept(
        socket_,
        [self = shared_from_this()](error_code err_code)
        {
            self->on_accept(err_code);
        });
}

/**
 * @brief Report a failure
 * 
 * @param err_code Reported error code
 * @param what Reason of the failure
 */
void Listener::fail(error_code err_code, char const* what){
    // Don't report on canceled operations
    if(err_code == asio::error::operation_aborted)
        return;
    std::cerr << what << ": " << err_code.message() << "\n";
}

/**
 * @brief Handle a connection
 * 
 * @param err_code Reported error code
 */
void Listener::on_accept(error_code err_code){
    if(err_code)
        return fail(err_code, "accept");
    else
        // Launch a new session for this connection
        std::make_shared<HttpSession>(
            std::move(socket_),
            state_)->run();

    // Accept another connection
    acceptor_.async_accept(
        socket_,
        [self = shared_from_this()](error_code err_code)
        {
            self->on_accept(err_code);
        });
}
