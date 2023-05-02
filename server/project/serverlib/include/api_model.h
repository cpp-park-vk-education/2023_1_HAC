#pragma once

#include <boost/beast/http.hpp>

using httpRequest = boost::beast::http::request<http::string_body>;
using httpResponse = boost::beast::http::response<http::string_body>;

class IAPIModelRequest{
 public:
    virtual httpResponce getData(httpRequest req) = 0;
}

class APIModelelRequest{
 public:
    APIModelelRequest();
    httpResponce getData(httpRequest req) override;
 private:
    void doConnect(std::string path);
    httpResponce onConnect(ssl::stream<tcp::socket> stream);   
}