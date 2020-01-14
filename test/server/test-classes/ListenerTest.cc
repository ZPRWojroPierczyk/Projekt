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
#include "ListenerTest.h"

ListenerTest::ListenerTest(Server& server)
{
    listener = std::make_shared<Server::Listener>(server);
}