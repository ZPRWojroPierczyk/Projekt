#include <string>
#include "Utilities.h"


/**
 * @brief Standard custom hasher for boost::asio endpoint using
 *        std::string specialization of std::hash<> class
 * 
 * @param client : endpoint to be hashed 
 * @return std::size_t result hash
 */
std::size_t
ClientHash::operator()(
    const boost::asio::ip::tcp::endpoint& client
) const{
    return std::hash<std::string>()(
        client.address().to_string() +
        ":" + std::to_string(client.port())
    );
}