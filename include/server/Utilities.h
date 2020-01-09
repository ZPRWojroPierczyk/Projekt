#include <cstdio>
#include <functional>
#include <boost/asio.hpp>

/**
 * @brief Standard hasher functor used to store boost::asio endpoint
 *        as a key in std::unordered_map.
 */
class ClientHash{
public:
    std::size_t operator()(const boost::asio::ip::tcp::endpoint& client) const;
};