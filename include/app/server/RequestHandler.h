#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <boost/beast.hpp>
#include "View.h"
#include "Controller.h"

class RequestHandlerTest;

class RequestHandler{
// Constructors
public:
    RequestHandler(const std::pair<std::shared_ptr<Controller>, std::shared_ptr<View>>& instance);

//Interface
public:
    template<class Body, class Allocator, class Send>
    void operator()(
        boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req,
        Send&& send
    );

// Private Friends
private:
    friend class RequestHandlerTest;

// Private members
private:
    /// Reference to the Server pbject conatining crucial informations about app
    std::shared_ptr<Controller> __controller;
    std::shared_ptr<View> __view;

// Private member methods
private:
    boost::beast::string_view __mimeType(const boost::beast::string_view& path);
    std::string __pathCat(const boost::beast::string_view& base, const boost::beast::string_view& path);
};

#endif
