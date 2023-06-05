// external dependencies
#include "handlers_test.h"

// Мок класса IRegisterController
class MockRegistrationController : public controllers::IRegisterController {
public:
   MOCK_METHOD(Json::Value, registration, (Json::Value& request), (override));
};


class RegisterHandlerTest: public ::testing::Test {
public:
    void SetUp() {
        registration_controller = new MockRegistrationController;
        http_request = new MockHTTPRequest;
        http_response = new MockHTTPResponse;
    }
    void TearDown() {
        delete registration_controller;
        delete http_request;
        delete http_response;
    };
protected:
    MockRegistrationController* registration_controller;
    MockHTTPRequest* http_request;
    MockHTTPResponse* http_response;
};



// TESTS
TEST_F(RegisterHandlerTest, CheckCorrectNegativeResponseInIncorrectBodyIncorrectSize) {
    handlers::RegisterHandler registration_handler(registration_controller);

    EXPECT_CALL(*http_request, getBody()).WillOnce(Return("loginpassword/email/"));
    EXPECT_CALL(*http_response, setStatus(BAD_REQUEST)).Times(1);
    EXPECT_CALL(*http_response, setBody(INVALID_HTTP_PREDICT_REGISTRATION)).Times(1);

    EXPECT_NO_THROW(registration_handler.handle(http_request, http_response));
}

TEST_F(RegisterHandlerTest, CheckCorrectNegativeResponseInIncorrectBodyIncorrectSizeWithSeparator) {
    handlers::RegisterHandler registration_handler(registration_controller);

    EXPECT_CALL(*http_request, getBody()).WillOnce(Return("loginpas//sword/email/"));
    EXPECT_CALL(*http_response, setStatus(BAD_REQUEST)).Times(1);
    EXPECT_CALL(*http_response, setBody(INVALID_HTTP_PREDICT_REGISTRATION)).Times(1);

    EXPECT_NO_THROW(registration_handler.handle(http_request, http_response));
}


TEST_F(RegisterHandlerTest, CheckCorrectPositiveResponseAndJSONPassLogin) {
    handlers::RegisterHandler registration_handler(registration_controller);

    EXPECT_CALL(*http_request, getBody()).WillOnce(Return("login/password/email/"));

    Json::Value expected_json_after_parsing;
    expected_json_after_parsing[HEADER_JSON_LOGIN] = "login";
    expected_json_after_parsing[HEADER_JSON_PASSWORD] = "password";
    expected_json_after_parsing[HEADER_JSON_EMAIL] = "email";

    Json::Value expect_return;
    expect_return[HEADER_JSON_STATUS] = true;
    expect_return[HEADER_JSON_TOKEN] = "testtoken";

    EXPECT_CALL(*registration_controller, registration(expected_json_after_parsing)).WillOnce(Return(expect_return));

    EXPECT_CALL(*http_response, setStatus(OK)).Times(1);
    EXPECT_CALL(*http_response, setHeader(COOKIE, "testtoken")).Times(1);

    EXPECT_NO_THROW(registration_handler.handle(http_request, http_response));
}


TEST_F(RegisterHandlerTest, CheckCorrectPositiveResponseAndJSONPassUnauthorized) {
    handlers::RegisterHandler registration_handler(registration_controller);

    EXPECT_CALL(*http_request, getBody()).WillOnce(Return("login/password/email/"));

    Json::Value expected_json_after_parsing;
    expected_json_after_parsing[HEADER_JSON_LOGIN] = "login";
    expected_json_after_parsing[HEADER_JSON_PASSWORD] = "password";
    expected_json_after_parsing[HEADER_JSON_EMAIL] = "email";

    Json::Value expect_return;
    expect_return[HEADER_JSON_STATUS] = false;

    EXPECT_CALL(*registration_controller, registration(expected_json_after_parsing)).WillOnce(Return(expect_return));
    EXPECT_CALL(*http_response, setStatus(UNAUTHORIZED)).Times(1);
    
    EXPECT_NO_THROW(registration_handler.handle(http_request, http_response));
}