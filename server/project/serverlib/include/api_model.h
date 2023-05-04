#pragma once //NO_LINT
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
// #include <boost/asio/ip/tcp.hpp>
// #include <boost/asio/ssl/error.hpp>
// #include <boost/asio/ssl/stream.hpp>
#include "http_protocol.h"

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>


namespace api {

class IAPIModelRequest;
using ptrToAPIModel = IAPIModelRequest*;

class IAPIModelRequest {
 public:
    virtual IHTTPResponse getData(IHTTPRequest req) = 0;
};

class APIModelelRequest : public IAPIModelRequest{
 public:
    APIModelelRequest();
    IHTTPResponse getData(IHTTPRequest req) override;
 private:
    void doConnect(std::string path);
    IHTTPResponse onConnect(ssl::stream<tcp::socket> stream);   
};

} // namespace api_model
