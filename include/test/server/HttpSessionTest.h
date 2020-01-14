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
    HttpSessionTest(boost::asio::ip::tcp::socket&& socket,
                    const std::pair<std::shared_ptr<Controller>, std::shared_ptr<View>> & instance,
                    std::chrono::seconds timeout,
                    boost::asio::io_context& context);

// Public members
public:
    HttpSession httpSession;
};