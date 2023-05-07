#pragma once // NO_LINT
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

#include "session.h"
#include "routers.h"


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class IListener {
 public:
    virtual void Run() = 0;
};

class Listener : public IListener{
 public:
    Listener() = delete;
    Listener(net::io_context& ioc, tcp::endpoint endpoints, RouterAdapter* router);
    void Run() override;
    
 private:
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    RouterAdapter* router_adapter;

    void doAccept();
    void onAccept();
};