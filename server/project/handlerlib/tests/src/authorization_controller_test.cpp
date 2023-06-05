#include "controllers_test.h"

class AuthorizationControllerTest: public ::testing::Test {
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
TEST_F(AuthorizationControllerTest, CheckCorrectPositiveResponse) {
    controllers::AuthorizeController auth_controller(ptr_db_controller);

    Json::Value expected_db_return_first;
    expected_db_return_first[HEADER_JSON_STATUS] = true;
    expected_db_return_first[HEADER_JSON_DB_STATUS_OPEN] = true;
    expected_db_return_first[HEADER_JSON_PASSWORD] = controllers::hashing("password");

    Json::Value expected_db_return_second;
    expected_db_return_second[HEADER_JSON_STATUS] = true;
    expected_db_return_second[HEADER_JSON_DB_STATUS_OPEN] = true;

    ::testing::Sequence seq;

    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::POST_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::AUTHORIZATION;
    db_protocol[HEADER_JSON_LOGIN] = "login";


    EXPECT_CALL(*ptr_db_controller, DataRequest(db_protocol))
        .InSequence(seq)
        .WillOnce(Return(expected_db_return_first));

    EXPECT_CALL(*ptr_db_controller, DataRequest(_))
        .InSequence(seq)
        .WillOnce(Return(expected_db_return_second));
    
    Json::Value request;
    request[HEADER_JSON_LOGIN] = "login";
    request[HEADER_JSON_PASSWORD] = "password";

    EXPECT_NO_THROW(auth_controller.authorization(request));
}


TEST_F(AuthorizationControllerTest, CheckCorrectNegativeResponseOnAlreadySuchUser) {
    controllers::AuthorizeController auth_controller(ptr_db_controller);

    Json::Value expected_db_return;
    expected_db_return[HEADER_JSON_STATUS] = false;
    expected_db_return[HEADER_JSON_DB_STATUS_OPEN] = true;

    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::POST_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::AUTHORIZATION;
    db_protocol[HEADER_JSON_LOGIN] = "login";

    EXPECT_CALL(*ptr_db_controller, DataRequest(db_protocol)).WillOnce(Return(expected_db_return));
    
    Json::Value request;
    request[HEADER_JSON_LOGIN] = "login";
    request[HEADER_JSON_PASSWORD] = "password";


    EXPECT_EQ(auth_controller.authorization(request), expected_db_return);
}


TEST_F(AuthorizationControllerTest, CheckCorrectNegativeResponseOnErrorCreateCookie) {
    controllers::AuthorizeController auth_controller(ptr_db_controller);

    Json::Value expected_db_return_first;
    expected_db_return_first[HEADER_JSON_STATUS] = true;
    expected_db_return_first[HEADER_JSON_DB_STATUS_OPEN] = true;
    expected_db_return_first[HEADER_JSON_PASSWORD] = controllers::hashing("password");

    Json::Value expected_db_return_second;
    expected_db_return_second[HEADER_JSON_STATUS] = false;
    expected_db_return_second[HEADER_JSON_DB_STATUS_OPEN] = false;

    ::testing::Sequence seq;

    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::POST_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::AUTHORIZATION;
    db_protocol[HEADER_JSON_LOGIN] = "login";
    

    EXPECT_CALL(*ptr_db_controller, DataRequest(db_protocol))
        .InSequence(seq)
        .WillOnce(Return(expected_db_return_first));

    EXPECT_CALL(*ptr_db_controller, DataRequest(_))
        .InSequence(seq)
        .WillOnce(Return(expected_db_return_second));;
    
    Json::Value request;
    request[HEADER_JSON_LOGIN] = "login";
    request[HEADER_JSON_PASSWORD] = "password";

    EXPECT_THROW(auth_controller.authorization(request), market_mentor::CreateCookieError);
}
