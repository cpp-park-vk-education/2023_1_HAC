#pragma once // NO_LINT
#include <iostream>

#include "session.h"
#include "routers.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class Listener : public std::enable_shared_from_this<Listener>
{

public:
    Listener(
        net::io_context& ioc,
        tcp::endpoint endpoint,
        IRouterAdapter* router_adapter
        );
    void run();

private:
    void doAccept();
    void onAccept(beast::error_code ec, tcp::socket socket);
    
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    IRouterAdapter* router_adapter_;
};