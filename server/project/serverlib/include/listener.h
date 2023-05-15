#pragma once // NO_LINT
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <iostream>

#include "session.h"
#include "routers.h"


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// class IListener {
//  public:
//     virtual void Run() = 0;
// };

// Accepts incoming connections and launches the sessions
class Listener : public std::enable_shared_from_this<Listener>
{

public:
    Listener(
        net::io_context& ioc,
        tcp::endpoint endpoint,
        std::shared_ptr<std::string const> const& doc_root);
    void run();

private:
    void do_accept();
    void on_accept(beast::error_code ec, tcp::socket socket);
    
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    std::shared_ptr<std::string const> doc_root_;
};