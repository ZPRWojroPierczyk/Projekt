/**
 * @file ServerTest.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Set of tests for Server class
 * @version 0.1
 * @date 2020-01-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <ios>
#include <stdexcept>
#include <boost/program_options.hpp>
#include <boost/test/unit_test.hpp>
#include "Server.h"

namespace po = boost::program_options;

BOOST_AUTO_TEST_SUITE( ServerSuite )
BOOST_AUTO_TEST_SUITE( ServerClassSuite )

/*--------------------------------------------------------------------------------*/
/*------------------------ Constructors and destructors --------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief The only constructor test-case.
 */
BOOST_AUTO_TEST_CASE( serverConstructorTest )
{
    // Construct valid Server
    BOOST_CHECK_NO_THROW(
        Server(
            std::chrono::minutes(3),
            std::string(ROOT) + "/config/http_server.conf"
        )
    );

    // Construct Server with invalid configuration file path
    BOOST_CHECK_THROW(
        Server(
            std::chrono::minutes(3),
            ""
        ),
        std::ios_base::failure
    );

    // Construct Server with invalid timeout
    BOOST_CHECK_THROW(
        Server(
            std::chrono::minutes(-3),
            std::string(ROOT) + "/config/http_server.conf"
        ),
        std::invalid_argument
    );

    // Construct server with config file containing wrong ip
    BOOST_CHECK_THROW(
        Server(
            std::chrono::minutes(3),
            std::string(ROOT) + "/test/server/bad-configs/bad_ip.conf"
        ),
        po::invalid_option_value
    );

    // Construct server with config file containing invalid port
    BOOST_CHECK_THROW(
        Server(
            std::chrono::minutes(3),
            std::string(ROOT) + "/test/server/bad-configs/bad_port.conf"
        ),
        po::invalid_option_value
    );

    // Construct server with config file containing invalid doc root path
    BOOST_CHECK_THROW(
        Server(
            std::chrono::minutes(3),
            std::string(ROOT) + "/test/server/bad-configs/bad_root.conf"
        ),
        po::invalid_option_value
    );
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Run server test-case
 */
BOOST_AUTO_TEST_CASE( serverRunTest )
{
    
    Server server(
        std::chrono::minutes(3),
        std::string(ROOT) + "/config/http_server.conf"
    );

    BOOST_CHECK_NO_THROW(server.run());
}

/**
 * @brief New client joins and leaves test-case
 */
BOOST_AUTO_TEST_CASE( serverJoinLeaveTest )
{    
    // Create server
    Server server(
        std::chrono::minutes(3),
        std::string(ROOT) + "/config/http_server.conf"
    );

    //Create IP address 
    auto address = boost::asio::ip::make_address("0.0.0.0");
    // Create new client 1
    boost::asio::ip::tcp::endpoint client_1(address, 8000);
    // Create new client 2
    boost::asio::ip::tcp::endpoint client_2(address, 8001);

    // Join client - check if client was added
    BOOST_CHECK(server.join(client_1));
    // Join another client - check if client was added
    BOOST_CHECK(server.join(client_2));
    // Try to join client one more time - check if client was NOT added
    BOOST_CHECK(server.join(client_1) == false);

    // Client 1 leaves
    BOOST_CHECK(server.leave(client_1));
    // Client 2 leaves
    BOOST_CHECK(server.leave(client_2));
    // Client 1 leaves one more time
    BOOST_CHECK(server.leave(client_1) == 0);
}


/**
 * @brief Redundant test for loadConfig() method. Redundancy comes
 *        from serverConstructorTest(), as construcor uses this method
 *        internally.
 */
BOOST_AUTO_TEST_CASE( serverLoadConfigTest )
{
    // Construct server
    Server server(
        std::chrono::minutes(3),
        std::string(ROOT) + "/config/http_server.conf"
    );

    // Load valid config
    BOOST_CHECK_NO_THROW(
        server.loadConfig(
            std::string(ROOT) + "/config/http_server.conf"
        )
    );

    // Load config with invalid configuration file path
    BOOST_CHECK_THROW(
        server.loadConfig(
            ""
        ),
        std::ios_base::failure
    );

    // Load config with config file containing wrong ip
    BOOST_CHECK_THROW(
        server.loadConfig(
            std::string() + "test/server/bad-configs/bad_ip.conf"
        ),
        po::invalid_option_value
    );

    // Load config with config file containing invalid port
    BOOST_CHECK_THROW(
        server.loadConfig(
            std::string() + "test/server/bad-configs/bad_port.conf"
        ),
        po::invalid_option_value
    );

    // Load config with config file containing invalid doc root path
    BOOST_CHECK_THROW(
        server.loadConfig(
            std::string() + "test/server/bad-configs/bad_root.conf"
        ),
        po::invalid_option_value
    );
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()