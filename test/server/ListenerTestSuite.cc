/**
 * @file ListenerTest.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Set of tests for Listener class
 * @version 0.1
 * @date 2020-01-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <boost/test/unit_test.hpp>
#include "ListenerTest.h"

namespace ut = boost::unit_test;

BOOST_AUTO_TEST_SUITE( ServerSuite )
BOOST_AUTO_TEST_SUITE( ListenerClassSuite )

BOOST_AUTO_TEST_CASE( listenerConstructorTest )
{
    // Create context
    boost::asio::io_context context;
    //Create IP address 
    auto address = boost::asio::ip::make_address("0.0.0.0");
    // Create new client 1
    boost::asio::ip::tcp::endpoint endpoint(address, 8000);

    // Create server
    Server server(
        std::string(ROOT) + "/config/http_server.conf"
    );

    // Constructor test
    BOOST_REQUIRE_NO_THROW(
        ListenerTest listenerTest(
            server
        )
    );
}

BOOST_AUTO_TEST_CASE ( listenerRunTest, * ut::depends_on(
"ServerSuite/ListenerClassSuite/listenerConstructorTest"
))
{
    // Create context
    boost::asio::io_context context;

    //Create IP address 
    auto address = boost::asio::ip::make_address("0.0.0.0");
    // Create new client 1
    boost::asio::ip::tcp::endpoint endpoint(address, 8000);

    // Create server
    Server server(
        std::string(ROOT) + "/config/http_server.conf"
    );

    // run() method test
    BOOST_CHECK_NO_THROW(
        std::make_shared<ListenerTest>(
            server
        )->run()
    );
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()