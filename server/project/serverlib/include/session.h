#pragma once // NO LINT
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include "routers.h"
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

//
// The concrete type of the response message (which depends on the
// request), is type-erased in message_generator.
template <class Body, class Allocator>
http::message_generator
handle_request(
    http::request<Body, http::basic_fields<Allocator>>&& req);

//------------------------------------------------------------------------------


// Handles an HTTP server connection
class Session : public std::enable_shared_from_this<Session>
{
public:
    // Take ownership of the stream
    Session(
        tcp::socket&& socket, IRouterAdapter* router_adapter);


    // Start the asynchronous operation
    void
    run();

    void
    do_read();

    void
    on_read(
        beast::error_code ec,
        std::size_t bytes_transferred);

    void
    send_response(http::message_generator&& msg);

    void
    on_write(
        bool keep_alive,
        beast::error_code ec,
        std::size_t bytes_transferred);

    void
    do_close();

private:
    IRouterAdapter* router_adapter_;
    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    http::request<http::string_body> req_;
};