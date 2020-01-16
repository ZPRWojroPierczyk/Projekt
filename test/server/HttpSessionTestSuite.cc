/**
 * @file HttpSessionTest.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Set of tests for HttpSession class
 * @version 0.1
 * @date 2020-01-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <boost/test/unit_test.hpp>
#include "HttpSessionTest.h"
#include "ServerTest.h"

BOOST_AUTO_TEST_SUITE( ServerSuite )
BOOST_AUTO_TEST_SUITE( HttpSessionClassSuite )

BOOST_AUTO_TEST_CASE( httpSessionConstructorTest )
{
    // Create server
    ServerTest serverTest(
        std::string(ROOT) + "/config/http_server.conf"
    );

    // Client's ID
    std::string client_id("0.0.0.0");
    // Create app instance
    serverTest.__join(client_id);

    // Create socket
    boost::asio::ip::tcp::socket socket(serverTest.__context);

    // Constructor test
    BOOST_REQUIRE_NO_THROW(
        HttpSession httpSession(
            serverTest.server,
            client_id,
            std::move(socket)
        )
    );

    // Create another socket ...
    boost::asio::ip::tcp::socket anotherSocket(serverTest.__context);

    // run() method test should throw an exception, as socket is not connected
    BOOST_CHECK_NO_THROW(
        std::make_shared<HttpSession>(
            serverTest.server,
            client_id,
            std::move(anotherSocket)
        )->run()
    );
    
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()