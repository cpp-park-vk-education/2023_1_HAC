// external dependencies
#include "utils_gtest.h"
#include "gmock/gmock.h"
#include <jsoncpp/json/json.h>

#include <memory>
// internal dependencies

#include "icontrollers.h"
#include "dbcontroller.hpp"
#include "handler_exception.h"
#include "handlers.h"
#include "controllers.h"

//
#include "api_model.h"
#include "http_protocol.h"



using ::testing::Return;
using ::testing::Throw;
using ::testing::_;


class MockDataBaseController : public dbcontroller::IDataBaseController {
public:
    MOCK_METHOD(Json::Value, DataRequest, (const Json::Value& request), (override));
    MOCK_METHOD(bool, ConnectToDatabase, (), (override));
    MOCK_METHOD(void, SetDatabaseConfig, (const std::string&  addr, const std::string&  port,
        const std::string& db_name, const std::string&  user, const std::string&  pass), (override));

    MOCK_METHOD( Json::Value, TimeSeriesPost, (const Json::Value& data), (override));
    MOCK_METHOD(Json::Value, TimeSeriesGet, (const std::string& name_stock, const std::string& len_lags), (override));

    MOCK_METHOD(Json::Value, ClientRequestPost, (const Json::Value& data), (override));
    MOCK_METHOD(Json::Value, ClientRequestGet, (const std::string& key), (override));
    MOCK_METHOD(Json::Value, ClientRequestUpdate, (const Json::Value& data), (override));
};


// Мок класса IPredictController
class MockPredictController : public controllers::IPredictController {
public:
   MOCK_METHOD(Json::Value, makePredict, (const Json::Value& request), (override));
   MOCK_METHOD(controllers::TimeSeriesPredicts, makeTimeSeries, (const std::vector<double>& samples_data, size_t window_size), (override));
   MOCK_METHOD(std::vector<double>, parseDBProtocol, (const Json::Value& response), (override));
};


class MockHTTPRequest: public IHTTPRequest {
public:
    MOCK_METHOD(void, setStatus, (int status_code), (override));
    MOCK_METHOD(void, setUrl, (const std::string& url), (override));
    MOCK_METHOD(void, setHeader, (const std::string &header, const std::string &value), (override));
    MOCK_METHOD(void, setBody, (const std::string &bytes), (override));
    MOCK_METHOD(std::string, getURL, (), (override));
    MOCK_METHOD((std::map<std::string, std::string>), getHeaders, (), (override));
    MOCK_METHOD(std::string, getBoby, (), (override));
};

class MockHTTPResponse: public IHTTPResponse {
public:
    MOCK_METHOD(void, setStatus, (int status_code), (override));
    MOCK_METHOD(void, setHeader, (const std::string &header, const std::string &value), (override));
    MOCK_METHOD(void, setBody, (const std::string &bytes), (override));
    MOCK_METHOD(std::string, getURL, (), (override));
    MOCK_METHOD(int, getStatus, (), (override));
    MOCK_METHOD((std::map<std::string, std::string>), getHeaders, (), (override));
    MOCK_METHOD(std::string, getBoby, (), (override));
    
};


// // Мок класса IModelController
// class MockModelController : public controllers::IModelController {
// public:
//     MOCK_METHOD(callModelApi, Json::Value(const TimeSeriesPredicts&));
// };

// // Мок класса IShowPlotController
// class MockShowPlotController : public controllers::IShowPlotController {
// public:
//     MOCK_METHOD(createPlotData, Json::Value(const Json::Value&));
// };

// // Мок класса IRegisterController
// class MockRegisterController : public controllers::IRegisterController {
// public:
//     MOCK_METHOD(registration, Json::Value(Json::Value&));
//     MOCK_METHOD(makeDBProtocol, Json::Value(const Json::Value&));
// };

// // Мок класса IAuthorizeController
// class MockAuthorizeController : public controllers::IAuthorizeController {
// public:
//     MOCK_METHOD(authorization, Json::Value(Json::Value&));
//     MOCK_METHOD(makeDBProtocol, Json::Value(const Json::Value&));
//     MOCK_METHOD(checkPassword, Json::Value(const Json::Value&, const Json::Value&));
// };

// // Мок класса IUpdateDataController
// class MockUpdateDataController : public controllers::IUpdateDataController {
// public:
//     MOCK_METHOD(updateData, bool(const handlers::ProtocolAPI&));
//     MOCK_METHOD(makeDBProtocol, Json::Value(const Json::Value&));
// };

class PredictHandlerTest: public ::testing::Test {
public:
    void SetUp() { 
        ptr_db_controller = new MockDataBaseController;
        ptr_predict_controller = new MockPredictController;
        http_request = new MockHTTPRequest;
        http_response = new MockHTTPResponse;
    }
    void TearDown() { 
        delete ptr_db_controller;
        delete ptr_predict_controller;
        delete http_request;
        delete http_response;
    };
protected:
    MockDataBaseController* ptr_db_controller;
    MockPredictController* ptr_predict_controller;
    MockHTTPRequest* http_request;
    MockHTTPResponse* http_response;
};



// TESTS
TEST_F(PredictHandlerTest, BaseTest) {
    handlers::PredictHandler predict_handler(ptr_predict_controller);

    Json::Value data;
    data["0"] = 1;
    data["1"] = 2;
    data["2"] = 3;
    data["3"] = 4;
    Json::Value expect_return;
    expect_return["status"] = true;
    expect_return["data"] = data;

    EXPECT_CALL(*ptr_predict_controller, makePredict(_)).WillOnce(Return(expect_return));

    EXPECT_CALL(*http_request, getURL()).WillOnce(Return("/?name=test&graph=test&lag=8&window_size=8"));

    EXPECT_CALL(*http_response, setStatus(200)).Times(1);
    EXPECT_CALL(*http_response, setHeader("Predict-Data", "Predict-Data")).Times(1);
    EXPECT_CALL(*http_response, setBody(data.toStyledString())).Times(1);

    predict_handler.handle(http_request, http_response);
    EXPECT_EQ(1, 1);
}

