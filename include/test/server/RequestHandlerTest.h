/**
 * @file ServerTest.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Class used to test private interface of RequestHandler class
 * @version 0.1
 * @date 2020-01-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "RequestHandler.h"

class RequestHandlerTest
{
// Constructor & Destructors
public:

    RequestHandlerTest(const std::pair<std::shared_ptr<Controller>, std::shared_ptr<View>>& instance);

// Public members
public:

    RequestHandler requestHandler;

//Interface
public:

    template<class Body, class Allocator, class Send>
    void operator()(
        boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req,
        Send&& send
    );
    boost::beast::string_view __mimeType(const boost::beast::string_view& path);

};