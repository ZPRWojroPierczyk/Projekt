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

class ListenerTest{
// Constructor
public:
    ListenerTest(Server& server);

// Public members
public:
    std::shared_ptr<Server::Listener> listener;
};