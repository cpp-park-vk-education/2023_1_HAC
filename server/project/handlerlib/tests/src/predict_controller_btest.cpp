#include "utils_gtest.h"
#include "http_protocol.h"
#include "dbcontroller.hpp"
#include <jsoncpp/json/value.h>
#include "controllers.h"

using namespace testing;
/*
// Заглушки для классов IHTTPRequest_ и IHTTPResponse_
class MockHTTPRequest : public IHTTPRequest_ {
public:
    MOCK_METHOD(std::string, getRequestURI, (), (const, override));
};

class MockHTTPResponse : public IHTTPResponse_ {
public:
    MOCK_METHOD(void, setStatusCode, (int), (override));
};

// Заглушки для классов DBController и ModelController
class MockDBController : public IDBController {
public:
    MOCK_METHOD(Json::Value, getPlotData, (const std::string&), (override));
};

class MockModelController : public IModelController {
public:
    MOCK_METHOD(IHTTPResponse_, makeModelRequest, (const Json::Value&), (override));
};


TEST(PredictControllerTest, ParseInputHttpRequestTest) {
    PredictController predict_controller(nullptr, nullptr);
    MockHTTPRequest mock_request;

    EXPECT_CALL(mock_request, getRequestURI())
        .WillOnce(Return("/predict?param1=foo&param2=bar"));

    Json::Value result = predict_controller.parseInputHttpRequest(mock_request);

    EXPECT_EQ(result["param1"], "foo");
    EXPECT_EQ(result["param2"], "bar");
}

TEST(PredictControllerTest, GetPlotDataFromDBTest) {
    PredictController predict_controller(nullptr, nullptr);
    MockDBController mock_db_controller;

    EXPECT_CALL(mock_db_controller, getPlotData("some_data_protocol"))
        .WillOnce(Return(Json::Value{1, 2, 3}));

    Json::Value result = predict_controller.getPlotDataFromDB(Json::Value("some_data_protocol"));

    EXPECT_EQ(result, (Json::Value{1, 2, 3}));
}
*/
// TEST(PredictControllerTest, MakeTimeSeriesTest) {
//     PredictController predict_controller(nullptr, nullptr);
//     Json::Value samples_data;
//     size_t window_size = 2;

//     TimeSeriesPredicts& result = predict_controller.makeTimeSeries(samples_data, window_size);

//     EXPECT_EQ(result.size(), samples_data.size() - window_size + 1);
// }

