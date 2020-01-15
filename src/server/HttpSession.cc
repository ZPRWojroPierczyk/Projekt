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
#include <boost/bind.hpp>
#include "HttpSession.h"
#include "RequestHandler.hpp"


using error_code = boost::system::error_code;


/*--------------------------------------------------------------------------------*/
/*-------------------------------- Constructors ----------------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Construct a new Http Session:: Http Session object
 * 
 * @param server Reference to the Server object owning session
 * @param clientID Identifier of the client initializing session
 * @param socket Boost:asio::ip::tcp::socket associated with the session
 * 
 * @note HttpSession instance should be created by the shared pointer and run()
 *       method should be called before the end of the creating scope. This
 *       approach to creation delegates session's life-time responsibility
 *       to the object itself, which is desired behaviour.
 */
HttpSession::HttpSession(Server& server,
                         const std::string& clientID,
                         boost::asio::ip::tcp::socket&& socket) :
    __clientID(clientID),
    __server(server),
    __socket(std::move(socket)),
    __sessionTimeoutTimer(__server.__context),
    __handler(__server.__getInstance(clientID))
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
 *       At this time it's __onRead() method's responsibility to prolong
 *       object's lifetime. Method's call decides if object's should be
 *       maintained or deleted.
 * 
 * @see Listener::__on_accept() (Listener.cc)
 */
void HttpSession::run(){
    
    // Initialize request reading
    boost::beast::http::async_read(
        __socket,
        __buffer,
        __req,
        [self = shared_from_this()]
        (error_code errCode, std::size_t bytes)
        {
            self->__onRead(errCode, bytes);
        }
    );

    /* --- Refresh timer --- */
    __sessionTimeoutTimer.expires_after(__server.__sessionTimeout);

    /* -- Set handler to the timeout timer -- */
    __sessionTimeoutTimer.async_wait(
        [this](const boost::system::error_code& ec)
        {
            // If timer was just refreshed return without unregistering client
            if(ec == boost::asio::error::operation_aborted)
                return;
                
            // If timer expired unregister client by leave()
            else
                return this->__closeConnection(); 
        }
    );
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Private member methods ---------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Method called after getting a new request from the client
 * @param errCode : Error code from the async_read()
 */
void HttpSession::__onRead(error_code errCode, std::size_t){

    /* --- Check error code --- */
    if(errCode){
        // When peer closed the connection
        if(errCode == boost::beast::http::error::end_of_stream){
            __socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, errCode);
            return;
        } 
        // Connection error
        else{
            __fail(
                errCode,
                (std::string("Read from ") + __socket.remote_endpoint().address().to_string() +
                std::string(" failed!")).c_str()
            );
            return;
        }
    }


    /* --- Create generic lambda to serve response sending --- */
    auto sendHandler = 
        [this](auto&& response) -> void
        {
            // Deduce message type
            using response_type = typename std::decay<decltype(response)>::type;
            auto sp = std::make_shared<response_type>(std::forward<decltype(response)>(response));

#if BOOST_MSVC
            // NOTE This causes an ICE in gcc 7.3
            // Write the response
            http::async_write(*(this->__socket), *sp,
				[self = shared_from_this(), sp](
					error_code ec, std::size_t bytes)
				{
					self->__onWrite(ec, bytes, sp->need_eof()); 
				});
#else
            // Write the response
            auto self = shared_from_this();
            http::async_write(
                this->__socket,
                *sp,
				[self, sp](
					error_code ec, std::size_t bytes)
				{
					self->__onWrite(ec, bytes, sp->need_eof()); 
				});
#endif
        };


    /* --- Handle response --- */
    __handler(
        std::move(__req),
        std::move(sendHandler)
    );


    /* --- Refresh session timer --- */
    __sessionTimeoutTimer.expires_after(__server.__sessionTimeout);

    /* -- Set handler to the session timeout timer -- */
    __sessionTimeoutTimer.async_wait(
        [this](const boost::system::error_code& ec)
        {
            // If timer was just refreshed return without unregistering client
            if(ec == boost::asio::error::operation_aborted)
                return;
                
            // If timer expired unregister client by leave()
            else
                return this->__closeConnection(); 
        }
    );


    /* --- Refresh client's timer --- */
    __server.__getTimeoutTimer(__clientID)->expires_after(__server.__clientTimeout);

    /* -- Set handler to the client timeout timer -- */
    __server.__getTimeoutTimer(__clientID)->async_wait(
        [this](const boost::system::error_code& ec)
        {
            
            // If timer was just refreshed return without unregistering client
            if(ec == boost::asio::error::operation_aborted)
                return;
                
            // If timer expired unregister client by leave()
            else{
                __server.__leave(__clientID);
                __server.__context.post(
                    [this](){
                        __server.__clean();
                    }
                );
                return;
            } 
        }
    );
}



/**
 * @brief Method called after writing a response to the client
 * 
 * @param errCode :  Error code of the async_write()
 * @param close : True if connection is to be closed
 */
void HttpSession::__onWrite(error_code errCode, std::size_t, bool close){

    /* --- Check error code --- */
    if(errCode){
        __fail(errCode, "Response writting failed!");
        return;
    }

    /* --- Close connection if asked for --- */
    if(close){
        // This means we should close the connection, usually because
        // the response indicated the "Connection: close" semantic.
        __socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, errCode);
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
        (error_code errCode, std::size_t bytes)
        {
            self->__onRead(errCode, bytes);
        }
    );
}



/**
 * @brief Reports a failure
 * 
 * @param : errCode Reported error code
 * @param : what Reason of the failure
 */
void HttpSession::__fail(const error_code& errCode, char const* what){
    
    // Don't report on canceled operations
    if(errCode == boost::asio::error::operation_aborted)
        return;
    
    // Report if any other error occured
    std::cerr << what << ": " << errCode.message() << "\n";
}



/**
 * @brief Closes the socket ending tcp connetcion.
 */
void HttpSession::__closeConnection(){
    error_code errCode;
    __socket.cancel(errCode);
}