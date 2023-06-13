#pragma once // NO LINT
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/http.hpp>
#include <boost/config.hpp>
#include "routers.h"
#include "logger.h"
#include "http_protocol.h"
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <thread>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

template <class Body, class Allocator>
http::message_generator
handle_request(
    http::request<Body, http::basic_fields<Allocator>>&& req);


class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(
        tcp::socket&& socket, IRouterAdapter* router_adapter);

    void run();

    void doRead();

    void onRead(
        beast::error_code ec,
        std::size_t bytes_transferred);

    void sendResponse(http::message_generator&& msg);

    void onWrite(
        bool keep_alive,
        beast::error_code ec,
        std::size_t bytes_transferred);

    void doClose();

private:
    IRouterAdapter* router_adapter_;
    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
    FileLogger& logger = FileLogger::getInstance();  
};