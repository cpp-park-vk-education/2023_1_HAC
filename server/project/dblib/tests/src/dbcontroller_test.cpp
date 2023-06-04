#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "data.hpp"
#include "dbcontroller.hpp"

using ::testing::Return;
using ::testing::_;

class DatabaseMock: public database::IDataBase {
public:
    ~DatabaseMock() = default;
    MOCK_METHOD(bool, IsOpen, (),(override));
    MOCK_METHOD(bool, SendQuery, (const std::string& query),(override));
    MOCK_METHOD(Json::Value, GetData, (const std::string& query),(override));
    MOCK_METHOD(Json::Value, GetRow, (const std::string& query),(override));
    MOCK_METHOD(bool, Connect,(), (override));
};

class MemoryDatabaseMock: public database::IMemoryDataBase {
public:
    ~MemoryDatabaseMock() = default;
    MOCK_METHOD(bool, IsOpen, (),(override));
    MOCK_METHOD(bool, Connect,(), (override));
    MOCK_METHOD(bool, Insert,(const std::string& key, const std::string& value, const size_t& ttl), (override));
    MOCK_METHOD(bool, Delete,(const std::string& key), (override));
    MOCK_METHOD(bool, Update,(const std::string& key, const std::string& value, const size_t& ttl), (override));
    MOCK_METHOD(bool, Has,(const std::string& key), (override));
    MOCK_METHOD(std::string, Get, (const std::string& key), (override));

};

class ClientRepositoryMock: public repository::IClientRepository {
public:
    ~ClientRepositoryMock() override = default;
    MOCK_METHOD(bool, Insert, (const std::shared_ptr<ClientData>& data),(override));
    MOCK_METHOD(bool, Delete, (const std::string& key),(override));         
    MOCK_METHOD(bool, Update, (const ClientUpdateType& type, const std::string& key, 
                                const std::shared_ptr<ClientData>& data),(override));         
    MOCK_METHOD(std::shared_ptr<ClientData>, GetByKey, (const ClientGetType& type, const std::string& key),(override));         
};


class TimeSeriesRepositoryMock: public repository::ITimeSeriesRepository {
public:
    ~TimeSeriesRepositoryMock() override = default;
    MOCK_METHOD(bool, Insert, (const std::shared_ptr<TimeSeriesData>& data),(override));
    MOCK_METHOD(std::shared_ptr<TimeSeriesData>, GetByKey, (const std::string& name_stock, const size_t& len_lags, 
                                const std::string& date, const std::string& finish_date),(override));     
    MOCK_METHOD(std::shared_ptr<AllStocks>, GetAllStocks,(), (override));    
    MOCK_METHOD(bool, InsertArray,(const std::shared_ptr<TimeSeriesData>& data), (override));    

};


class SubscriptionRepositoryMock: public repository::ISubscriptionRepository {
public:
    ~SubscriptionRepositoryMock() override = default;     
    MOCK_METHOD(std::shared_ptr<SubscriptionData>, GetByKey, (const std::string& key),(override));   
    MOCK_METHOD(std::shared_ptr<AllSubscription>, GetAll, (),(override));         
};

class TokenRepositoryMock: public repository::ITokenRepository {
public:
    ~TokenRepositoryMock() = default;     
    MOCK_METHOD(bool, Insert, (const TokenData& data),(override));   
    MOCK_METHOD(bool, Delete, (const std::string& key),(override));     
    MOCK_METHOD(std::shared_ptr<TokenData>, Get, (const std::string& key),(override));         
    
};


class DBControllerTest: public ::testing::Test {
public:
    DBControllerTest(): test_client_rep(new ClientRepositoryMock), 
                test_timeseries_rep(new TimeSeriesRepositoryMock), 
                test_sub_rep(new SubscriptionRepositoryMock), 
                db(new DatabaseMock), redis(new MemoryDatabaseMock), 
                test_token_rep(new TokenRepositoryMock), 
                db_controller(new dbcontroller::DataBaseController(db, redis,  test_client_rep, 
                    test_timeseries_rep, test_sub_rep, test_token_rep)) {
    }

protected:
    std::shared_ptr<ClientRepositoryMock> test_client_rep;
    std::shared_ptr<TimeSeriesRepositoryMock> test_timeseries_rep;
    std::shared_ptr<SubscriptionRepositoryMock> test_sub_rep;
    std::shared_ptr<TokenRepositoryMock> test_token_rep;  
    std::shared_ptr<DatabaseMock> db;
    std::shared_ptr<MemoryDatabaseMock> redis;
    std::shared_ptr<dbcontroller::IDataBaseController> db_controller;

};



// Client

TEST_F(DBControllerTest, ClientPost) {
    Json::Value request;
    request["Type"] = POST_REQUEST;
    request["TypeData"] = REGISTRATION;
    request["login"] = "test";
    request["email"] = "test@email.com";
    request["password"] = "pass";
    request["session_id"] = 1;
    request["token"] = "token";

    EXPECT_CALL(*db, IsOpen()).WillRepeatedly(Return(true)); 
    EXPECT_CALL(*test_client_rep, Insert(_)).WillOnce(Return(true)); 

    db_controller->DataRequest(request);
}

