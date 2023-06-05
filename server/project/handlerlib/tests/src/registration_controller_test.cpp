#include "controllers_test.h"

class RegistrationControllerTest: public ::testing::Test {
public:
    void SetUp() { 
        ptr_db_controller = new MockDataBaseController;
    }
    void TearDown() { 
        delete ptr_db_controller;
    };
protected:
        MockDataBaseController* ptr_db_controller;
};

// TESTS
TEST_F(RegistrationControllerTest, CheckCorrectPositiveResponse) {
    controllers::RegisterController register_controller(ptr_db_controller);

    Json::Value expected_db_return_first;
    expected_db_return_first[HEADER_JSON_STATUS] = true;
    expected_db_return_first[HEADER_JSON_DB_STATUS_OPEN] = true;

    Json::Value expected_db_return_second;
    expected_db_return_second[HEADER_JSON_STATUS] = true;
    expected_db_return_second[HEADER_JSON_DB_STATUS_OPEN] = true;

    ::testing::Sequence seq;

    EXPECT_CALL(*ptr_db_controller, DataRequest(_))
        .InSequence(seq)
        .WillOnce(Return(expected_db_return_first));

    EXPECT_CALL(*ptr_db_controller, DataRequest(_))
        .InSequence(seq)
        .WillOnce(Return(expected_db_return_second));
    
    Json::Value request;
    request[HEADER_JSON_LOGIN] = "login";
    request[HEADER_JSON_EMAIL] = "email";
    request[HEADER_JSON_PASSWORD] = "password";

    EXPECT_NO_THROW(register_controller.registration(request));
}


TEST_F(RegistrationControllerTest, CheckCorrectNegativeResponseOnAlreadySuchUser) {
    controllers::RegisterController register_controller(ptr_db_controller);

    Json::Value expected_db_return;
    expected_db_return[HEADER_JSON_STATUS] = false;
    expected_db_return[HEADER_JSON_DB_STATUS_OPEN] = true;

    EXPECT_CALL(*ptr_db_controller, DataRequest(_)).WillOnce(Return(expected_db_return));
    
    Json::Value request;
    request[HEADER_JSON_LOGIN] = "login";
    request[HEADER_JSON_EMAIL] = "email";
    request[HEADER_JSON_PASSWORD] = "password";


    EXPECT_EQ(register_controller.registration(request), expected_db_return);
}


TEST_F(RegistrationControllerTest, CheckCorrectNegativeResponseOnErrorCreateCookie) {
    controllers::RegisterController register_controller(ptr_db_controller);

    Json::Value expected_db_return_first;
    expected_db_return_first[HEADER_JSON_STATUS] = true;
    expected_db_return_first[HEADER_JSON_DB_STATUS_OPEN] = true;

    Json::Value expected_db_return_second;
    expected_db_return_second[HEADER_JSON_STATUS] = false;
    expected_db_return_second[HEADER_JSON_DB_STATUS_OPEN] = false;

    ::testing::Sequence seq;

    EXPECT_CALL(*ptr_db_controller, DataRequest(_))
        .InSequence(seq)
        .WillOnce(Return(expected_db_return_first));

    EXPECT_CALL(*ptr_db_controller, DataRequest(_))
        .InSequence(seq)
        .WillOnce(Return(expected_db_return_second));;
    
    Json::Value request;
    request[HEADER_JSON_LOGIN] = "login";
    request[HEADER_JSON_EMAIL] = "email";
    request[HEADER_JSON_PASSWORD] = "password";

    EXPECT_THROW(register_controller.registration(request), market_mentor::CreateCookieError);
}
