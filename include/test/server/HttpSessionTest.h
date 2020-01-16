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

/**
 * @brief Class making HttpSession's private members public
 *        to perform unit tests
 * 
 * @see HttpSession.h
 */
class HttpSessionTest
{
// Constructors & Destructors
public:

    HttpSessionTest(Server& server,
                    const std::string& clientID,
                    boost::asio::ip::tcp::socket&& socket);

// Public members
public:

    HttpSession httpSession;

// Interface
public:
    
    void run();
    void __onRead(boost::system::error_code err_code, std::size_t);
    void __onWrite(boost::system::error_code err_code, std::size_t, bool close);
    void __fail(const boost::system::error_code& err_code, char const* what);
    void __closeConnection();

};