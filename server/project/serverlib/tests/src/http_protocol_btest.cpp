#include "http_protocol.h"
#include "utils_gtest_net.h"
#include "gmock/gmock.h"
#include <stdexcept>
#include <boost/beast/http.hpp>
#include <vector>

TEST(IHTTPResponseTest, CreateObject) {
    httpResponse response_;
    EXPECT_NO_THROW(HTTPResponseToBoostAdapter a(response_));
}

TEST(IHTTPResponseTest, SetStatusCode) {
    httpResponse response_;
    HTTPResponseToBoostAdapter response(response_);
    response.setStatus(200);
    EXPECT_EQ(response.getStatus(), 200);
}

TEST(IHTTPResponseTest, SetHeaderTest) {
    httpResponse response_;
    HTTPResponseToBoostAdapter response(response_);
    response.setHeader("Content-Type", "text/html");
    EXPECT_EQ(response.getHeaders()["Content-Type"], "text/html");
}

TEST(IHTTPResponseTest, SetBodyEmpty) {
    httpResponse response_;
    HTTPResponseToBoostAdapter test_http_resp(response_);
    std::string bytes;
    EXPECT_NO_THROW(test_http_resp.setBody(bytes));
}

TEST(HTTPResponseToBoostAdapterTest, SetBodyTest) {
    httpResponse response_;
    HTTPResponseToBoostAdapter response(response_);
    std::string body = "foo bar baz";
    response.setBody(body);
    EXPECT_EQ(response.getBody(), "foo bar baz");
}

TEST(HTTPRequestToBoostAdapterTest, GetMethodTest) {
    httpRequest request_;
    request_.method(boost::beast::http::verb::post);
    HTTPRequestToBoostAdapter request(request_);
    std::map<std::string, std::string> expected_headers = { {"actions", "null"}, {"method", "POST"}};
    EXPECT_EQ(request.getHeaders(), expected_headers);
}

TEST(HTTPRequestToBoostAdapterTest, GetURLTest) {
    httpRequest request_;
    request_.target("http://example.com");
    HTTPRequestToBoostAdapter request(request_);
    EXPECT_EQ(request.getURL(), "http://example.com");
}

TEST(HTTPRequestToBoostAdapterTest, GetHeadersTest) {
    httpRequest request_;
    request_.method(boost::beast::http::verb::get);
    request_.insert("Content-Type", "text/html");
    request_.insert("Server", "Apache");
    HTTPRequestToBoostAdapter request(request_);
    std::map<std::string, std::string> expected_headers = {{"Content-Type", "text/html"}, {"Server", "Apache"},  {"actions", "null"}, {"method", "GET"}};
    EXPECT_EQ(request.getHeaders(), expected_headers);
}

TEST(HTTPRequestToBoostAdapterTest, GetBodyTest) {
    httpRequest request_;
    std::string body = "foo bar baz";
    request_.method(boost::beast::http::verb::post);
    request_.body() = body;
    HTTPRequestToBoostAdapter request(request_);
    EXPECT_EQ(request.getBody(), body);
}
