#pragma once // NO_LINT
#include <memory>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/config.hpp>
#include "routers.h"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class Session : public std::enable_shared_from_this<Session> {
 public:
    Session() = delete;
    Session(tcp::socket&& socket);

    void setRouterAdapter(IRouterAdapter* router_adapter);
    void run();

 private:
    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    IRouterAdapter* router_adapter;
    std::unique_ptr<IHTTPResponse> response;
    std::unique_ptr<IHTTPRequest> request;

    //void handleRequest(http::request<Body, http::basic_fields<Allocator>>&& req);
    void doRead();
    void onRead( beast::error_code ec,std::size_t bytes_transferred);
    void sendResponce(http::message_generator&& msg);
    void onWrite(bool keep_alive, beast::error_code ec, std::size_t bytes_transferred);
    void doClose();
};