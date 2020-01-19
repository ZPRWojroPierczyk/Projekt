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

HttpSession::HttpSession(Server& server,
                         const std::string& client_id,
                         boost::asio::ip::tcp::socket&& socket) :
    __clientID(client_id),
    __server(server),
    __socket(std::move(socket)),
    __sessionTimeoutTimer(__server.__context),
    __handler(server, client_id)
{}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

void HttpSession::run(){
    
    // Initialize request reading
    boost::beast::http::async_read(
        __socket,
        __buffer,
        __req,
        [self = shared_from_this()]
        (error_code err_code, std::size_t bytes)
        {
            self->__onRead(err_code, bytes);
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

void HttpSession::__onRead(error_code err_code, std::size_t){

    /* --- Check error code --- */
    if(err_code){
        // When peer closed the connection
        if(err_code == boost::beast::http::error::end_of_stream){
            __socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, err_code);
            return;
        } 
        // Connection error
        else{
            __fail(
                err_code,
                (std::string("Read from ") + __socket.remote_endpoint().address().to_string() +
                std::string(" failed!")).c_str()
            );
            return;
        }
    }


    /* --- Create generic lambda to serve response sending --- */
    auto send_handler = 
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
        std::move(send_handler)
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

void HttpSession::__onWrite(error_code err_code, std::size_t, bool close){

    /* --- Check error code --- */
    if(err_code){
        __fail(err_code, "Response writting failed!");
        return;
    }

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
            self->__onRead(err_code, bytes);
        }
    );
}

void HttpSession::__fail(const error_code& err_code, char const* what){
    
    // Don't report on canceled operations
    if(err_code == boost::asio::error::operation_aborted)
        return;
    
    // Report if any other error occured
    std::cerr << what << ": " << err_code.message() << "\n";
}

void HttpSession::__closeConnection(){
    error_code err_code;
    __socket.cancel(err_code);
}