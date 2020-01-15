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
    __server(configFile)
{}

void ServerTest::__stop(){
    __server.__stop();
}

void ServerTest::__loadConfig(const std::string& configFile){
    __server.__loadConfig(configFile);
}

bool ServerTest::__join (const std::string& clientID){
    return __server.__join(clientID);
}

bool ServerTest::__leave (const std::string& clientID){
    return __server.__leave (clientID);
}

void ServerTest::__clean(){
    __server.__clean();
}

ServerTest::clientsMap& ServerTest::getClients(){
    return __server.__clients;
}

boost::asio::io_context& ServerTest::__getContext(){
    return __server.__getContext();
}