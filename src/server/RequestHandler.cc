/**
 * @file RequestHandler.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Implementation of RequestHandler class' methods
 * @version 0.1
 * @date 2020-01-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "RequestHandler.h"

/*--------------------------------------------------------------------------------*/
/*-------------------------------- Constructors ----------------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Construct a new RequestHandler with given pointers to Controller and View modules
 *        of the MVC instance 
 * 
 * @param controller 
 * @param view 
 */
RequestHandler::RequestHandler(std::shared_ptr<Controller> controller, std::shared_ptr<View> view) :
    __controller(controller),
    __view(view)
{}



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
RequestHandler::__mimeType(const boost::beast::string_view& path)
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
std::string RequestHandler::__pathCat(
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