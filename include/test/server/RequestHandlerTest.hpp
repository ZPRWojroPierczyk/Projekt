/**
 * @file RequestHandlerTest.hpp
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Implementation of the template method of the RequestHandlerTest class
 * @version 0.1
 * @date 2020-01-16
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "RequestHandlerTest.h"

template<class Body, class Allocator, class Send>
void RequestHandlerTest::operator()(
    boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req,
    Send&& send)
{
    return requestHandler()<Body, Allocator, Send>(req, send);
}