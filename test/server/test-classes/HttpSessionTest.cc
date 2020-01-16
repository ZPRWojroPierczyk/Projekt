/**
 * @file ServerTest.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Implementation of ServerTest class
 * @version 0.1
 * @date 2020-01-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "HttpSessionTest.h"

HttpSessionTest::HttpSessionTest(Server& server,
                                 const std::string& clientID,
                                 boost::asio::ip::tcp::socket&& socket):
    httpSession(server, clientID, std::move(socket))
{}

void HttpSessionTest::run(){
    return httpSession.run();
}

void HttpSessionTest::__onRead(boost::system::error_code err_code, std::size_t size){
    return httpSession.__onRead(err_code, size);
}

void HttpSessionTest::__onWrite(boost::system::error_code err_code, std::size_t size, bool close){
    return httpSession.__onWrite(err_code, size, close);
}

void HttpSessionTest::__fail(const boost::system::error_code& err_code, char const* what){
    return httpSession.__fail(err_code, what);
}

void HttpSessionTest::__closeConnection(){
    return httpSession.__closeConnection();
}