// external dependencies
#include "handlers_test.h"

// Мок класса IPredictController
class MockPredictController : public controllers::IPredictController {
public:
   MOCK_METHOD(Json::Value, makePredict, (const Json::Value& request), (override));
   MOCK_METHOD(controllers::TimeSeriesPredicts, makeTimeSeries, (const std::vector<double>& samples_data, size_t lenpredict), (override));
   MOCK_METHOD(std::vector<double>, parseDBProtocol, (const Json::Value& response), (override));
};


class PredictHandlerTest: public ::testing::Test {
public:
    void SetUp() { 
        ptr_predict_controller = new MockPredictController;
        http_request = new MockHTTPRequest;
        http_response = new MockHTTPResponse;
    }
    void TearDown() { 
        delete ptr_predict_controller;
        delete http_request;
        delete http_response;
    };
protected:
    MockPredictController* ptr_predict_controller;
    MockHTTPRequest* http_request;
    MockHTTPResponse* http_response;
};



// TESTS
TEST_F(PredictHandlerTest, CheckCorrectPositiveResponse) {
    handlers::PredictHandler predict_handler(ptr_predict_controller);

    Json::Value data;
    data["0"] = 1;
    data["1"] = 2;
    data["2"] = 3;
    data["3"] = 4;
    Json::Value expect_return;
    expect_return[HEADER_JSON_STATUS] = true;
    expect_return[HEADER_JSON_DATA] = data;

    EXPECT_CALL(*ptr_predict_controller, makePredict(_)).WillOnce(Return(expect_return));

    EXPECT_CALL(*http_request, getURL()).WillOnce(Return("/?name=test&lenpredict=8"));

    EXPECT_CALL(*http_response, setStatus(OK)).Times(1);
    EXPECT_CALL(*http_response, setHeader(PREDICT_DATA, PREDICT_DATA)).Times(1);
    EXPECT_CALL(*http_response, setBody(data.toStyledString())).Times(1);

    EXPECT_NO_THROW(predict_handler.handle(http_request, http_response));
}

TEST_F(PredictHandlerTest, CheckCorrectNegativeResponseIncorrectNumberTokens) {
    handlers::PredictHandler predict_handler(ptr_predict_controller);

    EXPECT_CALL(*http_request, getURL()).WillOnce(Return("/?name=testlenpredict=8"));
    EXPECT_CALL(*http_response, setStatus(BAD_REQUEST)).Times(1);
    EXPECT_CALL(*http_response, setBody(INVALID_HTTP_PREDICT)).Times(1);

    EXPECT_NO_THROW(predict_handler.handle(http_request, http_response));
}

TEST_F(PredictHandlerTest, CheckCorrectNegativeResponseIncorrectSeparatorTokens) {
    handlers::PredictHandler predict_handler(ptr_predict_controller);
    EXPECT_CALL(*http_request, getURL()).WillOnce(Return("/?nametest&lenpredict=8"));

    EXPECT_CALL(*http_response, setStatus(BAD_REQUEST)).Times(1);
    EXPECT_CALL(*http_response, setBody(INVALID_HTTP_PREDICT)).Times(1);

    EXPECT_NO_THROW(predict_handler.handle(http_request, http_response));
}


TEST_F(PredictHandlerTest, CheckCorrectNegativeResponseIncorrectNameTokens) {
    handlers::PredictHandler predict_handler(ptr_predict_controller);
    EXPECT_CALL(*http_request, getURL()).WillOnce(Return("/?name=test&lags=8"));

    EXPECT_CALL(*http_response, setStatus(BAD_REQUEST)).Times(1);
    EXPECT_CALL(*http_response, setBody(INVALID_HTTP_PREDICT)).Times(1);

    EXPECT_NO_THROW(predict_handler.handle(http_request, http_response));
}


TEST_F(PredictHandlerTest, CheckCorrectJSONPassToMakePredict) {
    handlers::PredictHandler predict_handler(ptr_predict_controller);

    EXPECT_CALL(*http_request, getURL()).WillOnce(Return("/?name=test&lenpredict=8"));

    Json::Value data;
    data["0"] = 1;
    data["1"] = 2;
    data["2"] = 3;
    data["3"] = 4;
    Json::Value expect_return;
    expect_return[HEADER_JSON_STATUS] = true;
    expect_return[HEADER_JSON_DATA] = data;

    Json::Value expected_json_after_parsing;
    expected_json_after_parsing[HEADER_JSON_LEN_LAGS] = 8;
    expected_json_after_parsing[HEADER_JSON_LENPREDICT] = 8;
    expected_json_after_parsing[HEADER_JSON_NAME_STOCK] = "test";

    EXPECT_CALL(*ptr_predict_controller, makePredict(expected_json_after_parsing)).WillOnce(Return(expect_return));
    EXPECT_CALL(*http_response, setStatus(OK)).Times(1);
    EXPECT_CALL(*http_response, setHeader(PREDICT_DATA, PREDICT_DATA)).Times(1);
    EXPECT_CALL(*http_response, setBody(data.toStyledString())).Times(1);
    
    EXPECT_NO_THROW(predict_handler.handle(http_request, http_response));
}
