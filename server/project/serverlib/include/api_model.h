#pragma once //NO_LINT
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <jsoncpp/json/value.h>
#include <boost/asio.hpp>
#include "http_protocol.h"
#include "icontrollers.h"

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>


namespace api {

class IAPIModelRequest {
 public:
    virtual Json::Value getData(const controllers::TimeSeriesPredicts& samples_data) = 0;
};

using ptrToAPIModel = IAPIModelRequest*;

class APIModelRequest : public IAPIModelRequest{
 public:
    APIModelRequest();
    Json::Value getData(const controllers::TimeSeriesPredicts& samples_data) override;
 private:
    void doConnect(std::string path);
    IHTTPResponse* onConnect(ssl::stream<tcp::socket> stream);
};

} // namespace api_model