TEST_F(DBControllerTest, ClientGet) {
    Json::Value request;
    request["Type"] = POST_REQUEST;
    request["TypeData"] = AUTHORIZATION;
    request["login"] = "test";
    request["email"] = "test@email.com";
    request["password"] = "pass";
    request["session_id"] = 1;
    request["token"] = "token";

    EXPECT_CALL(*db, IsOpen()).WillRepeatedly(Return(true)); 
    EXPECT_CALL(*test_client_rep, GetByKey(_,_));
    db_controller->DataRequest(request);
}

TEST_F(DBControllerTest, ClientEmailUpdate) {
    Json::Value request;
    request["Type"] = POST_REQUEST;
    request["TypeData"] = CHANGE_USER_EMAIL_SETTINGS;
    request["login"] = "test";
    request["email"] = "test@email.com";

    EXPECT_CALL(*db, IsOpen()).WillRepeatedly(Return(true)); 
    EXPECT_CALL(*test_client_rep, Update(_, _, _)); 
    db_controller->DataRequest(request);
}

TEST_F(DBControllerTest, ClientPasswordUpdate) {
    Json::Value request;
    request["Type"] = POST_REQUEST;
    request["TypeData"] = CHANGE_USER_PASSWORD_SETTINGS;
    request["login"] = "test";
    request["password"] = "pass";

    EXPECT_CALL(*db, IsOpen()).WillRepeatedly(Return(true)); 
    EXPECT_CALL(*test_client_rep, Update(UPDATE_PASSWORD, _, _)); 
    db_controller->DataRequest(request);
}


// TimeSeries

TEST_F(DBControllerTest, GetTimeSeries) {
    Json::Value request;
    request["Type"] = GET_REQUEST;
    request["TypeData"] = TIMESERIES_REQUEST;
    request["name_stock"] = "test";
    request["len_lags"] = 1;

    EXPECT_CALL(*db, IsOpen()).WillRepeatedly(Return(true)); 
    EXPECT_CALL(*test_timeseries_rep, GetByKey(_, _, _, _));
    db_controller->DataRequest(request);

}


TEST_F(DBControllerTest, PostTimeSeries) {
    Json::Value request;
    Json::Value param, date;
    
    param[0] = 1;
    date[0] = "2021";
    request["Type"] = POST_REQUEST;
    request["TypeData"] = TIMESERIES_REQUEST;
    request["name_stock"] = "test";
    request["date"] = date;
    request["param"] = param;

    EXPECT_CALL(*db, IsOpen()).WillRepeatedly(Return(true)); 
    EXPECT_CALL(*test_timeseries_rep, Insert(_)); 
    db_controller->DataRequest(request);
}

TEST_F(DBControllerTest, GetStocks) {
    Json::Value request;
    request["Type"] = GET_REQUEST;
    request["TypeData"] = STOCKS_REQUEST;

    EXPECT_CALL(*db, IsOpen()).WillRepeatedly(Return(true)); 
    EXPECT_CALL(*test_timeseries_rep, GetAllStocks());
    db_controller->DataRequest(request);

}

TEST_F(DBControllerTest, InsertArray) {
    Json::Value request;
    Json::Value param, date;
    param[0] = 1;
    date[0] = "2021";   
    request["Type"] = POST_REQUEST;
    request["TypeData"] = TIMESERIES_FILL;
    request["name_stock"] = "test";
    request["date"] = date;
    request["param"] = param;


    EXPECT_CALL(*db, IsOpen()).WillRepeatedly(Return(true)); 
    EXPECT_CALL(*test_timeseries_rep, InsertArray(_));
    db_controller->DataRequest(request);

}

// Token

TEST_F(DBControllerTest, InsertToken) {
    Json::Value request;
    request["Type"] = POST_REQUEST;
    request["TypeData"] = SESSION_REQUEST;
    request["login"] = "login";
    request["token"] = "token";
    request["time_live"] = 10;

    EXPECT_CALL(*redis, IsOpen()).WillRepeatedly(Return(true)); 
    EXPECT_CALL(*test_token_rep, Insert(_));
    db_controller->DataRequest(request);
}

TEST_F(DBControllerTest, DeleteToken) {
    Json::Value request;
    request["Type"] = GET_REQUEST;
    request["TypeData"] = SESSION_DELETE;
    request["token"] = "token";

    EXPECT_CALL(*redis, IsOpen()).WillRepeatedly(Return(true)); 
    EXPECT_CALL(*test_token_rep, Delete(_));
    db_controller->DataRequest(request);
}


TEST_F(DBControllerTest, GetToken) {
    Json::Value request;
    request["Type"] = GET_REQUEST;
    request["TypeData"] = SESSION_REQUEST;
    request["token"] = "token";
    
    EXPECT_CALL(*redis, IsOpen()).WillRepeatedly(Return(true)); 
    EXPECT_CALL(*test_token_rep, Get(_));
    db_controller->DataRequest(request);
}