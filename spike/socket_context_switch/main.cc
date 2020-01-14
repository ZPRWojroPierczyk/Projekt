/**
 * A sample code showing the idea of how to switch socket from one io_context to another.
 * What is going on here is: accept sockets on context1, and handle echo logic
 * on a separate context2.
 *
 * compiled with:
 * g++ -O2 -DNDEBUG -DASIO_STANDALONE -I<PATH_TO_ASIO>/include -std=c++14 -pthread -static-libstdc++ main.cpp
 */

#include <iostream>
#include <thread>
#include <memory>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

/**
 * Echo session.
 */
class echo_session_t : public std::enable_shared_from_this< echo_session_t >
{
public:
    echo_session_t( tcp::socket socket)
        :   m_socket{ std::move(socket) }
    {}

    void start()
    {
        do_read();
    }

private:
    void do_read()
    {
        m_socket.async_read_some(
            boost::asio::buffer( m_data.data(), m_data.size() ),
            [self = shared_from_this()]( auto ec, auto len ) {
                if (!ec )
                {
                    std::cout << "[tid:" << std::this_thread::get_id() << "] "
                              << "read some data from "
                              << self->m_socket.remote_endpoint() << std::endl;

                    self->do_write( len );
                }
            } );
    }
    void do_write(std::size_t len )
    {
        boost::asio::async_write( m_socket, boost::asio::buffer( m_data.data(), len ),
            [self = shared_from_this()]( auto ec, auto ) {
                if( !ec )
                {
                    std::cout << "[tid:" << std::this_thread::get_id() << "] "
                              << "sent some data to "
                              << self->m_socket.remote_endpoint() << std::endl;
                    self->do_read();
                }
            } );
    }

    tcp::socket m_socket;
    std::array< char, 1024> m_data;
};

/**
 * Server - accepts connections and starts sessions.
 */
class echo_server_t
{
public:
    echo_server_t( boost::asio::io_context & io_context, boost::asio::io_context & handler_io_context )
        :   m_acceptor{ io_context, tcp::endpoint(tcp::v4(), 8090) }
        ,   m_handler_io_context{ handler_io_context }
        ,   m_acceptor_io_context{ io_context }
    {
        do_accept();
    }

private:
    void do_accept()
    {
        m_acceptor.async_accept(
            m_acceptor_io_context,
            [&]( auto ec, auto socket ){
                if( !ec )
                {
                    std::cout << "[tid:" << std::this_thread::get_id() << "] "
                              << "Receive: connection from "
                              << socket.remote_endpoint() << std::endl;

                    socket.get_executor().context() = m_handler_io_context;

                    std::make_shared< echo_session_t >( std::move( socket ) )->start();
                }
                do_accept();
            } );
    }

    tcp::acceptor m_acceptor;
    boost::asio::io_context & m_acceptor_io_context;
    boost::asio::io_context & m_handler_io_context;
};

int main()
{
    try
    {
        boost::asio::io_context io_context_acceptor;
        boost::asio::io_context io_context_handler;

        echo_server_t server{ io_context_acceptor, io_context_handler };

        std::thread accept_thread{
            [&]{
                std::cout << "[tid:" << std::this_thread::get_id() << "] "
                             "Acceptor ctx runs here" << std::endl;
                io_context_acceptor.run();
            } };

        boost::asio::signal_set break_signals{ io_context_handler, SIGINT };
        break_signals.async_wait(
            [&]( auto ec, int ){
                if( !ec )
                {
                    std::cout << "Stopping..." << std::endl;
                    io_context_acceptor.stop();
                    io_context_handler.stop();
                }
            } );

        std::thread handler_thread{
            [&]{
                std::cout << "[tid:" << std::this_thread::get_id() << "] "
                             "Handler ctx runs here" << std::endl;
                io_context_handler.run(); } };

        handler_thread.join();
        accept_thread.join();
    }
    catch(const std::exception & ex)
    {
        std::cerr << "Error: "<< ex.what();
    }

    return 0;
}