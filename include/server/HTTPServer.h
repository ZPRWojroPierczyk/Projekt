#ifndef HTTPSERVER_H
#define HTTPSERVER_H

/*
* HTTPServer provide fast boost::beast-based interface between
* HTTP client and app-readable format requests represented with
* ClientRequest and ServerResponse class' objects 
* (@see ClientRequest.hpp ServerResponse.hpp)
*/

class HTTPServer{
public:
    bool run();
};

#endif
