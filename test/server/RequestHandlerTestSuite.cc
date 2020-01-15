/**
 * @file ListenerTest.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Set of tests for RequestHandler class
 * @version 0.1
 * @date 2020-01-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <boost/test/unit_test.hpp>
#include "RequestHandlerTest.h"

namespace ut = boost::unit_test;

BOOST_AUTO_TEST_SUITE( ServerSuite )
BOOST_AUTO_TEST_SUITE( RequestHandlerClassSuite )

BOOST_AUTO_TEST_CASE( requestHandlerConstructorTest )
{
    auto model = std::make_shared<Model>("");
    auto controller = std::make_shared<Controller>(model);
    auto view = std::make_shared<View>(
                    model,
                    std::string(ROOT) + "/web/simple-client"
                );

    // Constructor test
    BOOST_REQUIRE_NO_THROW(
        RequestHandler requestHandlerTest(
            std::pair(controller, view)
        )
    );
}



/**
 * @brief Test method responsible for encoding requested mime type
 */
BOOST_AUTO_TEST_CASE ( requestHandlerMimeTypeTest, * ut::depends_on(
"ServerSuite/RequestHandlerClassSuite/requestHandlerConstructorTest"
))
{
    // Create handler
    auto model = std::make_shared<Model>("");
    auto controller = std::make_shared<Controller>(model);
    auto view = std::make_shared<View>(
                    model,
                    std::string(ROOT) + "/web/simple-client"
                );
    RequestHandlerTest requestHandler(std::pair(controller, view));
  
    // Test different mime types
    BOOST_CHECK( requestHandler.__mimeType("file.htm") == "text/html" );
    BOOST_CHECK( requestHandler.__mimeType("file.html") == "text/html" );
    BOOST_CHECK( requestHandler.__mimeType("file.php") == "text/html" );
    BOOST_CHECK( requestHandler.__mimeType("file.css") == "text/css" );
    BOOST_CHECK( requestHandler.__mimeType("file.txt") == "text/plain" );
    BOOST_CHECK( requestHandler.__mimeType("file.js") == "application/javascript" );
    BOOST_CHECK( requestHandler.__mimeType("file.json") == "application/json" );
    BOOST_CHECK( requestHandler.__mimeType("file.xml") == "application/xml" );
    BOOST_CHECK( requestHandler.__mimeType("file.swf") == "application/x-shockwave-flash" );
    BOOST_CHECK( requestHandler.__mimeType("file.flv") == "video/x-flv" );
    BOOST_CHECK( requestHandler.__mimeType("file.png") == "image/png" );
    BOOST_CHECK( requestHandler.__mimeType("file.jpe") == "image/jpeg" );
    BOOST_CHECK( requestHandler.__mimeType("file.jpeg") == "image/jpeg" );
    BOOST_CHECK( requestHandler.__mimeType("file.jpg") == "image/jpeg" );
    BOOST_CHECK( requestHandler.__mimeType("file.gif")  == "image/gif" );
    BOOST_CHECK( requestHandler.__mimeType("file.bmp")  == "image/bmp" );
    BOOST_CHECK( requestHandler.__mimeType("file.ico")  == "image/vnd.microsoft.icon" );
    BOOST_CHECK( requestHandler.__mimeType("file.tiff") == "image/tiff" );
    BOOST_CHECK( requestHandler.__mimeType("file.tif")  == "image/tiff" );
    BOOST_CHECK( requestHandler.__mimeType("file.svg")  == "image/svg+xml" );
    BOOST_CHECK( requestHandler.__mimeType("file.svgz") == "image/svg+xml" );
}



/**
 * @brief Test method responsible for combining requested resource path with the local
 *        web-content root path
 */
BOOST_AUTO_TEST_CASE ( requestHandlerPathCat, * ut::depends_on(
"ServerSuite/RequestHandlerClassSuite/requestHandlerConstructorTest"
))
{
    // Create handler
    auto model = std::make_shared<Model>("");
    auto controller = std::make_shared<Controller>(model);
    auto view = std::make_shared<View>(
                    model,
                    std::string(ROOT) + "/web/simple-client"
                );
    RequestHandlerTest requestHandler(std::pair(controller, view));

    BOOST_CHECK( requestHandler.__pathCat("/base", "/css/file.css") == "/base/css/file.css" );
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()