#include "api_stock.h"

api::APIStockRequest::APIStockRequest(){};
Json::Value api::APIStockRequest::getData(const handlers::ProtocolAPI& protocol){
    http::response<http::string_body> res;

        auto const host = "finnhub.io" ;
        auto const port = "443";
        auto const target = "/api/v1/stock/candle?symbol=AAPL&resolution=1&from=1679476980&to=1679649780&token=cgjamq1r01qoenkm782gcgjamq1r01qoenkm7830";
        
        int version =  11;

        boost::asio::io_context ioc;

    
        ssl::context ctx{ssl::context::sslv23_client};
        load_root_certificates(ctx);

        tcp::resolver resolver{ioc};
        ssl::stream<tcp::socket>stream{ioc, ctx};

        if(! SSL_set_tlsext_host_name(stream.native_handle(), host))
        {
            boost::system::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
            throw boost::system::system_error{ec};
        }

        auto const results = resolver.resolve(host, port);

        boost::asio::connect(stream.next_layer(), results.begin(), results.end());
        stream.handshake(ssl::stream_base::client);

        http::request<http::string_body> req{http::verb::get, target, version};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        http::write(stream, req);

        boost::beast::flat_buffer buffer;


        http::read(stream, buffer, res);

        std::cout << res.body() << std::endl;

        boost::system::error_code ec;
        stream.shutdown(ec);
        if(ec == boost::asio::error::eof)
        {
            
            ec.assign(0, ec.category());
        }
        if(ec)
            throw boost::system::system_error{ec};

    Json::Value completeJsonData;
    Json::Reader reader;
    auto epoch_seconds = reader.parse(res.body(), completeJsonData);
    std::cout << completeJsonData["c"].toStyledString();  
};

void api::APIStockRequest::doConnect(const handlers::ProtocolAPI& protocol){
};
IHTTPResponse* api::APIStockRequest::onConnect(ssl::stream<tcp::socket> stream){


};
