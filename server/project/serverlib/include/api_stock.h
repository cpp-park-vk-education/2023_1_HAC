#pragma once // NO_LINT
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include "http_protocol.h"
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
    virtual IHTTPResponse* getData(std::string path) = 0; 
};

class APIStockRequest : public IAPIStockRequest{
 public:
    APIStockRequest();
    IHTTPResponse* getData(std::string path) override;
 private:
    void doConnect(std::string path);
    IHTTPResponse* onConnect(ssl::stream<tcp::socket> stream);
};

} // namespace api_stock 

