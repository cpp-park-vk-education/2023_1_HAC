#include "api_stock.h"
#include "http_protocol.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>

TEST(APIStockRequestTest, GetDataReturnsResponse) {
    api::APIStockRequest apii;
    auto response = apii.getData("/test");
    EXPECT_NE(response, nullptr);
}

TEST(APIStockRequestTest, GetDataEmptyPath) {
    api::APIStockRequest api;
    auto response = api.getData("error_path");
    std::vector<char> empty_body; 
    EXPECT_EQ(response->getBoby(), empty_body);
}

TEST(APIStockRequestTest, GetDataReturnsValidContentType) {
    api::APIStockRequest apii;
    auto request = apii.getData("/test");
    std::map<std::string, std::string> req; 
    req.insert( std::make_pair("Content-Type", "application/json"));
    EXPECT_EQ(request->getHeaders(), req);
}
