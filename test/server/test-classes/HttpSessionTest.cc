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