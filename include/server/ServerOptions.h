/**
 * @file ServerOptions.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief ServerOptions class' declaration
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef SERVER_OPTIONS_H
#define SERVER_OPTIONS_H

#include <string>

/**
 * @brief Class represents constant set of options used to configure app's HTTP server 
 */
class ServerOptions
{
public:
    ServerOptions() {};
public:
    
    void loadOptions();

    const std::string getAddress() const {return ipAddress;}
    const unsigned short int getPort() const {return portNumber;}
    const std::string getDocRoot() const {return docRoot;}
private:
    // IP address of the server
    std::string ipAddress;
    // Port number of the server
    unsigned short int portNumber;
    /// Absolute path to the folder containing static files
    std::string docRoot;
};

#endif
