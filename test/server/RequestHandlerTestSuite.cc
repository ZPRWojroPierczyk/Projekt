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
    //     Server server(
    //         std::string(ROOT) + "/config/http_server.conf"
    //     );

    // auto model = std::make_shared<Model>("");
    // auto controller = std::make_shared<Controller>(model);
    // auto view = std::make_shared<View>(model);

    // // Constructor test
    // BOOST_REQUIRE_NO_THROW(
    //     RequestHandler requestHandlerTest(
    //         server,
    //         std::pair(controller, view)
    //     )
    // );
}



/**
 * @brief Test method responsible for encoding requested mime type
 */
BOOST_AUTO_TEST_CASE ( requestHandlerMimeTypeTest, * ut::depends_on(
"ServerSuite/RequestHandlerClassSuite/requestHandlerConstructorTest"
))
{
    // // Create server
    // Server server(
    //     std::string(ROOT) + "/config/http_server.conf"
    // );

    // // Create handler
    // auto model = std::make_shared<Model>("");
    // auto controller = std::make_shared<Controller>(model);
    // auto view = std::make_shared<View>(model);
    // RequestHandlerTest request_handler(server, std::pair(controller, view));
  
    // // Test different mime types
    // BOOST_CHECK( request_handler.__mimeType("file.htm") == "text/html" );
    // BOOST_CHECK( request_handler.__mimeType("file.html") == "text/html" );
    // BOOST_CHECK( request_handler.__mimeType("file.php") == "text/html" );
    // BOOST_CHECK( request_handler.__mimeType("file.css") == "text/css" );
    // BOOST_CHECK( request_handler.__mimeType("file.txt") == "text/plain" );
    // BOOST_CHECK( request_handler.__mimeType("file.js") == "application/javascript" );
    // BOOST_CHECK( request_handler.__mimeType("file.json") == "application/json" );
    // BOOST_CHECK( request_handler.__mimeType("file.xml") == "application/xml" );
    // BOOST_CHECK( request_handler.__mimeType("file.swf") == "application/x-shockwave-flash" );
    // BOOST_CHECK( request_handler.__mimeType("file.flv") == "video/x-flv" );
    // BOOST_CHECK( request_handler.__mimeType("file.png") == "image/png" );
    // BOOST_CHECK( request_handler.__mimeType("file.jpe") == "image/jpeg" );
    // BOOST_CHECK( request_handler.__mimeType("file.jpeg") == "image/jpeg" );
    // BOOST_CHECK( request_handler.__mimeType("file.jpg") == "image/jpeg" );
    // BOOST_CHECK( request_handler.__mimeType("file.gif")  == "image/gif" );
    // BOOST_CHECK( request_handler.__mimeType("file.bmp")  == "image/bmp" );
    // BOOST_CHECK( request_handler.__mimeType("file.ico")  == "image/vnd.microsoft.icon" );
    // BOOST_CHECK( request_handler.__mimeType("file.tiff") == "image/tiff" );
    // BOOST_CHECK( request_handler.__mimeType("file.tif")  == "image/tiff" );
    // BOOST_CHECK( request_handler.__mimeType("file.svg")  == "image/svg+xml" );
    // BOOST_CHECK( request_handler.__mimeType("file.svgz") == "image/svg+xml" );
}

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()