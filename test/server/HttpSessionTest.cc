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
#include "HttpSession.h"

BOOST_AUTO_TEST_SUITE( ServerSuite )
BOOST_AUTO_TEST_SUITE( HttpSessionClassSuite )

BOOST_AUTO_TEST_CASE( httpSessionConstructorTest )
{
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
        HttpSession httpSession(
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
        std::make_shared<HttpSession>(
            std::move(anotherSocket),
            server
        )->run()
    );
    
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()