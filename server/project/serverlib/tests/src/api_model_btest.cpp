#include "api_model.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "utils_gtest_net.h"

TEST(APIModelRequestTest, GetDataEmptyHTTP) {
    api::APIModelRequest apii;
    HTTPRequestToBoostAdapter* req;

    auto response = apii.getData(req);
    const std::vector<char> empty_http; 
    EXPECT_EQ(response->getBoby(), empty_http);
}