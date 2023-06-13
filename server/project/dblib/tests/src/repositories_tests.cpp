#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "repositories.hpp"

using ::testing::Return;
using ::testing::Throw;
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


class RepositoryTest: public ::testing::Test {
public:
    RepositoryTest(): db(new DatabaseMock), redis(new MemoryDatabaseMock), 
            client_rep(new repository::ClientRepository(db)),
            timeseries_rep(new repository::TimeSeriesRepository(db)) , 
            sub_rep(new repository::SubscriptionRepository(db)), 
            token_rep(new repository::TokenRepository(redis)) {}

protected:
    std::shared_ptr<DatabaseMock> db;
    std::shared_ptr<MemoryDatabaseMock> redis;
    std::shared_ptr<repository::IClientRepository> client_rep;
    std::shared_ptr<repository::ITimeSeriesRepository> timeseries_rep;
    std::shared_ptr<repository::ISubscriptionRepository> sub_rep;
    std::shared_ptr<repository::ITokenRepository> token_rep;
};


// ClientRepository

TEST_F(RepositoryTest, InsertClientDataCase){
    auto client = std::make_shared<ClientData>();
    client->login = "test";
    client->email = "test@email.com";
    client->hash = "pass";

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, SendQuery(_)).WillOnce(Return(true)); 
 
    EXPECT_TRUE(client_rep->Insert(client));
}

TEST_F(RepositoryTest, ClientGetCase){

    auto client = std::make_shared<ClientData>();
    client->login = "test";
    client->email = "test@email.com";
    client->hash = "pass";
    ClientGetType type = LOGIN_KEY;
    Json::Value row;
    row[0] = "1";
    row[1] = client->login;
    row[2] = client->email;
    row[3] = client->hash;

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, GetRow(_)).WillOnce(Return(row)); 
    
    EXPECT_NO_THROW(client_rep->GetByKey(type, client->login));
}


TEST_F(RepositoryTest, DeleteClientDataCase){
    std::string key = "test";    
    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, SendQuery(_)).WillOnce(Return(true)); 
    EXPECT_TRUE(client_rep->Delete(key));
}


TEST_F(RepositoryTest, UpdatePasswordClientDataCase){
    std::string key = "test";    

    auto update_client = std::make_shared<ClientData>();
    update_client->login = "login";
    update_client->hash = "1";

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, SendQuery(_)).WillOnce(Return(true)); 
    ClientUpdateType type = UPDATE_PASSWORD;
    EXPECT_TRUE(client_rep->Update(type, key, update_client));
}

TEST_F(RepositoryTest, UpdateEmailClientDataCase){
    std::string key = "test";    

    auto update_client = std::make_shared<ClientData>();
    update_client->login = "login";
    update_client->email = "new_email";

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, SendQuery(_)).WillOnce(Return(true)); 
    ClientUpdateType type = UPDATE_EMAIL;
    EXPECT_TRUE(client_rep->Update(type, key, update_client));
}

TEST_F(RepositoryTest, BadQueryForPostUser){
    auto client = std::make_shared<ClientData>();
    client->login = "test";
    client->email = "test@email.com";
    client->hash = "pass";

    EXPECT_CALL(*db, IsOpen()).WillRepeatedly(Return(false));
    EXPECT_CALL(*db, SendQuery(_)).WillRepeatedly(Return(false)); 

    EXPECT_FALSE(client_rep->Insert(client));
    EXPECT_FALSE(client_rep->Delete(client->login));
}

TEST_F(RepositoryTest, BadQueryForGetUser){
    std::string key = "test";
    ClientGetType type = LOGIN_KEY;
    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, GetRow(_)).WillOnce(Throw(ConnectError("Error"))); 
    
    auto client = client_rep->GetByKey(type,key);
    EXPECT_EQ(client, nullptr);
}

// TimeSeries

TEST_F(RepositoryTest, TimeSeriesInsertCase){
    Json::Value value, date;
    value[0] = 1;
    date[0] = "2021-01-02 19:00";
    auto time = std::make_shared<TimeSeriesData>();
    time->name_stock = "test";
    time->date = date;
    time->param = value;

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, SendQuery(_)).WillOnce(Return(true)); 
    EXPECT_TRUE(timeseries_rep->Insert(time));
}

