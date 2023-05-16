#include "session.h"

template <class Body, class Allocator>
http::message_generator
handle_request(
    http::request<Body, http::basic_fields<Allocator>>&& req, IRouterAdapter* router_adapter)
{
    http::response<http::string_body> res;
    router_adapter->handle(req, res);

    std::unique_ptr<IHTTPRequest> req_ = std::make_unique<HTTPRequestToBoostAdapter>(req);
    std::cerr << "body: "<< req_->getBoby() << std::endl;
    
    const auto& base = req.base();
    //std::cerr << "method: "<< boost::beast::http::to_string(base.method()) << std::endl;
    auto headers = req_->getHeaders();
    for (const auto& pair : headers)
    {
        std::cerr << "headers " << pair.first << " :: " << pair.second << std::endl;
    }


    http::response<http::string_body> ress;
    ress.result(http::status::ok);
    ress.set(http::field::server, "my_server");
    ress.set(http::field::content_type, "text/plain");
    ress.set(http::field::content_length, std::to_string(strlen("Romann")));
    ress.body() = "Romann";
    ress.keep_alive(false);
    return ress;
}

//------------------------------------------------------------------------------



    Session::Session(
        tcp::socket&& socket,
        IRouterAdapter* router_adapter)
        : stream_(std::move(socket)),
        router_adapter_(router_adapter)
    {
    }

    // Start the asynchronous operation
    void
    Session::run()
    {
        // We need to be executing within a strand to perform async operations
        // on the I/O objects in this session. Although not strictly necessary
        // for single-threaded contexts, this example code is written to be
        // thread-safe by default.
        net::dispatch(stream_.get_executor(),
                      beast::bind_front_handler(
                          &Session::do_read,
                          shared_from_this()));
    }

    void
    Session::do_read()
    {
        // Make the request empty before reading,
        // otherwise the operation behavior is undefined.
        req_ = {};

        // Set the timeout.
        stream_.expires_after(std::chrono::seconds(30));

        // Read a request
        http::async_read(stream_, buffer_, req_,
            beast::bind_front_handler(
                &Session::on_read,
                shared_from_this()));
    }

    void
    Session::on_read(
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        // This means they closed the connection
        if(ec == http::error::end_of_stream)
            return do_close();

        // Send the response
        std::cout << "-----------------req start----------" << std::endl;
        std::cout << req_ << std::endl;
        std::cout << "-----------------req end----------" << std::endl;

            
        //std::cout << req_.body()<< std::endl;
        send_response(
        handle_request(std::move(req_), router_adapter_));
        
    }

    void
    Session::send_response(http::message_generator&& msg)
    {
        bool keep_alive = msg.keep_alive();

        //std::cout << msg;
        // Write the response
        beast::async_write(
            stream_,
            std::move(msg),
            beast::bind_front_handler(
                &Session::on_write, shared_from_this(), keep_alive));
    }

    void
    Session::on_write(
        bool keep_alive,
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);


        if(! keep_alive)
        {
            // This means we should close the connection, usually because
            // the response indicated the "Connection: close" semantic.
            return do_close();
        }

        // Read another request
        do_read();
    }

    void
    Session::do_close()
    {
        // Send a TCP shutdown
        beast::error_code ec;
        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);

        // At this point the connection is closed gracefully
    }