// external dependencies
#include "handlers_test.h"

// Мок класса IAuthorizeController
class MockAuthorizationController : public controllers::IAuthorizeController {
public:
   MOCK_METHOD(Json::Value, authorization, (Json::Value& request), (override));
};


class AuthorizeHandlerTest: public ::testing::Test {
public:
    void SetUp() {
        authorization_controller = new MockAuthorizationController;
        http_request = new MockHTTPRequest;
        http_response = new MockHTTPResponse;
    }
    void TearDown() {
        delete authorization_controller;
        delete http_request;
        delete http_response;
    };
protected:
    MockAuthorizationController* authorization_controller;
    MockHTTPRequest* http_request;
    MockHTTPResponse* http_response;
};



// TESTS
TEST_F(AuthorizeHandlerTest, CheckCorrectNegativeResponseInIncorrectBodyIncorrectSize) {
    handlers::AuthorizeHandler authorization_handler(authorization_controller);

    EXPECT_CALL(*http_request, getBody()).WillOnce(Return("loginpassword"));
    EXPECT_CALL(*http_response, setStatus(BAD_REQUEST)).Times(1);
    EXPECT_CALL(*http_response, setBody(INVALID_HTTP_PREDICT_AUTHORIZATION)).Times(1);

    EXPECT_NO_THROW(authorization_handler.handle(http_request, http_response));
}

TEST_F(AuthorizeHandlerTest, CheckCorrectNegativeResponseInIncorrectBodyIncorrectSizeWithSeparator) {
    handlers::AuthorizeHandler authorization_handler(authorization_controller);

    EXPECT_CALL(*http_request, getBody()).WillOnce(Return("password/"));
    EXPECT_CALL(*http_response, setStatus(BAD_REQUEST)).Times(1);
    EXPECT_CALL(*http_response, setBody(INVALID_HTTP_PREDICT_AUTHORIZATION)).Times(1);

    EXPECT_NO_THROW(authorization_handler.handle(http_request, http_response));
}


TEST_F(AuthorizeHandlerTest, CheckCorrectPositiveResponseAndJSONPassLogin) {
    handlers::AuthorizeHandler authorization_handler(authorization_controller);

    EXPECT_CALL(*http_request, getBody()).WillOnce(Return("login/password"));

    Json::Value expected_json_after_parsing;
    expected_json_after_parsing[HEADER_JSON_LOGIN] = "login";
    expected_json_after_parsing[HEADER_JSON_PASSWORD] = "password";

    Json::Value expect_return;
    expect_return[HEADER_JSON_STATUS] = true;
    expect_return[HEADER_JSON_TOKEN] = "testtoken";

    EXPECT_CALL(*authorization_controller, authorization(expected_json_after_parsing)).WillOnce(Return(expect_return));

    EXPECT_CALL(*http_response, setStatus(OK)).Times(1);
    EXPECT_CALL(*http_response, setHeader(COOKIE, "testtoken")).Times(1);

    EXPECT_NO_THROW(authorization_handler.handle(http_request, http_response));
}


TEST_F(AuthorizeHandlerTest, CheckCorrectPositiveResponseAndJSONPassUnauthorized) {
    handlers::AuthorizeHandler authorization_handler(authorization_controller);

    EXPECT_CALL(*http_request, getBody()).WillOnce(Return("login/password"));

    Json::Value expected_json_after_parsing;
    expected_json_after_parsing[HEADER_JSON_LOGIN] = "login";
    expected_json_after_parsing[HEADER_JSON_PASSWORD] = "password";

    Json::Value expect_return;
    expect_return[HEADER_JSON_STATUS] = false;

    EXPECT_CALL(*authorization_controller, authorization(expected_json_after_parsing)).WillOnce(Return(expect_return));
    EXPECT_CALL(*http_response, setStatus(UNAUTHORIZED)).Times(1);
    
    EXPECT_NO_THROW(authorization_handler.handle(http_request, http_response));
}