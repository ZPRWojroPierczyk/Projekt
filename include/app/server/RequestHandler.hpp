/**
 * @file RequestHandler.hpp
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief HTTP responses handler
 * @version 0.1
 * @date 2020-01-09
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <sstream>
#include <string>
#include <boost/beast.hpp>
#include "RequestHandler.h"

namespace http = boost::beast::http;

/**
 * @brief Produces an HTTP response for the given request
 * 
 * The type of the response object depends on the
 * contents of the request, so the interface requires the
 * caller to pass a generic lambda for receiving the response.
 * 
 * @tparam Body Is supplied as a template argument to the message class. It controls both
 *         the type of the data member of the resulting message object
 * @tparam Allocator Allocator used for a beast::http::basic_fields
 * @tparam Send Generic lambda type used to send response
 * 
 * @param req HTTP request
 * @param send Generic lambda used to send response
 * 
 * @see RequestHandler.h to get a comprehension about what operator()
 *      is responsible for.
 * 
 */
template<class Body, class Allocator, class Send>
void RequestHandler::operator()(
    http::request<Body, http::basic_fields<Allocator>>&& req,
    Send&& send)
{
    /* --- Prepare lambdas for possible error responses --- */

    // Returns a bad request response
    auto const bad_request =
    [&req](boost::beast::string_view why)
    {
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = why.to_string();
        res.prepare_payload();
        return res;
    };

    // Returns a not found response
    auto const not_found =
    [&req](boost::beast::string_view target)
    {
        http::response<http::string_body> res{http::status::not_found, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "The resource '" + target.to_string() + "' was not found.";
        res.prepare_payload();
        return res;
    };

    // Returns a server error response
    auto const server_error =
    [&req](boost::beast::string_view what)
    {
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + what.to_string() + "'";
        res.prepare_payload();
        return res;
    };

    // Post : client's interaction with the server (e.g. request
    //        for data or model's state update)
    if(req.method() == http::verb::post){
        
        // Lift Content-type from the header
        std::stringstream headers_stream;
        headers_stream << req.base();
        std::string action_type;
        while(std::getline(headers_stream, action_type)){
            if(action_type.find("Content-Type") != std::string::npos){
                // Erease header's name
                auto toErase = std::string("Content-Type: ").length();
                action_type.erase(0, toErase);
                break;
            }
        }

        // Get body to the string
        std::stringstream request_dody_stream;
        request_dody_stream << req.body();
        std::string request_body = request_dody_stream.str();

        // Inform controller about client's action
        std::string requested_data = __controller->action(action_type, request_body);
        
        // Check if client's action requires data to be sent back
        if(requested_data != ""){
            // Get data to send back
            std::string data = __view->getData(requested_data);

            // Construct response body
            http::string_body::value_type response_body(data);
            // Cache the size since we need it after the move
            auto const size = response_body.size();

            // Construct HTTP response
            http::response<http::string_body> res{
                std::piecewise_construct,
                std::make_tuple(std::move(response_body)),
                std::make_tuple(http::status::ok, req.version())
            };

            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "application/json");
            res.content_length(size);
            res.keep_alive(req.keep_alive());

            return send(std::move(res));
        }
        else return;
    }
    // Head/get : request for resources
    else if(req.method() == http::verb::get || req.method() == http::verb::head){

        // Request path must be absolute and not contain "..".
        if( req.target().empty() ||
            req.target()[0] != '/' ||
            req.target().find("..") != boost::beast::string_view::npos)
            return send(bad_request("Illegal request-target"));

        // Get the path to the requested file
        bool is_redirect = false;
        std::string relative_path = __view->getResource(std::string(req.target()), is_redirect);


        /* --- Redirection required --- */

        if(is_redirect){

            http::response<http::empty_body> res{http::status::see_other, req.version()};
            // Set 'Location: ' header to point URI to redirect client to
            res.set(http::field::location, relative_path);
            res.keep_alive(req.keep_alive());

            return send(std::move(res));

        }
        
        
        /* --- Redirection not required --- */

        // Build the absolute path to the requested file
        std::string path = __pathCat(__docRoot, relative_path);
        if(path.back() == '/')
            path.append("index.html");

        // Attempt to open the file
        boost::beast::error_code ec;
        http::file_body::value_type body;
        body.open(path.c_str(), boost::beast::file_mode::scan, ec);

        // Handle the case where the file doesn't exist
        if(ec == boost::system::errc::no_such_file_or_directory)
            return send(not_found(req.target()));

        // Handle an unknown error
        if(ec)
            return send(server_error(ec.message()));

        // Cache the size since we need it after the move
        auto const size = body.size();
        
        // Respond to HEAD request
        if(req.method() == http::verb::head){

            http::response<http::empty_body> res{http::status::ok, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, __mimeType(path));
            res.content_length(size);
            res.keep_alive(req.keep_alive());

            return send(std::move(res));

        }
        // Respond to GET request
        else{
        
            http::response<http::file_body> res{
                std::piecewise_construct,
                std::make_tuple(std::move(body)),
                std::make_tuple(http::status::ok, req.version())};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, __mimeType(path));
            res.content_length(size);
            res.keep_alive(req.keep_alive());

            return send(std::move(res));

        }
    }
    // Another requests are not valid
    else{
        return send(bad_request("Unknown HTTP-method"));
    }
}