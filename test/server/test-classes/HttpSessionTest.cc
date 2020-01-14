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

HttpSessionTest::HttpSessionTest(boost::asio::ip::tcp::socket&& socket,
                                 const std::pair<std::shared_ptr<Controller>, std::shared_ptr<View>> & instance,
                                 std::chrono::seconds timeout,
                                 boost::asio::io_context& context):
    httpSession(std::move(socket), instance, timeout, context)
{}