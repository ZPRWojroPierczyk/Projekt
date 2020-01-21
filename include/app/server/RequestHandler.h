/**
 * @file Listener.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief RequestHandler class' declaration
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 * @note This code was in large measue base on the Vinnie Falco's
 *       speach given on the CppCon 2018. His examples regarding
 *       boost::beast uasge are available on the git repositery
 *       on https://github.com/vinniefalco/CppCon2018
 */
#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <boost/beast.hpp>
#include "Server.h"
#include "View.h"
#include "Controller.h"

class RequestHandlerTest;

/**
 * @brief Class constituting an interface between the server-side and the MVC instances
 * 
 * RequestHandler is responsible for dealing with all clients' requests.
 * In the first place it parses HTTP messages and interprets it. Basing on the
 * request type (either POST or GET/HEAD) it decides if Model should be modified.
 * 
 * @note Client's action that can influence Model are sent with POST requests
 *       by the local JavaScript scripts.
 * 
 * If so, RequestHandler calls Controller::action() passing actionType, and data associated
 * with an action to it. When the method returns RequestHandler decides whether some data
 * should be sent back to the client basing on the returned value. If so, it calls
 * View::getData() to get JSON-formatted string constituting body for the response.
 * 
 * @note Type of the action is sent in the Content-Type header of the request.
 * 
 * If RequestHandler receives GET/HEAD request, Model does not need to be modifief
 * (as it indicates browser's request for the resources), so it only calls View::getResource()
 * to get a path to the file that should be sent back to the client.
 * 
 * @see Controller.h
 * @see View.h
 * 
 */
class RequestHandler
{
// Constructors & Destructors
public:    

    /**
     * @brief Construct a new RequestHandler with given pointers to Controller and View modules
     *        of the MVC instance 
     * 
     * @param server Reference to the Server object
     * @param client_id 
     */
    RequestHandler(const Server& server, const std::string& client_id);

//Interface
public:
    template<class Body, class Allocator, class Send>
    void operator()(
        boost::beast::http::request<Body, boost::beast::http::basic_fields<Allocator>>&& req,
        Send&& send
    );

// Private Friends
private:

    /// Class used to public private interface for unit tests
    friend class RequestHandlerTest;

// Private members
private:

    /// Reference to the Serverj instance
    const std::string& __docRoot;

    /// Pointer to the Controller instance
    std::shared_ptr<Controller> __controller;

    /// Pointer to the View instance
    std::shared_ptr<View> __view;

// Private member methods
private:

    /**
     * @brief Return a reasonable mime type based on the extension of a file.
     * 
     * @param path : Path to the requested resource
     * @returns Mime type
     */
    boost::beast::string_view __mimeType(const boost::beast::string_view& path);
    
    /**
     * @brief Append an HTTP rel-path to a local filesystem path.
     * 
     * @param base Path to the doc's root folder
     * @param path Relative path to the resource
     * @return Returned path (normalized for the platform.)
     */
    std::string __pathCat(const boost::beast::string_view& base, const boost::beast::string_view& path);

};

#endif
