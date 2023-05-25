#include "api_stock.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>

api::APIStockRequest::APIStockRequest(){};
Json::Value api::APIStockRequest::getData(const handlers::ProtocolAPI& protocol){
    http::response<http::string_body> res;

    parseApiProtocol(protocol);
    std::cerr << "---------------------" << target << "---------------------"  << std::endl;
    int version =  11;
    boost::asio::io_context ioc;

    ssl::context ctx{ssl::context::sslv23_client};
    load_root_certificates(ctx);

    tcp::resolver resolver{ioc};
    ssl::stream<tcp::socket>stream{ioc, ctx};

    if(! SSL_set_tlsext_host_name(stream.native_handle(), host.c_str()))
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
    Json::Value completeJson_data;
    Json::Reader reader;
    auto epoch_seconds = reader.parse(res.body(), completeJson_data);
    std::cerr << completeJson_data["c"].toStyledString();
    return completeJson_data;
};

void api::APIStockRequest::parseApiProtocol(const handlers::ProtocolAPI& protocol){
    host = protocol.name_stock_hub;
    target = "/api/v1/stock/candle?symbol=";
    target += protocol.name_packet_stock;
    target += "&resolution=60&from=";
    target += protocol.start_time;
    target += "&to=";
    target += protocol.end_time;
    target += "&token=";
    target += protocol.token;
};

Json::Value api::APIStockRequest::getOneStockPrise(const handlers::ProtocolAPI& protocol){
    Json::Value data_from_srock = getData(protocol);
    Json::Value json_resp;
    json_resp["name_stock"] = protocol.name_packet_stock;
    int date = data_from_srock["t"][0].asInt();
    json_resp["date"] = convertIntToDateTime(date);
    json_resp["data"] = data_from_srock["c"][0].asDouble();
    std::cout << "from api--------------------" << json_resp.toStyledString() <<  "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    return json_resp;
};

Json::Value api::APIStockRequest::getSeveralStockPrice(const handlers::ProtocolAPI& protocol){
    Json::Value data_from_srock = getData(protocol);
    Json::Value json_resp;
    json_resp["data"] = data_from_srock["c"]; 
    return json_resp;
};

std::string api::APIStockRequest::convertIntToDateTime(int unixTime) {
    std::time_t time = static_cast<std::time_t>(unixTime);
    std::tm* tm = std::localtime(&time);
    std::ostringstream ss;

    // Установка формата вывода
    ss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");

    return ss.str();
}

void api::APIStockRequest::doConnect(const handlers::ProtocolAPI& protocol){
};
IHTTPResponse* api::APIStockRequest::onConnect(ssl::stream<tcp::socket> stream){
};
