#pragma once

#include "root_certificates.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <jsoncpp/json/json.h>
#include <cstdlib>
#include <iostream>
#include <string>

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

using httpRequest = boost::beast::http::request<http::string_body>;
using httpResponse = boost::beast::http::response<http::string_body>;

class IAPIStockRequest{
 public:
    virtual httpResponce getData(std::string path) = 0; 
}

class APIStockRequest : public IAPIStockRequest{
 public:
    APIStockRequest();
    httpResponce getData(std::string path) override;
 private:
    void doConnect(std::string path);
    httpResponce onConnect(ssl::stream<tcp::socket> stream);
}