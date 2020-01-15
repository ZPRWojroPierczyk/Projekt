/**
 * @file ServerTest.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Class used to test private interface of HttpSession class
 * @version 0.1
 * @date 2020-01-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <chrono>
#include <memory>
#include <string>
#include <boost/asio.hpp>

#include "HttpSession.h"

class HttpSessionTest{
// Constructor
public:
    HttpSessionTest(Server& server,
                    const std::string& clientID,
                    boost::asio::ip::tcp::socket&& socket);

// Public members
public:
    HttpSession httpSession;
};