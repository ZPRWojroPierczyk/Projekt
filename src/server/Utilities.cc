#include <string>
#include "Utilities.h"


std::size_t
ClientHash::operator()(
    const boost::asio::ip::tcp::endpoint& client
) const{
    return std::hash<std::string>()(
        client.address().to_string() +
        ":" + std::to_string(client.port())
    );
}