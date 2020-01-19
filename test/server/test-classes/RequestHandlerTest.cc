/**
 * @file RequestHandlerTestSuite.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Implementation of RequestHandlerTest class
 * @version 0.1
 * @date 2020-01-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "RequestHandlerTest.h"

RequestHandlerTest::RequestHandlerTest(Server& server, const std::string& client_id) :
    requestHandler(server, client_id)
{}

boost::beast::string_view RequestHandlerTest::__mimeType(const boost::beast::string_view& path){
    return requestHandler.__mimeType(path);
}

std::string RequestHandlerTest::__pathCat(const boost::beast::string_view& base, const boost::beast::string_view& path){
    return requestHandler.__pathCat(base, path);
}