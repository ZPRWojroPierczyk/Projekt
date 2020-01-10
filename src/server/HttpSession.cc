/**
 * @file HttpSession.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief HttpSession class' methods definitions
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <iostream>
#include <boost/beast/http.hpp>
#include "HttpSession.h"
#include "HttpSession.hpp"


using error_code = boost::system::error_code;


/*--------------------------------------------------------------------------------*/
/*-------------------------------- Constructors ----------------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Construct a new Http Session:: Http Session object
 * 
 * @param socket : boost:asio::ip::tcp::socket associated with the session
 * @param state : State shared between sessions
 * 
 * @note HttpSession instance should be created by the shared pointer and run()
 *       method should be called before the end of the creating scope. This
 *       approach to creation delegates session's life-time responsibility
 *       to the object itself, which is desired behaviour.
 */
HttpSession::HttpSession(boost::asio::ip::tcp::socket&& socket,
                         Server& server) :
    __socket(std::move(socket)),
    __server(server)
{}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Run the session
 * 
 * @note run() method creates another shared_ptr on the HttpSession 
 *       (first was created by Listener::__on_accept()) which, in turn,
 *       is again removed at the end of the scope.
 * 
 *       At this time it's __on_read() method's responsibility to prolong
 *       object's lifetime. Method's call decides if object's should be
 *       maintained or deleted.
 * 
 * @see Listener::__on_accept() (Listener.cc)
 */
void HttpSession::run(){
    // Read a request
    boost::beast::http::async_read(
        __socket,
        __buffer,
        __req,
        [self = shared_from_this()]
        (error_code err_code, std::size_t bytes)
        {
            self->__on_read(err_code, bytes);
        });
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Private member methods ---------------------------*/
/*--------------------------------------------------------------------------------*/


/**
 * @brief Return a reasonable mime type based on the extension of a file.
 * 
 * @param path : Path to the requested resource
 * @returns Mime type
 */
boost::beast::string_view 
HttpSession::__mimeType(const boost::beast::string_view& path)
{
    using boost::beast::iequals;
    // Get extension's name
    auto const ext = [&path]()
    {
        auto const pos = path.rfind(".");
        if(pos == boost::beast::string_view::npos)
            return boost::beast::string_view{};
        return path.substr(pos);
    }();

    // Check for the mime type
    if(iequals(ext, ".htm"))  return "text/html";
    if(iequals(ext, ".html")) return "text/html";
    if(iequals(ext, ".php"))  return "text/html";
    if(iequals(ext, ".css"))  return "text/css";
    if(iequals(ext, ".txt"))  return "text/plain";
    if(iequals(ext, ".js"))   return "application/javascript";
    if(iequals(ext, ".json")) return "application/json";
    if(iequals(ext, ".xml"))  return "application/xml";
    if(iequals(ext, ".swf"))  return "application/x-shockwave-flash";
    if(iequals(ext, ".flv"))  return "video/x-flv";
    if(iequals(ext, ".png"))  return "image/png";
    if(iequals(ext, ".jpe"))  return "image/jpeg";
    if(iequals(ext, ".jpeg")) return "image/jpeg";
    if(iequals(ext, ".jpg"))  return "image/jpeg";
    if(iequals(ext, ".gif"))  return "image/gif";
    if(iequals(ext, ".bmp"))  return "image/bmp";
    if(iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
    if(iequals(ext, ".tiff")) return "image/tiff";
    if(iequals(ext, ".tif"))  return "image/tiff";
    if(iequals(ext, ".svg"))  return "image/svg+xml";
    if(iequals(ext, ".svgz")) return "image/svg+xml";
    return "application/text";
}


/**
 * @brief Append an HTTP rel-path to a local filesystem path.
 * 
 * @param base Path to the doc's root folder
 * @param path Relative path to the resource
 * @return std::string Returned path (normalized for the platform.)
 */
std::string HttpSession::__pathCat(
    const boost::beast::string_view& base,
    const boost::beast::string_view& path)
{
    if(base.empty())
        return path.to_string();
    std::string result = base.to_string();

#if BOOST_MSVC

    char constexpr path_separator = '\\';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    for(auto& c : result)
        if(c == '/')
            c = path_separator;

#else

    char constexpr path_separator = '/';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());

#endif
    return result;
}



/**
 * @brief Method called after getting a new request from the client
 * @param err_code : Error code from the async_read()
 */
void HttpSession::__on_read(error_code err_code, std::size_t){

    /* --- Check error code --- */    
    
    if(err_code){
        // This means they closed the connection
        if(err_code == boost::beast::http::error::end_of_stream){
            __socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, err_code);
            return;
        } // Connection error
        else{
            return __fail(err_code, "read");
        }
    }

        
    /* --- Send response --- */
    
    __handleRequest(
        __server.getDocRoot(),
        std::move(__req),
         // Generic lambda as a response handler (type of the response
         // cannot be predicated apriori)
        [this](auto&& response) -> void
        {
            // Deduce message type
            using response_type = typename std::decay<decltype(response)>::type;
            auto sp = std::make_shared<response_type>(std::forward<decltype(response)>(response));

#if BOOST_MSVC
            // NOTE This causes an ICE in gcc 7.3
            // Write the response
            http::async_write(this->__socket, *sp,
				[self = shared_from_this(), sp](
					error_code ec, std::size_t bytes)
				{
					self->on_write(ec, bytes, sp->need_eof()); 
				});
#else
            // Write the response
            auto self = shared_from_this();
            http::async_write(this->__socket, *sp,
				[self, sp](
					error_code ec, std::size_t bytes)
				{
					self->__on_write(ec, bytes, sp->need_eof()); 
				});
#endif
        });
}



/**
 * @brief Method called after writing a response to the client
 * 
 * @param err_code :  Error code of the async_write()
 * @param close : True if connection is to be closed
 */
void HttpSession::__on_write(error_code err_code, std::size_t, bool close){
    
    /* --- Check error code --- */

    if(err_code)
        return __fail(err_code, "write");

    /* --- Close connection if asked for --- */

    if(close){
        // This means we should close the connection, usually because
        // the response indicated the "Connection: close" semantic.
        __socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, err_code);
        return;
    }

    // Clear contents of the request message,
    // otherwise the read behavior is undefined.
    __req = {};

    // Read another request
    boost::beast::http::async_read(
        __socket,
        __buffer,
        __req,
        [self = shared_from_this()]
        (error_code err_code, std::size_t bytes)
        {
            self->__on_read(err_code, bytes);
        });
}



/**
 * @brief Reports a failure
 * 
 * @param : err_code Reported error code
 * @param : what Reason of the failure
 */
void HttpSession::__fail(const error_code& err_code, char const* what){
    
    // Don't report on canceled operations
    if(err_code == boost::asio::error::operation_aborted) return;
    
    // Report if any other error occured
    std::cerr << what << ": " << err_code.message() << "\n";
}