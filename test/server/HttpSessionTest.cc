#include <boost/test/unit_test.hpp>
#include "HttpSession.h"

void httpSessionConstructorTest(){
    // Create context
    boost::asio::io_context context;
    // Create socket
    boost::asio::ip::tcp::socket socket(context);
    // Create server
    Server server(
        std::chrono::minutes(3),
        std::string(ROOT) + "/config/http_server.conf"
    );

    // Constructor test
    BOOST_REQUIRE_NO_THROW(
        HttpSession(
            std::move(socket),
            server
        )
    );

    // Create another socket ...
    boost::asio::ip::tcp::socket anotherSocket(context);
    // ... and another http session
    HttpSession httpSession(
            std::move(anotherSocket),
            server
    );

    // run() method test
    BOOST_CHECK_NO_THROW(
        httpSession.run()
    );
    
}