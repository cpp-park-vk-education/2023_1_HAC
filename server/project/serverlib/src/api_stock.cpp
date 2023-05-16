#include "api_stock.h"

api::APIStockRequest::APIStockRequest(){};
Json::Value api::APIStockRequest::getData(const handlers::ProtocolAPI& protocol){
    http::response<http::string_body> res;
        // Check command line arguments.
        auto const host = "finnhub.io" ;
        auto const port = "443";
        auto const target =  "/api/v1/stock/candle?symbol=AAPL&resolution=1&from=1679476980&to=1679649780&token=cgjamq1r01qoenkm782gcgjamq1r01qoenkm7830";
        int version =  11;

        // The io_context is required for all I/O
        boost::asio::io_context ioc;

        // The SSL context is required, and holds certificates
        ssl::context ctx{ssl::context::sslv23_client};

        // This holds the root certificate used for verification
        load_root_certificates(ctx);

        // These objects perform our I/O
        tcp::resolver resolver{ioc};
        ssl::stream<tcp::socket>stream{ioc, ctx};

        // Set SNI Hostname (many hosts need this to handshake successfully)
        if(! SSL_set_tlsext_host_name(stream.native_handle(), host))
        {
            boost::system::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
            throw boost::system::system_error{ec};
        }

        // Look up the domain name
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        boost::asio::connect(stream.next_layer(), results.begin(), results.end());

        // Perform the SSL handshake
        stream.handshake(ssl::stream_base::client);

        // Set up an HTTP GET request message
        http::request<http::string_body> req{http::verb::get, target, version};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // Send the HTTP request to the remote host
        http::write(stream, req);

        // This buffer is used for reading and must be persisted
        boost::beast::flat_buffer buffer;

        // Declare a container to hold the response


        // Receive the HTTP response
        http::read(stream, buffer, res);

        // Write the message to standard out
        std::cout << res.body() << std::endl;

        // Gracefully close the stream
        boost::system::error_code ec;
        stream.shutdown(ec);
        if(ec == boost::asio::error::eof)
        {
            // Rationale:
            // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
            ec.assign(0, ec.category());
        }
        if(ec)
            throw boost::system::system_error{ec};

        // If we get here then the connection is closed gracefully
    Json::Value completeJsonData;
    Json::Reader reader;
    auto epoch_seconds = reader.parse(res.body(), completeJsonData);
    std::cout << completeJsonData["c"].toStyledString();  
};

void api::APIStockRequest::doConnect(const handlers::ProtocolAPI& protocol){
};
IHTTPResponse* api::APIStockRequest::onConnect(ssl::stream<tcp::socket> stream){


};
