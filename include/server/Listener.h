/**
 * @file Listener.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Listener class' declaration
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef HTTP_LISTENER_H
#define HTTP_LISTENER_H

#include <memory>
#include <string>
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio.hpp>
#include "SharedState.h"

namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;
using error_code = boost::system::error_code;

/**
 * @brief Listener is designed to listen on the pointed socket and create new
 *        sessions when a new request aproaches
 */
class Listener : public std::enable_shared_from_this<Listener>
{
    /// Acceptor of the connection
    tcp::acceptor acceptor_;
    /// Socket to listen on
    tcp::socket socket_;
    /// Shared state of the application conatining crucial informations about app
    std::shared_ptr<SharedState> state_;

    void fail(error_code err_code, char const* what);
    void on_accept(error_code err_code);

public:
    Listener(
        asio::io_context& io_context,
        tcp::endpoint endpoint,
        std::shared_ptr<SharedState> const& state);

    // Start accepting incoming connections
    void run();
};

#endif
