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
#include "ServerTest.h"

ServerTest::ServerTest(const std::string& configFile) :
    server(configFile),
    __clients(server.__clients),
    __context(server.__context)
{}

void ServerTest::__stop(){
    server.__stop();
}

void ServerTest::__loadConfig(const std::string& configFile){
    server.__loadConfig(configFile);
}

bool ServerTest::__join (const std::string& clientID){
    return server.__join(clientID);
}

bool ServerTest::__leave (const std::string& clientID){
    return server.__leave (clientID);
}

void ServerTest::__clean(){
    server.__clean();
}

const std::pair<std::shared_ptr<Controller>, std::shared_ptr<View>>&
ServerTest::__getInstance(const std::string& clientID){
    return server.__getInstance(clientID);
}

const std::shared_ptr<boost::asio::steady_timer>&
ServerTest::__getTimeoutTimer(const std::string& clientID){
    return server.__getTimeoutTimer(clientID);
}