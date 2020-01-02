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
#include "SharedState.h"

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;
using error_code = boost::system::error_code;

/**
 * @brief Class representing a single HTTP session
 * 
 * Servers all requests acquired from the client including reading, decoding
 * request and sending the response back
 */
class HttpSession : public std::enable_shared_from_this<HttpSession>
{
private:
    /// Socket associated with the connection
    tcp::socket socket_;
    /// Buffer used in async_read() and async_write() operations
    beast::flat_buffer buffer_;
    /// Shared state of the application conatining crucial informations about app
    std::shared_ptr<SharedState> state_;
    /// HTTP request from the client
    http::request<http::string_body> req_;

    void fail(error_code err_code, char const* what);
    void on_read(error_code err_code, std::size_t);
    void on_write(
        error_code err_code, std::size_t, bool close);

public:
    HttpSession(
        tcp::socket socket,
        std::shared_ptr<SharedState> const& state);

    void run();
};

#endif
