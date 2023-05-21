#include "http_protocol.h"
#include "utils_gtest_net.h"
#include "gmock/gmock.h"
#include <stdexcept>
#include <boost/beast/http.hpp>
#include <vector>
/*
TEST(IHTTPResponseTest, CreateObject) {
    EXPECT_NO_THROW(HTTPResponseToBoostAdapter a);
}

TEST(IHTTPResponseTest, SetStatusCode) {
    HTTPResponseToBoostAdapter response;
    response.setStatus(200);
    EXPECT_EQ(response.toBoost().result_int(), 200);
}

TEST(IHTTPResponseTest, SetHeaderEmpty) {
    HTTPResponseToBoostAdapter test_http_resp;
    EXPECT_THROW(test_http_resp.setHeader("", ""), std::runtime_error);
}

TEST(IHTTPResponseTest, SetHeaderTest) {
    HTTPResponseToBoostAdapter response;
    response.setHeader("Content-Type", "text/html");
    EXPECT_EQ(response.toBoost().find(boost::beast::http::field::content_type)->value(), "text/html");
}

TEST(IHTTPResponseTest, SetBodyEmpty) {
    HTTPResponseToBoostAdapter test_http_resp;
    std::vector<char> bytes;
    EXPECT_NO_THROW(test_http_resp.setBody(bytes));
}


TEST(HTTPResponseToBoostAdapterTest, SetBodyTest) {
    HTTPResponseToBoostAdapter response;
    std::vector<char> body = {'h', 'e', 'l', 'l', 'o'};
    response.setBody(body);
    EXPECT_EQ(response.toBoost().body(), "hello");
}

TEST(IHTTPResponseTest, ToBoost) {
    HTTPResponseToBoostAdapter test_http_resp;
    boost::beast::http::response<boost::beast::http::string_body> test_body;
    EXPECT_EQ(test_http_resp.toBoost().result(), test_body.result());
    EXPECT_EQ(test_http_resp.toBoost().find(boost::beast::http::field::content_type)->value(), 
            test_body.find(boost::beast::http::field::content_type)->value());
    EXPECT_EQ(test_http_resp.toBoost().body(), test_body.body());   
}

TEST(HTTPRequestToBoostAdapterTest, GetURLTest) {
    HTTPRequestToBoostAdapter request("http://example.com", "", "");
    EXPECT_EQ(request.getURL(), "http://example.com");
}

TEST(HTTPRequestToBoostAdapterTest, GetHeadersTest) {
    std::string headers_str = "Content-Type: text/html\r\nServer: Apache\r\n";
    HTTPRequestToBoostAdapter request("", headers_str, "");
    std::map<std::string, std::string> expected_headers = {{"Content-Type", "text/html"}, {"Server", "Apache"}};
    EXPECT_EQ(request.getHeaders(), expected_headers);
}

TEST(HTTPRequestToBoostAdapterTest, GetBodyTest) {
    std::vector<char> body = {'h', 'e', 'l', 'l', 'o'};
    HTTPRequestToBoostAdapter request("", "", body);
    EXPECT_EQ(request.getBoby(), body);
}
*/