#include "session.h"
#include <iostream>

Session::Session(tcp::socket&& socket): stream_(std::move(socket)){};

template <class Body, class Allocator>
http::message_generator handleRequest (http::request<Body, http::basic_fields<Allocator>>&& req){
    http::response<http::string_body> res;
    res.result(http::status::ok);
    res.set(http::field::server, "my_server");
    res.set(http::field::content_type, "text/plain");
    res.set(http::field::content_length, std::to_string(strlen("Roma")));
    res.body() = "Roma";
    res.keep_alive(false);
    return res;

};


void Session::setRouterAdapter(IRouterAdapter* router_adapter) {}
void Session::run(){
    std::cerr << "Session on accept";
      net::dispatch(stream_.get_executor(),
                      beast::bind_front_handler(
                          &Session::doRead,
                          shared_from_this()));

};


void Session::doRead(){
    // Make the request empty before reading,
        // otherwise the operation behavior is undefined.
        req_ = {};

        // Set the timeout.
        stream_.expires_after(std::chrono::seconds(30));

        // Read a request
        http::async_read(stream_, buffer_, req_,
            beast::bind_front_handler(
                &Session::onRead,
                shared_from_this()));
};
void Session::onRead(
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        // Send the response
        std::cout << req_ << std::endl;
        //std::cout << req_.body()<< std::endl;
        sendResponce(
        handleRequest(std::move(req_)));
};
void Session::sendResponce(http::message_generator&& msg){

    bool keep_alive = msg.keep_alive();

        //std::cout << msg;
        // Write the response
        beast::async_write(
            stream_,
            std::move(msg),
            beast::bind_front_handler(
                &Session::onWrite, shared_from_this(), keep_alive));

};

void Session::onWrite(bool keep_alive, beast::error_code ec, std::size_t bytes_transferred){
    boost::ignore_unused(bytes_transferred);
     if(! keep_alive)
        {
            // This means we should close the connection, usually because
            // the response indicated the "Connection: close" semantic.
            return doClose();
        }

        // Read another request
        doRead();
}
void Session::doClose(){
      beast::error_code ec;
      stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
};
