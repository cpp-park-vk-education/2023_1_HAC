#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "repositories.hpp"

using ::testing::Return;
using ::testing::Throw;
using ::testing::_;

using namespace repository;
using namespace database;

class DatabaseMock: public IDataBase {
public:
    ~DatabaseMock() = default;
    MOCK_METHOD(bool, IsOpen, (),(override));
    MOCK_METHOD(bool, SendQuery, (const std::string& query),(override));
    MOCK_METHOD(Json::Value, GetData, (const std::string& query),(override));
    MOCK_METHOD(Json::Value, GetRow, (const std::string& query),(override));
};

class RepositoryTest: public ::testing::Test {
public:
    RepositoryTest(): db(new DatabaseMock), client_rep(new ClientRepository(db)),
         timeseries_rep(new TimeSeriesRepository(db)) , sub_rep(new SubscriptionRepository(db)) {}

protected:
    std::shared_ptr<DatabaseMock> db;
    std::shared_ptr<IClientRepository> client_rep;
    std::shared_ptr<ITimeSeriesRepository> timeseries_rep;
    std::shared_ptr<ISubscriptionRepository> sub_rep;
};


// ClientRepository

TEST_F(RepositoryTest, InsertClientDataCase){
    auto client = std::make_shared<ClientData>();
    client->login = "test";
    client->email = "test@email.com";
    client->hash_password = "pass";

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, SendQuery(_)).WillOnce(Return(true)); 
 
    EXPECT_TRUE(client_rep->Insert(client));
}

// Получаем данные от БД и сравниваем их с данными, которые должны быть на выходе метода
TEST_F(RepositoryTest, ClientGetCase){

    auto client = std::make_shared<ClientData>();
    client->login = "test";
    client->email = "test@email.com";
    client->hash_password = "pass";

    Json::Value row;
    row["login"] = client->login;
    row["email"] = client->email;
    row["hash_password"] = client->hash_password;


    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, GetRow(_)).WillOnce(Return(row)); 
    
    EXPECT_NO_THROW(client_rep->GetByKey(client->login));
}


TEST_F(RepositoryTest, DeleteClientDataCase){
    std::string key = "test";    
    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, SendQuery(_)).WillOnce(Return(true)); 
    EXPECT_TRUE(client_rep->Delete(key));
}


TEST_F(RepositoryTest, UpdateClientDataCase){
    std::string key = "test";    

    // Меняем имя клиента

    auto update_client = std::make_shared<ClientData>();
    update_client->login = "updated_name";
    update_client->email = "test@email.com";
    update_client->hash_password = "pass";

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, SendQuery(_)).WillOnce(Return(true)); 
    EXPECT_TRUE(client_rep->Update(key, update_client));
}


TEST_F(RepositoryTest, BadQueryForPostUser){
    auto client = std::make_shared<ClientData>();
    client->login = "test";
    client->email = "test@email.com";
    client->hash_password = "pass";

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(false));
    EXPECT_CALL(*db, SendQuery(_)).WillOnce(Return(false)); 

    EXPECT_FALSE(client_rep->Insert(client));
    EXPECT_FALSE(client_rep->Delete(client->login));
}

TEST_F(RepositoryTest, BadQueryForGetUser){
    std::string key = "test";

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, GetRow(_)).WillOnce(Throw(SqlError("Error"))); 

    EXPECT_THROW({
        auto client = client_rep->GetByKey(key);
    }, UnavailableDataBase);
}

// TimeSeries

TEST_F(RepositoryTest, TimeSeriesInsertCase){
    Json::Value value;
    Json::Reader reader; 
    std::string json_string = "{\"test\": 1}";
    reader.parse(json_string, value);
    
    auto time = std::make_shared<TimeSeriesData>();
    time->name_stock = "test";
    time->date = "2021-01-02";
    time->param = value;

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, SendQuery(_)).WillOnce(Return(true)); 
    EXPECT_TRUE(timeseries_rep->Insert(time));
}

TEST_F(RepositoryTest, TimeSeriesGetCase){

    auto time = std::make_shared<TimeSeriesData>();
    time->name_stock = "test";
    time->date = "2021-01-02";
    std::string json_string = "{\"test\": 1}";
    Json::Reader reader;
    reader.parse(json_string, time->param);

    Json::Value row;
    row["name_stock"] = time->name_stock;
    row["date"] = time->name_stock;
    row["param"] = time->param;

    size_t len_lags = 1;

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, GetRow(_)).WillOnce(Return(row)); 

    EXPECT_NO_THROW(timeseries_rep->GetByKey(time->name_stock, len_lags));
}

TEST_F(RepositoryTest, BadQueryForPostTimeSeries){
    auto time = std::make_shared<TimeSeriesData>();
    time->name_stock = "test";
    time->date = "2021-01-02";
    std::string json_string = "{\"test\": 1}";
    Json::Reader reader;
    reader.parse(json_string, time->param);

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(false));
    EXPECT_CALL(*db, SendQuery(_)).WillOnce(Return(false)); 
    EXPECT_FALSE(timeseries_rep->Insert(time));
}

TEST_F(RepositoryTest, BadQueryForGetTimeSeries){
    std::string name_stock = "test";
    size_t len_lags = 1;
    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, GetRow(_)).WillOnce(Throw(SqlError("Error"))); 

    EXPECT_THROW({
        timeseries_rep->GetByKey(name_stock, len_lags);
    }, UnavailableDataBase);
}


// SubscriptionRepository

TEST_F(RepositoryTest, BadConnectForGetSubscription){
    std::string key = "test";

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, GetRow(_)).WillOnce(Throw(SqlError("Error"))); 

    EXPECT_THROW({
        sub_rep->GetByKey(key);
    }, UnavailableDataBase);
}

TEST_F(RepositoryTest, BadConnectForGetAllSubscription) {
    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, GetRow(_)).WillOnce(Throw(SqlError("Error"))); 
    EXPECT_THROW({
        sub_rep->GetAll();
    }, UnavailableDataBase);
}
