#ifndef SERVER_OPTIONS_H
#define SERVER_OPTIONS_H

#include <string>

/**
 * @brief 
 * 
 */
class ServerOptions
{
public:
    ServerOptions() {};
public:
    void loadOptions();
    std::string getAddress() {return ip_address;}
    int getPort() {return port_number;}
    std::string getDocRoot() {return doc_root;}
private:
    std::string ip_address;
    int port_number;
    std::string doc_root;
};

#endif
