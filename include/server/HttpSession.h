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
 * @brief 
 * 
 */
class HttpSession : public std::enable_shared_from_this<HttpSession>
{
private:
    tcp::socket socket_;
    beast::flat_buffer buffer_;
    std::shared_ptr<SharedState> state_;
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
