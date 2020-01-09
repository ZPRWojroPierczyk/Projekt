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

#include <cstdlib>
#include <memory>
#include <boost/system/error_code.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast.hpp>

#include "Server.h"

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
                Server& server);

// Interface
public:
    void run();

// Private members
private:
    /// Socket associated with the connection
    boost::asio::ip::tcp::socket __socket;
    /// Buffer used in async_read() and async_write() operations
    boost::beast::flat_buffer __buffer;
    /// Reference to the Server pbject conatining crucial informations about app
    Server& __server;
    /// HTTP request from the client
    boost::beast::http::request<boost::beast::http::string_body> __req;

// Private member methods
private:
    
    boost::beast::string_view __mimeType(const boost::beast::string_view& path);
    std::string __pathCat(const boost::beast::string_view& base, const boost::beast::string_view& path);

    template<class Body, class Allocator, class Send>
    void __handleRequest(
        const boost::beast::string_view& docRoot,
        boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req,
        Send&& send
    );

    void __on_read(boost::system::error_code err_code, std::size_t);
    void __on_write(boost::system::error_code err_code, std::size_t, bool close);
    void __fail(const boost::system::error_code& err_code, char const* what);
};

#endif
