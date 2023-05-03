#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "data.hpp"
#include "dbcontroller.hpp"

using ::testing::Return;
using namespace dbcontroller;
using namespace repository;
using namespace database;


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
    DBControllerTest(): test_client_rep(new ClientRepositoryMock()), test_timeseries_rep(new TimeSeriesRepositoryMock()),
     test_sub_rep(new SubscriptionRepositoryMock()), db_controller(new DataBaseController(test_client_rep, test_timeseries_rep, test_sub_rep)) {
    }

protected:
    std::unique_ptr<IDataBaseController> db_controller;
    std::shared_ptr<ClientRepositoryMock> test_client_rep;
    std::shared_ptr<TimeSeriesRepositoryMock> test_timeseries_rep;
    std::shared_ptr<SubscriptionRepositoryMock> test_sub_rep;

};

// Client


TEST_F(DBControllerTest, ClientPost) {
    DBRequestProtocol<ClientData> request;
    request.type_request = POST_REQUEST;
    request.data = std::make_shared<ClientData>();
    request.data->login = "test";
    request.data->email = "test@email.com";
    request.data->hash_password = "pass";
    
    EXPECT_CALL(*test_client_rep, Insert(request.data)).WillOnce(Return(true)); 
    db_controller->DataRequest(request);
}

TEST_F(DBControllerTest, ClientGet) {
    DBRequestProtocol<ClientData> request;
    request.type_request = GET_REQUEST;
    request.data = std::make_shared<ClientData>();
    request.data->login = "test";
    request.data->email = "test@email.com";
    request.data->hash_password = "pass";
    
    EXPECT_CALL(*test_client_rep, GetByKey(request.data->login));
    db_controller->DataRequest(request);
}

TEST_F(DBControllerTest, ClientUpdate) {
    DBRequestProtocol<ClientData> request;
    request.type_request = UPDATE_REQUEST;
    request.data = std::make_shared<ClientData>();
    request.data->login = "test";
    EXPECT_CALL(*test_client_rep, Update(request.data->login, request.data)).WillOnce(Return(true)); 
    db_controller->DataRequest(request);
}

TEST_F(DBControllerTest, ClientAutorize) {
    DBRequestProtocol<AuthorizeData> request;
    request.type_request = GET_REQUEST;
    request.data = std::make_shared<AuthorizeData>();
    request.data->login = "test";
    EXPECT_CALL(*test_client_rep, GetByKey(request.data->login));
    db_controller->DataRequest(request);

}

// TimeSeries

TEST_F(DBControllerTest, GetTimeSeries) {
    DBRequestProtocol<TimeSeriesRequest> request;
    request.type_request = GET_REQUEST;
    request.data = std::make_shared<TimeSeriesRequest>();
    request.data->name_stock = "test";
    request.data->len_lags = 1;
    EXPECT_CALL(*test_timeseries_rep, GetByKey(request.data->name_stock, request.data->len_lags));
    db_controller->DataRequest(request);

}


TEST_F(DBControllerTest, PostTimeSeries) {
    DBRequestProtocol<TimeSeriesData> request;
    request.type_request = POST_REQUEST;
    request.data = std::make_shared<TimeSeriesData>();
    request.data->name_stock = "test";
    request.data->date = "2020";
    EXPECT_CALL(*test_timeseries_rep, Insert(request.data)).WillOnce(Return(true)); 
    db_controller->DataRequest(request);
}