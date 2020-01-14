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
    server(configFile)
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

void ServerTest::__clean(const boost::system::error_code& errCode){
    server.__clean(errCode);
}

ServerTest::clientsMap& ServerTest::getClients(){
    return server.__clients;
}