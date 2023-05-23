#pragma once // NO_LINT
#include "root_certificates.hpp"
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/reader.h>
#include "http_protocol.h"
#include "handlers.h"
#include "config_handler.h"
#include <map>
#include <cstdlib>
#include <iostream>
#include <string>


using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>


namespace api {

class IAPIStockRequest;
using ptrToAPIStock = IAPIStockRequest*;


class IAPIStockRequest {
 public:
    virtual Json::Value getData(const handlers::ProtocolAPI& protocol) = 0; 
};

class APIStockRequest : public IAPIStockRequest{
 public:
    APIStockRequest();
    Json::Value getData(const handlers::ProtocolAPI& protocol) override;
 private:
    void parseApiProtocol(const handlers::ProtocolAPI& protocol);
    void doConnect(const handlers::ProtocolAPI& protocol);
    IHTTPResponse* onConnect(ssl::stream<tcp::socket> stream);
    std::string host;
    std::string port = "443";
    std::string target;
};

} // namespace api_stock 

