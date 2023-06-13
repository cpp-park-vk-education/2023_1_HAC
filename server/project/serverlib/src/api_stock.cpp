#include "api_stock.h"
#include "server_exception.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>
#include <fstream>

const std::string JSON_RESPONSE_DATA_HEADER = "data";
const std::string JSON_RESPONSE_DATE_HEADER = "date";
const std::string JSON_RESPONSE_NAME_STOCK_HEADER = "name_stock";
const std::string RESPONSE_FROM_STOCK_DATA_HEADER = "c";
const std::string RESPONSE_FROM_STOCK_DATE_HEADER = "t";
const int OFFSET_HOURS = 3;

api::APIStockRequest::APIStockRequest(const std::string& path_to_config_file){
    std::ifstream config_stream (path_to_config_file);
    if (!config_stream) {
        throw market_mentor::InvalidServerConfig("Wrong stock config path");
    }
    std::string host, token;
    config_stream >> host >> token;
    if (!host.length() || !token.length()) {
        throw market_mentor::InvalidServerConfig("Invalid stock config data");
    }  
    host_ = host;
    token_ = token;
};

Json::Value api::APIStockRequest::getData(const handlers::ProtocolAPI& protocol){
    http::response<http::string_body> res;

    parseApiProtocol(protocol);
    int version =  11;
    boost::asio::io_context ioc;

    ssl::context ctx{ssl::context::sslv23_client};
    load_root_certificates(ctx);

    tcp::resolver resolver{ioc};
    ssl::stream<tcp::socket>stream{ioc, ctx};

    try{

        if(!SSL_set_tlsext_host_name(stream.native_handle(), host_.c_str()))
        {
            boost::system::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
            throw boost::system::system_error{ec};
        }

        auto const results = resolver.resolve(host_, port);

        boost::asio::connect(stream.next_layer(), results.begin(), results.end());
        stream.handshake(ssl::stream_base::client);
    } catch(...) {
        throw market_mentor::ConnectionException("Cannot connect to stock");
    }

    http::request<http::string_body> req{http::verb::get, target, version};
    req.set(http::field::host, host_);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    http::write(stream, req);

    try{
        boost::beast::flat_buffer buffer;

        http::read(stream, buffer, res);
        
        boost::system::error_code ec;
        stream.shutdown(ec);
        if(ec == boost::asio::error::eof)
        {     
            ec.assign(0, ec.category());
        }
        Json::Value completeJson_data;
        Json::Reader reader;
        auto epoch_seconds = reader.parse(res.body(), completeJson_data);
        return completeJson_data;
    } catch(...) {
        throw market_mentor::ReedDataException("Error reading data from stock");
    }
};

void api::APIStockRequest::parseApiProtocol(const handlers::ProtocolAPI& protocol){
    target = "/api/v1/stock/candle?symbol=";
    target += protocol.name_packet_stock;
    target += "&resolution=60&from=";
    target += protocol.start_time;
    target += "&to=";
    target += protocol.end_time;
    target += "&token=";
    target += token_;
};

Json::Value api::APIStockRequest::getOneStockPrise(const handlers::ProtocolAPI& protocol){
    Json::Value data_from_srock = getData(protocol);
    Json::Value json_resp;
    try{
        json_resp[JSON_RESPONSE_NAME_STOCK_HEADER] = protocol.name_packet_stock;
        int date = data_from_srock[RESPONSE_FROM_STOCK_DATE_HEADER][0].asInt();
        json_resp[JSON_RESPONSE_DATE_HEADER] = convertIntToDateTime(date);
        json_resp[JSON_RESPONSE_DATA_HEADER] = data_from_srock[RESPONSE_FROM_STOCK_DATA_HEADER][0].asDouble();
    } catch (...) {
        market_mentor::ParseDataException("Error parsing stock request");
    };
    return json_resp;
};

Json::Value api::APIStockRequest::getSeveralStockPrice(const handlers::ProtocolAPI& protocol){
    Json::Value data_from_srock = getData(protocol);
    Json::Value json_resp;
    try{
        json_resp[JSON_RESPONSE_DATA_HEADER] = data_from_srock[RESPONSE_FROM_STOCK_DATA_HEADER];
        int j = 0; 
        for (auto i : data_from_srock[RESPONSE_FROM_STOCK_DATE_HEADER]){
            json_resp[JSON_RESPONSE_DATE_HEADER][j] = convertIntToDateTime(i.asInt());
            ++j;
        }
    } catch (...) {
        market_mentor::ParseDataException("Error parsing stock request");
    };
    return json_resp;
};

std::string api::APIStockRequest::convertIntToDateTime(int unixTime) {
    std::time_t time = static_cast<std::time_t>(unixTime);
    std::chrono::seconds utc_offset(0);
    utc_offset = std::chrono::hours(OFFSET_HOURS);
    time += utc_offset.count();
    std::tm* tm = std::localtime(&time);
    std::ostringstream ss;

    
    ss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");

    return ss.str();
}