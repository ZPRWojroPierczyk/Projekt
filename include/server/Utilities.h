#include <cstdio>
#include <functional>
#include <boost/asio.hpp>

class ClientHash{
public:
    std::size_t operator()(const boost::asio::ip::tcp::endpoint& client) const;
};