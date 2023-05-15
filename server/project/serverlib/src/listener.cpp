#include "listener.h"
#include <iostream>


Listener::Listener(
        net::io_context& ioc,
        tcp::endpoint endpoint,
        std::shared_ptr<std::string const> const& doc_root)
        : ioc_(ioc)
        , acceptor_(net::make_strand(ioc))
        , doc_root_(doc_root)
    {
        beast::error_code ec;

        // Open the acceptor
        acceptor_.open(endpoint.protocol(), ec);

        // Allow address reuse
        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        // Bind to the server address
        acceptor_.bind(endpoint, ec);
        // Start listening for connections
        acceptor_.listen(
            net::socket_base::max_listen_connections, ec);
    }

    // Start accepting incoming connections
    void
    Listener::run()
    {
        do_accept();
    }

    void
    Listener::do_accept()
    {
        // The new connection gets its own strand
        acceptor_.async_accept( 
            net::make_strand(ioc_),
            beast::bind_front_handler(
                &Listener::on_accept,
                shared_from_this()));
    }

    void
    Listener::on_accept(beast::error_code ec, tcp::socket socket)
    {

            // Create the session and run it
            std::make_shared<Session>(
                std::move(socket),
                doc_root_)->run();

        // Accept another connection
        do_accept();
    }