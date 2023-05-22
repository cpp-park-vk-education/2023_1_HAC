// external dependencies
#include "handlers_test.h"

// Мок класса IShowPlotController
class MockShowPlotController : public controllers::IShowPlotController {
public:
   MOCK_METHOD(Json::Value, createPlotData, (const Json::Value& request), (override));
   MOCK_METHOD(Json::Value, makeDBProtocol, (const Json::Value& request), (override));
};


class ShowPlotHandlerTest: public ::testing::Test {
public:
    void SetUp() { 
        ptr_showplot_controller = new MockShowPlotController;
        http_request = new MockHTTPRequest;
        http_response = new MockHTTPResponse;
    }
    void TearDown() { 
        delete ptr_showplot_controller;
        delete http_request;
        delete http_response;
    };
protected:
    MockShowPlotController* ptr_showplot_controller;
    MockHTTPRequest* http_request;
    MockHTTPResponse* http_response;
};


// TESTS
TEST_F(ShowPlotHandlerTest, CheckCorrectNegativeResponseIncorrectNumberTokens) {
    handlers::ShowPlotHandler showplot_handler(ptr_showplot_controller);

    EXPECT_CALL(*http_request, getURL()).WillOnce(Return("/?name=testlag=8"));
    EXPECT_CALL(*http_response, setStatus(BAD_REQUEST)).Times(1);
    EXPECT_CALL(*http_response, setBody(INVALID_HTTP_PLOT)).Times(1);

    EXPECT_NO_THROW(showplot_handler.handle(http_request, http_response));
}

TEST_F(ShowPlotHandlerTest, CheckCorrectNegativeResponseIncorrectSeparatorTokens) {
    handlers::ShowPlotHandler showplot_handler(ptr_showplot_controller);
    EXPECT_CALL(*http_request, getURL()).WillOnce(Return("/?nametest&lag=8"));

    EXPECT_CALL(*http_response, setStatus(BAD_REQUEST)).Times(1);
    EXPECT_CALL(*http_response, setBody(INVALID_HTTP_PLOT)).Times(1);

    EXPECT_NO_THROW(showplot_handler.handle(http_request, http_response));
}


TEST_F(ShowPlotHandlerTest, CheckCorrectNegativeResponseIncorrectNameTokens) {
    handlers::ShowPlotHandler showplot_handler(ptr_showplot_controller);
    EXPECT_CALL(*http_request, getURL()).WillOnce(Return("/?name=test&afsafasfsa=8"));

    EXPECT_CALL(*http_response, setStatus(BAD_REQUEST)).Times(1);
    EXPECT_CALL(*http_response, setBody(INVALID_HTTP_PLOT)).Times(1);

    EXPECT_NO_THROW(showplot_handler.handle(http_request, http_response));
}


TEST_F(ShowPlotHandlerTest, CheckCorrectPositiveResponseAndCorrectJSONPass) {
    handlers::ShowPlotHandler showplot_handler(ptr_showplot_controller);

    EXPECT_CALL(*http_request, getURL()).WillOnce(Return("/?name=test&lag=8"));

    Json::Value data;
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    data[3] = 4;
    Json::Value expect_return;
    expect_return[HEADER_JSON_STATUS] = true;
    expect_return[HEADER_JSON_DATA] = data;

    Json::Value expected_json_after_parsing;
    expected_json_after_parsing[HEADER_JSON_LEN_LAGS] = 8;
    expected_json_after_parsing[HEADER_JSON_NAME_STOCK] = "test";

    EXPECT_CALL(*ptr_showplot_controller, createPlotData(expected_json_after_parsing)).WillOnce(Return(expect_return));
    EXPECT_CALL(*http_response, setStatus(OK)).Times(1);
    EXPECT_CALL(*http_response, setHeader(PLOT_DATA, PLOT_DATA)).Times(1);
    EXPECT_CALL(*http_response, setBody(data.toStyledString())).Times(1);
    
    EXPECT_NO_THROW(showplot_handler.handle(http_request, http_response));
}

TEST_F(ShowPlotHandlerTest, CheckCorrectNegativeResponseOnNullUrl) {
    handlers::ShowPlotHandler showplot_handler(ptr_showplot_controller);
    EXPECT_CALL(*http_request, getURL()).WillOnce(Return("/?name=test&afsafasfsa=8"));

    EXPECT_CALL(*http_response, setStatus(BAD_REQUEST)).Times(1);
    EXPECT_CALL(*http_response, setBody(INVALID_HTTP_PLOT)).Times(1);

    EXPECT_NO_THROW(showplot_handler.handle(http_request, http_response));
}


TEST_F(ShowPlotHandlerTest, CheckCorrectProcessingNegativeResponseFromControllerServerError) {
    handlers::ShowPlotHandler showplot_handler(ptr_showplot_controller);

    EXPECT_CALL(*http_request, getURL()).WillOnce(Return("/?name=test&lag=8"));

    Json::Value expect_return;
    expect_return[HEADER_JSON_STATUS] = false;
    expect_return[HEADER_JSON_DB_STATUS_OPEN] = false;
    expect_return[HEADER_JSON_ERROR] = "error";

    Json::Value expected_json_after_parsing;
    expected_json_after_parsing[HEADER_JSON_LEN_LAGS] = 8;
    expected_json_after_parsing[HEADER_JSON_NAME_STOCK] = "test";

    EXPECT_CALL(*ptr_showplot_controller, createPlotData(expected_json_after_parsing)).WillOnce(Return(expect_return));
    EXPECT_CALL(*http_response, setStatus(INTERNAL_SERVER_ERROR)).Times(1);
    EXPECT_CALL(*http_response, setHeader(ERROR_MESSAGE, ERROR_MESSAGE)).Times(1);
    EXPECT_CALL(*http_response, setBody("error")).Times(1);
    
    EXPECT_NO_THROW(showplot_handler.handle(http_request, http_response));
}


TEST_F(ShowPlotHandlerTest,  CheckCorrectProcessingNegativeResponseFromControllerNotFound) {
    handlers::ShowPlotHandler showplot_handler(ptr_showplot_controller);

    EXPECT_CALL(*http_request, getURL()).WillOnce(Return("/?name=test&lag=8"));

    Json::Value expect_return;
    expect_return[HEADER_JSON_STATUS] = false;
    expect_return[HEADER_JSON_DB_STATUS_OPEN] = true;
    expect_return[HEADER_JSON_ERROR] = "error";

    Json::Value expected_json_after_parsing;
    expected_json_after_parsing[HEADER_JSON_LEN_LAGS] = 8;
    expected_json_after_parsing[HEADER_JSON_NAME_STOCK] = "test";

    EXPECT_CALL(*ptr_showplot_controller, createPlotData(expected_json_after_parsing)).WillOnce(Return(expect_return));
    EXPECT_CALL(*http_response, setStatus(NOT_FOUND)).Times(1);
    EXPECT_CALL(*http_response, setHeader(ERROR_MESSAGE, ERROR_MESSAGE)).Times(1);
    EXPECT_CALL(*http_response, setBody("error")).Times(1);
    
    EXPECT_NO_THROW(showplot_handler.handle(http_request, http_response));
}

