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

ListenerTest::ListenerTest(Server& server) :
    listener(server)
{}

void ListenerTest::run(){
    return listener.run();
}

void ListenerTest::__on_accept(const boost::system::error_code& err_code){
    return listener.__on_accept(err_code);
}

bool ListenerTest::__fail(const boost::system::error_code& err_code, char const* what){
    return listener.__fail(err_code, what);
}