#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "data.hpp"
#include "dbcontroller.hpp"

using ::testing::Return;
using ::testing::_;

using namespace dbcontroller;
using namespace repository;

// DBController вызывает только соотвествующие методы у репозиториев и получает результат

class ClientRepositoryMock: public IClientRepository {
public:
    ~ClientRepositoryMock() override = default;
    MOCK_METHOD(bool, Insert, (const std::shared_ptr<ClientData>& data),(override));
    MOCK_METHOD(bool, Delete, (const std::string& key),(override));         
    MOCK_METHOD(bool, Update, (const std::string& key, const std::shared_ptr<ClientData>& data),(override));         
    MOCK_METHOD(std::shared_ptr<ClientData>, GetByKey, (const std::string& key),(override));         
};

class TimeSeriesRepositoryMock: public ITimeSeriesRepository {
public:
    ~TimeSeriesRepositoryMock() override = default;
    MOCK_METHOD(bool, Insert, (const std::shared_ptr<TimeSeriesData>& data),(override));
    MOCK_METHOD(std::shared_ptr<TimeSeriesData>, GetByKey, (const std::string& name_stock, const size_t& len_lags),(override));         
};

class SubscriptionRepositoryMock: public ISubscriptionRepository {
public:
    ~SubscriptionRepositoryMock() override = default;     
    MOCK_METHOD(std::shared_ptr<SubscriptionData>, GetByKey, (const std::string& key),(override));   
    MOCK_METHOD(std::shared_ptr<AllSubscription>, GetAll, (),(override));         
};


class DBControllerTest: public ::testing::Test {
public:
    DBControllerTest(): test_client_rep(new ClientRepositoryMock), test_timeseries_rep(new TimeSeriesRepositoryMock),
     test_sub_rep(new SubscriptionRepositoryMock), db_controller(new DataBaseController(test_client_rep, test_timeseries_rep, test_sub_rep)) {
    }

protected:
    std::unique_ptr<IDataBaseController> db_controller;
    std::shared_ptr<ClientRepositoryMock> test_client_rep;
    std::shared_ptr<TimeSeriesRepositoryMock> test_timeseries_rep;
    std::shared_ptr<SubscriptionRepositoryMock> test_sub_rep;

};

// Client

TEST_F(DBControllerTest, ClientPost) {

    Json::Value request;
    request["Type"] = POST_REQUEST;
    request["TypeData"] = CLIENT_DATA;
    request["login"] = "test";
    request["email"] = "test@email.com";
    request["password"] = "pass";
    
    EXPECT_CALL(*test_client_rep, Insert(_)); 
    db_controller->DataRequest(request);
}

TEST_F(DBControllerTest, ClientGet) {
    Json::Value request;
    request["Type"] = GET_REQUEST;
    request["TypeData"] = CLIENT_DATA;
    request["login"] = "test";
    request["email"] = "test@email.com";
    request["password"] = "pass";

    EXPECT_CALL(*test_client_rep, GetByKey(_));
    db_controller->DataRequest(request);
}

TEST_F(DBControllerTest, ClientUpdate) {
    Json::Value request;
    request["Type"] = UPDATE_REQUEST;
    request["TypeData"] = CLIENT_DATA;
    request["login"] = "test";
    request["email"] = "test@email.com";
    request["password"] = "pass";

    EXPECT_CALL(*test_client_rep, Update(_, _)); 
    db_controller->DataRequest(request);
}

TEST_F(DBControllerTest, ClientAutorize) {
    Json::Value request;
    request["Type"] = GET_REQUEST;
    request["TypeData"] = AUTHORIZE_DATA;
    request["login"] = "test";
    request["password"] = "pass";

    EXPECT_CALL(*test_client_rep, GetByKey(_));
    db_controller->DataRequest(request);

}

// TimeSeries

TEST_F(DBControllerTest, GetTimeSeries) {
    Json::Value request;
    request["Type"] = GET_REQUEST;
    request["TypeData"] = TIMESERIES_REQUEST;
    request["name_stock"] = "test";
    request["len_lags"] = 1;

    EXPECT_CALL(*test_timeseries_rep, GetByKey(_, _));
    db_controller->DataRequest(request);

}


TEST_F(DBControllerTest, PostTimeSeries) {
    Json::Value request;
    Json::Value param;
    
    std::vector<double> param;
    param.push_back(0.1);

    request["Type"] = POST_REQUEST;
    request["TypeData"] = TIMESERIES_DATA;
    request["name_stock"] = "test";
    request["date"] = 1;
    request["param"] = param;

    EXPECT_CALL(*test_timeseries_rep, Insert(_)); 
    db_controller->DataRequest(request);
}