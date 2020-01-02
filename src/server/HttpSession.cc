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


/**
 * @brief Return a reasonable mime type based on the extension of a file.
 * 
 * @param path Path to the requested resource
 * @return boost::beast::string_view Mime type
 */
boost::beast::string_view 
mimeType(boost::beast::string_view path)
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
std::string
pathCat(
    boost::beast::string_view base,
    boost::beast::string_view path)
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

// This function . 
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
 * @param doc_root Path to the root folder containing static files
 * @param req HTTP request
 * @param send Generic lambda used to send response
 */
template<
    class Body, class Allocator,
    class Send>
void
handleRequest(
    boost::beast::string_view doc_root,
    http::request<Body, http::basic_fields<Allocator>>&& req,
    Send&& send)
{
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

    // Make sure we can handle the method
    if( req.method() != http::verb::get &&
        req.method() != http::verb::head)
        return send(bad_request("Unknown HTTP-method"));

    // Request path must be absolute and not contain "..".
    if( req.target().empty() ||
        req.target()[0] != '/' ||
        req.target().find("..") != boost::beast::string_view::npos)
        return send(bad_request("Illegal request-target"));

    // Build the path to the requested file
    std::string path = pathCat(doc_root, req.target());
    if(req.target().back() == '/')
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
        res.set(http::field::content_type, mimeType(path));
        res.content_length(size);
        res.keep_alive(req.keep_alive());
        return send(std::move(res));
    }

    // Respond to GET request
    http::response<http::file_body> res{
        std::piecewise_construct,
        std::make_tuple(std::move(body)),
        std::make_tuple(http::status::ok, req.version())};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, mimeType(path));
    res.content_length(size);
    res.keep_alive(req.keep_alive());
    return send(std::move(res));
}

//------------------------------------------------------------------------------
//------------------------------- HttpSession methods --------------------------
//------------------------------------------------------------------------------

/**
 * @brief Construct a new Http Session:: Http Session object
 * 
 * @param socket boost:asio::ip::tcp::socket associated with the session
 * @param state State shared between sessions
 */
HttpSession::HttpSession(
    tcp::socket socket,
    std::shared_ptr<SharedState> const& state)
    : socket_(std::move(socket))
    , state_(state)
{
}

/**
 * @brief Run the session
 * 
 */
void HttpSession::run(){
    // Read a request
    http::async_read(socket_, buffer_, req_,
        [self = shared_from_this()]
        (error_code err_code, std::size_t bytes)
        {
            self->on_read(err_code, bytes);
        });
}

/**
 * @brief Report a failure
 * 
 * @param err_code 
 * @param what 
 */
void HttpSession::fail(error_code err_code, char const* what){
    // Don't report on canceled operations
    if(err_code == asio::error::operation_aborted)
        return;

    std::cerr << what << ": " << err_code.message() << "\n";
}

/**
 * @brief Method called after getting a new request from the client
 * 
 * @param err_code Error code of the async_read()
 */
void HttpSession::on_read(error_code err_code, std::size_t){
    // This means they closed the connection
    if(err_code == http::error::end_of_stream){
        socket_.shutdown(tcp::socket::shutdown_send, err_code);
        return;
    }

    // Handle the error, if any
    if(err_code)
        return fail(err_code, "read");

    // Send the response
    handleRequest(state_->doc_root(), std::move(req_),
        [this]
        (auto&& response)
        {
            // The lifetime of the message has to extend
            // for the duration of the async operation so
            // we use a shared_ptr to manage it.
            using response_type = typename std::decay<decltype(response)>::type;
            auto sp = std::make_shared<response_type>(std::forward<decltype(response)>(response));

#if BOOST_MSVC
            // NOTE This causes an ICE in gcc 7.3
            // Write the response
            http::async_write(this->socket_, *sp,
				[self = shared_from_this(), sp](
					error_code ec, std::size_t bytes)
				{
					self->on_write(ec, bytes, sp->need_eof()); 
				});
#else
            // Write the response
            auto self = shared_from_this();
            http::async_write(this->socket_, *sp,
				[self, sp](
					error_code ec, std::size_t bytes)
				{
					self->on_write(ec, bytes, sp->need_eof()); 
				});
#endif
        });
}

/**
 * @brief Method called after writing a response to the client
 * 
 * @param err_code Error code of the async_write()
 * @param close If connection is to be closed
 */
void HttpSession::on_write(error_code err_code, std::size_t, bool close){
    // Handle the error, if any
    if(err_code)
        return fail(err_code, "write");

    if(close){
        // This means we should close the connection, usually because
        // the response indicated the "Connection: close" semantic.
        socket_.shutdown(tcp::socket::shutdown_send, err_code);
        return;
    }

    // Clear contents of the request message,
    // otherwise the read behavior is undefined.
    req_ = {};

    // Read another request
    http::async_read(socket_, buffer_, req_,
        [self = shared_from_this()]
        (error_code err_code, std::size_t bytes)
        {
            self->on_read(err_code, bytes);
        });
}
