#include <iostream>
#include "HTTPServer.hpp"

int main(int argc, char const *argv[]){
    
    HTTPServer server = 
        HTTPServer();

    /* Server configuration */

    if (!server.run()){
        std::cerr << "Wrong server configuration - exit app.";
        return 1;
    }
    
    return 0;
}
