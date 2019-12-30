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
 * @brief 
 * 
 */
class Listener : public std::enable_shared_from_this<Listener>
{
    tcp::acceptor acceptor_;
    tcp::socket socket_;
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
