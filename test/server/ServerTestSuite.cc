/**
 * @file server_test.cc
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
#include "ServerTest.h"

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
            std::string(ROOT) + "/config/http_server.conf"
        )
    );

    // Construct Server with invalid configuration file path
    BOOST_CHECK_THROW(
        Server(
            ""
        ),
        std::ios_base::failure
    );

    // Construct server with config file containing wrong ip
    BOOST_CHECK_THROW(
        Server(
            std::string(ROOT) + "/test/server/bad-configs/bad_ip.conf"
        ),
        po::invalid_option_value
    );

    // Construct server with config file containing invalid port
    BOOST_CHECK_THROW(
        Server(
            std::string(ROOT) + "/test/server/bad-configs/bad_port.conf"
        ),
        po::invalid_option_value
    );

    // Construct server with config file containing invalid doc root path
    BOOST_CHECK_THROW(
        Server(
            std::string(ROOT) + "/test/server/bad-configs/bad_root.conf"
        ),
        po::invalid_option_value
    );

    // Construct Server with invalid client timeout
    BOOST_CHECK_THROW(
        Server(
            std::string(ROOT) + "/test/server/bad-configs/bad_client_timeout.conf"
        ),
        po::invalid_option_value
    );

    // Construct Server with invalid session timeout
    BOOST_CHECK_THROW(
        Server(
            std::string(ROOT) + "/test/server/bad-configs/bad_session_timeout.conf"
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
    
    ServerTest server_test(
        std::string(ROOT) + "/config/http_server.conf"
    );

    BOOST_CHECK_NO_THROW(server_test.run());
}



/*--------------------------------------------------------------------------------*/
/*---------------------------- Private member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Redundant test for loadConfig() method. Redundancy comes
 *        from serverConstructorTest(), as construcor uses this method
 *        internally.
 */
BOOST_AUTO_TEST_CASE( serverLoadConfigTest )
{
    // Construct server
    ServerTest server_test(
        std::string(ROOT) + "/config/http_server.conf"
    );

    // Load valid config
    BOOST_CHECK_NO_THROW(
        server_test.__loadConfig(
            std::string(ROOT) + "/config/http_server.conf"
        )
    );

    // Load config with invalid configuration file path
    BOOST_CHECK_THROW(
        server_test.__loadConfig(
            ""
        ),
        std::ios_base::failure
    );

    // Load config with config file containing wrong ip
    BOOST_CHECK_THROW(
        server_test.__loadConfig(
            std::string() + "test/server/bad-configs/bad_ip.conf"
        ),
        po::invalid_option_value
    );

    // Load config with config file containing invalid port
    BOOST_CHECK_THROW(
        server_test.__loadConfig(
            std::string() + "test/server/bad-configs/bad_port.conf"
        ),
        po::invalid_option_value
    );

    // Load config with config file containing invalid doc root path
    BOOST_CHECK_THROW(
        server_test.__loadConfig(
            std::string() + "test/server/bad-configs/bad_root.conf"
        ),
        po::invalid_option_value
    );
}



/**
 * @brief New client joins and leaves test-case
 */
BOOST_AUTO_TEST_CASE( serverJoinLeaveTest )
{    

}


/**
 * @brief Test clean() method that is periodically called to clean
 *        inactive clients from the clients table
 */
BOOST_AUTO_TEST_CASE( serverCleanTest )
{

}


BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()