#include <boost/test/unit_test.hpp>
#include "Listener.h"

void listenerConstructorTest(){
    // Create context
    boost::asio::io_context context;

    //Create IP address 
    auto address = boost::asio::ip::make_address("0.0.0.0");
    // Create new client 1
    boost::asio::ip::tcp::endpoint endpoint(address, 8000);

    // Create server
    Server server(
        std::chrono::minutes(3),
        std::string(ROOT) + "/config/http_server.conf"
    );

    // Constructor test
    BOOST_REQUIRE_NO_THROW(
        Listener(
            context,
            endpoint,
            server
        )
    );

    // Construct another listener
    Listener listener(
        context,
        endpoint,
        server
    );

    // run() method test
    BOOST_CHECK_NO_THROW(
        listener.run()
    );
    
}