#include "api_stock.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "utils_gtest_net.h"

using namespace api;
using namespace testing;

TEST(APIModelTests, EmptyPath) {
    EXPECT_NO_THROW(api::APIStockRequest a);
}

class MockHTTPResponse : public IHTTPResponse {
 public:
    MOCK_METHOD(void, setStatus, (int status_code), (override));
    MOCK_METHOD(void, setHeader, (std::string header, std::string value), (override));
    MOCK_METHOD(void, setBody, (std::vector<char> bytes), (override));
};

TEST(APIStockRequestTest, GetDataReturnsResponse) {
    APIStockRequest request;
    std::string path = "/data";
    
    auto mockResponse = std::make_unique<MockHTTPResponse>();
    EXPECT_CALL(*mockResponse, setStatus(_)).Times(1);
    EXPECT_CALL(*mockResponse, setHeader(_, _)).Times(AtLeast(1));
    EXPECT_CALL(*mockResponse, setBody(_)).Times(1);

    IHTTPResponse* response = request.getData(path);
    ASSERT_NE(response, nullptr);

    ON_CALL(*mockResponse, setStatus(_)).WillByDefault(Invoke([=](int status_code) {
        // 
    }));
    ON_CALL(*mockResponse, setHeader(_, _)).WillByDefault(Invoke([=](std::string header, std::string value) {
        // ...
    }));
    ON_CALL(*mockResponse, setBody(_)).WillByDefault(Invoke([=](std::vector<char> bytes) {
        // ...
    }));
    delete response;
}
