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

BOOST_AUTO_TEST_SUITE( ServerSuite )
BOOST_AUTO_TEST_SUITE( HttpSessionClassSuite )

BOOST_AUTO_TEST_CASE( httpSessionConstructorTest )
{
    // Create context
    boost::asio::io_context context;
    // Create socket
    boost::asio::ip::tcp::socket socket(context);
    // Create app instance
    auto model = std::make_shared<Model>("");
    auto instance = std::pair<std::shared_ptr<Controller>, std::shared_ptr<View>>(
        std::make_shared<Controller>(model),
        std::make_shared<View>(
            model,
            std::string(ROOT) + "/web/simple-client"
        )
    );

    // Constructor test
    BOOST_REQUIRE_NO_THROW(
        HttpSession httpSession(
            std::move(socket),
            instance,
            std::chrono::seconds(30),
            context
        )
    );

    // Create another socket ...
    boost::asio::ip::tcp::socket anotherSocket(context);

    // run() method test - should throw exception, as
    // socket is not connected
    BOOST_CHECK_THROW(
        std::make_shared<HttpSessionTest>(
            std::move(anotherSocket),
            instance,
            std::chrono::seconds(30),
            context
        )->httpSession.run(),
        std::exception
    );
    
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()