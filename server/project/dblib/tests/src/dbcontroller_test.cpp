#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "data.hpp"
#include "dbcontroller.hpp"

using ::testing::Return;
using ::testing::_;

using namespace dbcontroller;
using namespace repository;

class DatabaseMock: public IDataBase {
public:
    ~DatabaseMock() = default;
    MOCK_METHOD(bool, IsOpen, (),(override));
    MOCK_METHOD(bool, SendQuery, (const std::string& query),(override));
    MOCK_METHOD(Json::Value, GetData, (const std::string& query),(override));
    MOCK_METHOD(Json::Value, GetRow, (const std::string& query),(override));
    MOCK_METHOD(bool, Connect,(), (override));
};

class ClientRepositoryMock: public IClientRepository {
public:
    ~ClientRepositoryMock() override = default;
    MOCK_METHOD(bool, Insert, (const std::shared_ptr<ClientData>& data),(override));
    MOCK_METHOD(bool, Delete, (const std::string& key),(override));         
    MOCK_METHOD(bool, Update, (const ClientUpdateType& type, const std::string& key, 
                                const std::shared_ptr<ClientData>& data),(override));         
    MOCK_METHOD(std::shared_ptr<ClientData>, GetByKey, (const std::string& key),(override));         
};

class TimeSeriesRepositoryMock: public ITimeSeriesRepository {
public:
    ~TimeSeriesRepositoryMock() override = default;
    MOCK_METHOD(bool, Insert, (const std::shared_ptr<TimeSeriesData>& data),(override));
    MOCK_METHOD(std::shared_ptr<TimeSeriesData>, GetByKey, (const std::string& name_stock, const size_t& len_lags, 
                                const std::string& date, const std::string& finish_date),(override));     
    MOCK_METHOD(std::shared_ptr<AllStocks>, GetAllStocks,(), (override));    
};

class SubscriptionRepositoryMock: public ISubscriptionRepository {
public:
    ~SubscriptionRepositoryMock() override = default;     
    MOCK_METHOD(std::shared_ptr<SubscriptionData>, GetByKey, (const std::string& key),(override));   
    MOCK_METHOD(std::shared_ptr<AllSubscription>, GetAll, (),(override));         
};


class DBControllerTest: public ::testing::Test {
public:
    DBControllerTest(): db(new DatabaseMock), test_client_rep(new ClientRepositoryMock), 
            test_timeseries_rep(new TimeSeriesRepositoryMock), 
            db_controller(new DataBaseController(db, test_client_rep, test_timeseries_rep, nullptr)) {
    }

protected:
    std::shared_ptr<IDataBaseController> db_controller;
    std::shared_ptr<DatabaseMock> db;
    std::shared_ptr<ClientRepositoryMock> test_client_rep;
    std::shared_ptr<TimeSeriesRepositoryMock> test_timeseries_rep;
  //  std::shared_ptr<SubscriptionRepositoryMock> test_sub_rep;

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
    EXPECT_CALL(*test_client_rep, GetByKey(_));
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

TEST_F(DBControllerTest, ClientSessionUpdate) {
    Json::Value request;
    request["Type"] = POST_REQUEST;
    request["TypeData"] = CHANGE_USER_SESSION;
    request["login"] = "test";
    request["session_id"] = 1;
    request["token"] = "token";

    EXPECT_CALL(*db, IsOpen()).WillRepeatedly(Return(true)); 
    EXPECT_CALL(*test_client_rep, Update(UPDATE_SESSION, _, _)); 
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
    Json::Value param;
    
    param[0] = 1;

    request["Type"] = POST_REQUEST;
    request["TypeData"] = TIMESERIES_REQUEST;
    request["name_stock"] = "test";
    request["date"] = 1;
    request["param"] = param;

    EXPECT_CALL(*db, IsOpen()).WillRepeatedly(Return(true)); 
    EXPECT_CALL(*test_timeseries_rep, Insert(_)); 
    db_controller->DataRequest(request);
}