TEST_F(RepositoryTest, TimeSeriesGetCase){

    auto time = std::make_shared<TimeSeriesData>();
    time->name_stock = "test";
    time->date = "2021-01-02 19:00";
    std::string json_string = "1";
    Json::Reader reader;
    reader.parse(json_string, time->param);

    Json::Value row;
    row[0] = 0;
    row[1] = time->name_stock;
    row[2] = time->date;
    row[3] = time->param;

    Json::Value table;
    table[0] = row;
    size_t len_lags = 1;

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, GetData(_)).WillOnce(Return(table)); 

    EXPECT_NO_THROW(timeseries_rep->GetByKey(time->name_stock, len_lags));
}

TEST_F(RepositoryTest, TimeSeriesGetCaseWithDate){

    auto time = std::make_shared<TimeSeriesData>();
    time->name_stock = "test";
    time->date = "2021-01-02 19:00";
    std::string json_string = "1";
    Json::Reader reader;
    reader.parse(json_string, time->param);

    Json::Value row;
    row[0] = 0;
    row[1] = time->name_stock;
    row[2] = time->date;
    row[3] = time->param;

    Json::Value table;
    table[0] = row;
    size_t len_lags = 1;
    std::string start_time = "2020-01-01 19:00";
    std::string finish_time = "2023-01-01 19:00";

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, GetData(_)).WillOnce(Return(table)); 

    EXPECT_NO_THROW(timeseries_rep->GetByKey(time->name_stock, len_lags, start_time, finish_time));
}

TEST_F(RepositoryTest, BadQueryForPostTimeSeries){
    auto time = std::make_shared<TimeSeriesData>();
    Json::Value date;
    time->name_stock = "test";
    time->date[0] ="2021-01-02";
    time->param[0] = 1;

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, SendQuery(_)).WillOnce(Return(false)); 
    EXPECT_FALSE(timeseries_rep->Insert(time));
}

TEST_F(RepositoryTest, BadQueryForGetTimeSeries){
    std::string name_stock = "test";
    size_t len_lags = 1;
    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, GetData(_)).WillOnce(Throw(ConnectError("Error"))); 

    EXPECT_EQ(timeseries_rep->GetByKey(name_stock, len_lags), nullptr);
}

TEST_F(RepositoryTest, GetAllTimeSeries){
    Json::Value data_from_db;
    data_from_db[0][0] = "1";
    data_from_db[1][0] = "test";
    data_from_db[2][0] = "1";
    data_from_db[3][0] = "1";

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, GetData(_)).WillOnce(Return(data_from_db)); 

    EXPECT_NO_THROW(timeseries_rep->GetAllStocks());
}

// SubscriptionRepository

TEST_F(RepositoryTest, GetSubscription) {
    std::string key = "test";

    Json::Value data_from_db;
    data_from_db[0] = "1";
    data_from_db[1] = "test";
    data_from_db[2] = "1";
    data_from_db[3] = "1";

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, GetRow(_)).WillOnce(Return(data_from_db)); 

    EXPECT_NO_THROW(sub_rep->GetByKey(key));
}

TEST_F(RepositoryTest, GetAllSubscription) {
    Json::Value data_from_db, row;

    row[0] = "1";
    row[1] = "test";
    row[2] = "1";
    row[3] = "1";
    data_from_db[0] = row;
    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, GetData(_)).WillOnce(Return(data_from_db)); 

    EXPECT_NO_THROW(sub_rep->GetAll());
}

TEST_F(RepositoryTest, BadConnectForGetSubscription){
    std::string key = "test";

    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, GetRow(_)).WillOnce(Throw(ConnectError("Error"))); 

    EXPECT_EQ(sub_rep->GetByKey(key), nullptr);
}

TEST_F(RepositoryTest, BadConnectForGetAllSubscription) {
    EXPECT_CALL(*db, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*db, GetData(_)).WillOnce(Throw(ConnectError("Error"))); 

    EXPECT_EQ(sub_rep->GetAll(), nullptr);

}


// Token

TEST_F(RepositoryTest, GetTokenCase) {
    auto data = std::make_shared<TokenData>();
    data->token = "Key";
    data->login = "login";
    data->time_live = 0;

    EXPECT_CALL(*redis, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*redis, Get(_)).WillOnce(Return("login")); 
    EXPECT_EQ(token_rep->Get("Key")->login, data->login);
}

TEST_F(RepositoryTest, GetBadTokenCase) {
    EXPECT_CALL(*redis, IsOpen()).WillOnce(Return(true));
    EXPECT_CALL(*redis, Get(_)).WillOnce(Return("NULL")); 
    EXPECT_EQ(token_rep->Get("Key"), nullptr);
}