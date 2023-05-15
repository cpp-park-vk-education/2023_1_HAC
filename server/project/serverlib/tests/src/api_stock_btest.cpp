#include "api_stock.h"
#include "http_protocol.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>

TEST(APIStockRequestTest, GetDataReturnsResponse) {
    api::IAPIStockRequest* api_stock = new api::APIStockRequest();
    auto response = api_stock->getData("/test");
    EXPECT_NE(response, nullptr);
    delete api_stock;
}

TEST(APIStockRequestTest, GetDataEmptyPath) {
    api::IAPIStockRequest* api_stock = new api::APIStockRequest();
    auto response = api_stock->getData("error_path");
    std::vector<char> empty_body; 
    EXPECT_EQ(response->getBoby(), empty_body);
    delete api_stock;
}

TEST(APIStockRequestTest, GetDataErrorPath) {
    api::IAPIStockRequest* api_stock = new api::APIStockRequest();
    auto response = api_stock->getData("error_path");
    std::vector<char> empty_body; 
    EXPECT_EQ(response->getStatus(), 404);
    delete api_stock;
}

TEST(APIStockRequestTest, GetDataCorrectPath) {
    api::IAPIStockRequest* api_stock = new api::APIStockRequest();
    auto response = api_stock->getData("finnhub.io");
    std::vector<char> empty_body; 
    EXPECT_EQ(response->getStatus(), 200);
    delete api_stock;
}

TEST(APIStockRequestTest, GetDataReturnsValidContentType) {
    api::IAPIStockRequest* api_stock = new api::APIStockRequest();
    auto request = api_stock->getData("finnhub.io");
    std::map<std::string, std::string> req; 
    req.insert(std::make_pair("Content-Type", "application/json"));
    EXPECT_EQ(request->getHeaders(), req);
    delete api_stock;
}
