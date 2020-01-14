/**
 * @file HttpSession.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief HttpSession class' declaration
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include <chrono>
#include <cstdlib>
#include <memory>
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>

#include "RequestHandler.h"
#include "View.h"
#include "Controller.h"

class HttpSessionTest;

/**
 * @brief Class representing a single HTTP session
 * 
 * Servers all requests acquired from the client including reading, decoding
 * request and sending the response back
 */
class HttpSession : public std::enable_shared_from_this<HttpSession>
{
// Constructors
public:
    HttpSession(boost::asio::ip::tcp::socket&& socket,
                const std::pair<std::shared_ptr<Controller>, std::shared_ptr<View>> & instance,
                std::chrono::seconds timeout,
                boost::asio::io_context& context);

// Interface
public:
    void run();

// Private Friends
private:
    friend class HttpServerTest;

// Private members
private:
    /// Socket associated with the connection
    boost::asio::ip::tcp::socket __socket;
    /// Buffer used in async_read() and async_write() operations
    boost::beast::flat_buffer __buffer;    
    /// HTTP request from the client
    boost::beast::http::request<boost::beast::http::string_body> __req;

    /// Handler responsible for communication with MVC instance
    RequestHandler __handler;

    /// Session's timeout
    std::chrono::seconds __timeout;
    /// Timeout timer
    boost::asio::steady_timer __timer;

// Private member methods
private:
    void __onRead(boost::system::error_code err_code, std::size_t);
    void __onWrite(boost::system::error_code err_code, std::size_t, bool close);
    void __fail(const boost::system::error_code& err_code, char const* what);
    void __closeConnection();
};

#endif
