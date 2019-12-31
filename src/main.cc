#include <iostream>
#include <string>
#include <boost/asio/signal_set.hpp>
#include "Listener.h"
#include "SharedState.h"
#include "ServerOptions.h"

/**
 * main() purpose is to load HTTP server's configurationand run
 * it. After that main thread is delegated to server asynchonous
 * operations related to boost::beast actions 
 * (boost::asio::io_context::run())
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char* argv[]){

    // Load options from config file
    ServerOptions options;
    options.loadOptions();

    // Create asio server parameters from loaded options
    auto address = asio::ip::make_address(options.getAddress().c_str());
    auto port = static_cast<unsigned short>(options.getPort());
    auto docRoot = options.getDocRoot();

    // The io_context is required for all I/O
    asio::io_context ioc;

    // Create and launch a listening port
    // NOTE : Shared pointer lives in such cases up to the
    //        end of the scope. It is run() method
    //        respoinsibility to create it's coppy and
    //        prolong Listener lifetime. 
    std::make_shared<Listener>(
        ioc,
        tcp::endpoint{address, port},
        std::make_shared<SharedState>(docRoot)
    )->run();

    // Capture SIGINT and SIGTERM to perform a clean shutdown
    asio::signal_set signals(ioc, SIGINT, SIGTERM);
    signals.async_wait(
        [&ioc](boost::system::error_code const&, int)
        {
            // Stop the io_context. This will cause run()
            // to return immediately, eventually destroying the
            // io_context and any remaining handlers in it.
            ioc.stop();
        }
    );

    // Run the I/O service on the main thread
    ioc.run();

    // Return on SIGINT (ctrl+C) or SIGTERM (kill)
    return EXIT_SUCCESS;
}
