#include "api_model.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "utils_gtest_net.h"

TEST(APIModelTest, CreateApiUnit) {
    api::IAPIModelRequest* api_model = new api::APIModelRequest();
    EXPECT_NE(api_model, nullptr);
    delete api_model;
}
/*
TEST(APIModelTest, GetDataWrongUrl){
    api::IAPIModelRequest* api_model = new api::APIModelRequest();
    IHTTPRequest* req = new HTTPRequestToBoostAdapter();
    req->setUrl("wrong_url");
    auto resp = api_model->getData(req);
    EXPECT_EQ(resp->getStatus(), 404);
    delete api_model;
}

TEST(APIModelTest, GetDataCorrectUrl){
    api::IAPIModelRequest* api_model = new api::APIModelRequest();
    IHTTPRequest* req = new HTTPRequestToBoostAdapter();
    req->setUrl("api_model"); // Сейчас не знаю, какой у неё будет url, поменяю, когда сделаю 
    auto resp = api_model->getData(req);
    EXPECT_EQ(resp->getStatus(), 200);
    delete api_model;
}

TEST(APIModelTest, GetDataEmptyHTTP) {
    api::IAPIModelRequest* api_model = new api::APIModelRequest();
    HTTPRequestToBoostAdapter* req;

    auto response = api_model->getData(req);
    const std::vector<char> empty_http; 
    EXPECT_EQ(response->getBoby(), empty_http);
    delete api_model;
}
*/