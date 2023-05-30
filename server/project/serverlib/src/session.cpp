#include "session.h"
#include <sstream>

template <class Body, class Allocator>
http::message_generator
handle_request(
    http::request<Body, http::basic_fields<Allocator>>&& req, IRouterAdapter* router_adapter)
{
    FileLogger& logger = FileLogger::getInstance();
    http::response<http::string_body> res;
    router_adapter->handle(req, res);

    const auto& base = req.base();

    res.set(http::field::content_length, std::to_string(res.body().size()));
    std::string resp_params;
    resp_params = "\nStatus Response: " + std::to_string(res.result_int());
    resp_params += "\nHeader: " + std::to_string(res.result_int());
    resp_params += "\nBody: " + res.body(); 
    logger.log("Response params:" + resp_params);
    return res;
}

Session::Session(
    tcp::socket&& socket,
    IRouterAdapter* router_adapter)
    : stream_(std::move(socket)),
    router_adapter_(router_adapter) {}

void Session::run() {
    net::dispatch(stream_.get_executor(),
                  beast::bind_front_handler(
                      &Session::doRead,
                      shared_from_this()));
}

void Session::doRead() {
    req_ = {};
    stream_.expires_after(std::chrono::seconds(30));

        http::async_read(stream_, buffer_, req_,
            beast::bind_front_handler(
                &Session::onRead,
                shared_from_this()));
    }

void Session::onRead(
    beast::error_code ec,
    std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if(ec == http::error::end_of_stream)
        return doClose();
    
    if (ec){ 
        return logger.log("Error session read:" + ec.message());
    }

    std::cout << "-----------------req start----------" << std::endl; // переделать на лог
    std::cout << req_ << std::endl;
    std::cout << "-----------------req end----------" << std::endl;    
    std::ostringstream oss;
    oss << "Catch request with params" << "\n" << req_;

    logger.log(oss.str());


    sendResponse(
    handle_request(std::move(req_), router_adapter_));
    
}

void Session::sendResponse(http::message_generator&& msg) {
    bool keep_alive = msg.keep_alive();
    beast::async_write(
        stream_,
        std::move(msg),
        beast::bind_front_handler(
            &Session::onWrite, shared_from_this(), keep_alive));
}

void Session::onWrite(
    bool keep_alive,
    beast::error_code ec,
    std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (ec){
        return logger.log("Error session write:" + ec.message());
    }
    if(! keep_alive)
    {
        return doClose();
    }
    doRead();
}

void Session::doClose() {
    beast::error_code ec;
    stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
}
