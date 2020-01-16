/**
 * @file ServerTest.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Class used to test private interface of Listener class
 * @version 0.1
 * @date 2020-01-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <string>
#include "Listener.h"

class ListenerTest
{
// Constructor & Destructors
public:

    ListenerTest(Server& server);

// Public members
public:

    Server::Listener listener;

// Interface
public:

    void run();
    void __on_accept(const boost::system::error_code& err_code);
    bool __fail(const boost::system::error_code& err_code, char const* what);

};