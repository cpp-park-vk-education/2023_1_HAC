#include "utils_gtest.h"
#include "http_protocol.h"
#include "dbcontroller.hpp"
#include "controllers.h"
#include "dbcontroller.hpp"
#include "icontrollers.h"

#include <map>
#include <memory>
#include <vector>
#include <gmock/gmock.h>
#include <jsoncpp/json/value.h>


using namespace testing;

class MockDBController : public dbcontroller::IDataBaseController {
 public:
    MOCK_METHOD(Json::Value, DataRequest, (const Json::Value& request), (override));
 private:    
    MOCK_METHOD(bool, TimeSeriesPost, (const std::shared_ptr<TimeSeriesData>& data), (override));
    MOCK_METHOD(std::shared_ptr<TimeSeriesData>, TimeSeriesGet, (const std::string& name_stock, const std::string& len_lags), (override));

    MOCK_METHOD(bool, ClientRequestPost, (const std::shared_ptr<ClientData>& data), (override));
    MOCK_METHOD(std::shared_ptr<ClientData>, ClientRequestGet, (const std::string& key), (override));
    MOCK_METHOD(bool, ClientRequestUpdate,(const std::shared_ptr<ClientData> data), (override));
};

class MockHTTPResponse : public IHTTPResponse {
public:
    MOCK_METHOD(void, setStatusCode, (int status_code), (override));
    MOCK_METHOD(void, setHeaders, (const std::map<std::string, std::string>& headers), (override));
    MOCK_METHOD(void, setBody, (const std::vector<char>& body), (override));
};


class MockHTTPRequest : public IHTTPRequest {
public:
    MOCK_METHOD(std::string, getURL, (), (override));
    MOCK_METHOD(std::map<std::string, std::string>, getHeaders, (), (override));
    MOCK_METHOD(std::vector<char>, getBody, (), (override));
};



class MockModelController : public controllers::IModelController {
 public:
    MOCK_METHOD(IHTTPResponse_, callModelApi, (const controllers::TimeSeriesPredicts&), (override));
 private:
    MOCK_METHOD(IHTTPRequest_, makeHttpForModel,(const controllers::TimeSeriesPredicts& samples_data), (override));
};


TEST(PredictControllerBaseErrorTest, BadInputJsonFromDBError) {
    MockDBController mock_db_controller;

    EXPECT_CALL(mock_db_controller, DataRequest(Json::Value()))
        .WillOnce(Return("bad json data"));

    MockModelController mock_model_controller;

    controllers::PredictController predict_controller(&mock_db_controller, &mock_model_controller);

    MockHTTPRequest mock_request;
    MockHTTPResponse mock_response;

    predict_controller.makePredict(&mock_request, &mock_response);

    EXPECT_EQ(mock_response.getBody(), "Error: bad input JSON from DB");
}


TEST(PredictControllerTest, MakePredictCallsParseInputHttpRequest) {
    MockDBController mock_db_controller;
    MockModelController mock_model_controller;
    controllers::PredictController predict_controller(&mock_db_controller, &mock_model_controller);

    MockHTTPRequest mock_request;
    EXPECT_CALL(mock_request, getBody())
        .WillOnce(Return("test request body"));

    MockHTTPResponse mock_response;
    EXPECT_CALL(mock_response, setBody(_))
        .WillOnce(DoAll(
            WithArg<0>([](const std::string& response_body){
                EXPECT_EQ(response_body, "test response body");
            }),
            Return()
        ));

    EXPECT_CALL(predict_controller, parseInputHttpRequest(_))
        .WillOnce(Return(Json::Value()));

    predict_controller.makePredict(&mock_request, &mock_response);
}

TEST(PredictControllerTest, MakePredictCallsGetPlotDataFromDB) {
    MockDBController mock_db_controller;
    MockModelController mock_model_controller;
    controllers::PredictController predict_controller(&mock_db_controller, &mock_model_controller);

    MockHTTPRequest mock_request;
    EXPECT_CALL(mock_request, getBody())
        .WillOnce(Return("test request body"));

    MockHTTPResponse mock_response;
    EXPECT_CALL(mock_response, setBody(_))
        .WillOnce(DoAll(
            WithArg<0>([](const std::string& response_body){
                EXPECT_EQ(response_body, "test response body");
            }),
            Return()
        ));

    Json::Value expected_query;
    expected_query["data"] = "test request body";
    EXPECT_CALL(mock_db_controller, getPlotData(expected_query))
        .WillOnce(Return(Json::Value()));  // Mock the response

    EXPECT_CALL(predict_controller, getPlotDataFromDB(_))
        .WillOnce(Return(Json::Value()));  // Mock the response

    predict_controller.makePredict(&mock_request, &mock_response);
}





