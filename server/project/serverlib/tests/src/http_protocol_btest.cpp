#include "http_protocol.h"
#include "utils_gtest_net.h"
#include "gmock/gmock.h"
#include <stdexcept>
#include <vector>
#include "api_model.h"

TEST(IHTTPResponseTest, CreateObt) {
    EXPECT_NO_THROW(api::APIModelelRequest a);
}


TEST(IHTTPResponseTest, CreateObject) {
    EXPECT_NO_THROW(HTTPResponseToBoostAdapter a);
}

TEST(IHTTPResponseTest, SetTsatusCode) {
    HTTPResponseToBoostAdapter test_http_resp;
    EXPECT_NO_THROW(test_http_resp.setStatus(200));
}

TEST(IHTTPResponseTest, SetHeaderEmpty) {
    HTTPResponseToBoostAdapter test_http_resp;
    EXPECT_THROW(test_http_resp.setHeader("", ""), std::runtime_error);
}

TEST(IHTTPResponseTest, SetBodyEmpty) {
    HTTPResponseToBoostAdapter test_http_resp;
    std::vector<char> bytes;
    EXPECT_NO_THROW(test_http_resp.setBody(bytes));
}