#include "listener.h"
#include "logger.h"
#include <iostream>


Listener::Listener(
        net::io_context& ioc,
        tcp::endpoint endpoint,
        IRouterAdapter* router_adapter)
        : ioc_(ioc)
        , acceptor_(net::make_strand(ioc)),
        router_adapter_(router_adapter)
    {
        FileLogger& logger = FileLogger::getInstance();
        beast::error_code ec;
        acceptor_.open(endpoint.protocol(), ec);

        if (ec) {
            logger.log("Error listener open: " + ec.message());
            return;
        }

        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        if (ec){
            logger.log("Error listener set_option: " + ec.message());
            return;
        }
        acceptor_.bind(endpoint, ec);
        if (ec){
            logger.log("Error listener bind: " + ec.message());
            return;
        }


        acceptor_.listen(
            net::socket_base::max_listen_connections, ec);
        
        if (ec) {
            logger.log("Error listener listen: " + ec.message());
            return;
        }
    }

    void Listener::run()
    {
        doAccept();
    }

    void Listener::doAccept()
    {
        acceptor_.async_accept( 
            net::make_strand(ioc_),
            beast::bind_front_handler(
                &Listener::onAccept,
                shared_from_this()));
    }

    void Listener::onAccept(beast::error_code ec, tcp::socket socket)
    {
        if (ec){
            // Write to log
            return;
        }
            std::make_shared<Session>(
                std::move(socket), router_adapter_)->run();

        doAccept();
    }