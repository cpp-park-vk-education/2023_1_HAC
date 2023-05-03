// #include <gtest/gtest.h>
// #include <gmock/gmock.h>

// // #include "postgresserver.hpp"
// #include "repositories.hpp"
// #include "data.hpp"

// using ::testing::Return;

// // std::string kIpAddr = "127.0.0.1";
// // std::string kPort = "5432";
// // std::string kName = "marketmentor";
// // std::string kUser = "marketmentor_server";
// // std::string kPass = "marketmentor_password";

// // class IDataBase {
// // public:
// //     virtual ~IDataBase() {}
// //     virtual bool IsOpen() = 0;
// //     virtual bool SendQuery(const std::string& query) = 0;
// //     virtual std::vector<std::vector<std::string>> GetData(const std::string& query) = 0;
// //     virtual std::vector<std::string> GetRow(const std::string& query) = 0;
// // };

// class DatabaseMock: public IDataBase { 
// public:
//     MOCK_METHOD(bool, IsOpen, (), (override));
//     MOCK_METHOD(bool, SendQuery, (const std::string& query), (override));
//     MOCK_METHOD(std::vector<std::vector<std::string>>, GetData, (const std::string& query), (override));
//     MOCK_METHOD(std::vector<std::string>, GetRow, (const std::string& query), (override));
// };


// // ClientRepository

// TEST(RepositoryTest, InsertClientDataCase){
//     std::shared_ptr<IDataBase> db = std::make_shared<DatabaseMock>();
//     ClientRepository client_rep(db);
//     auto client = std::make_unique<ClientData>();
//     client->login = "test";
//     client->email = "test@email.com";
//     client->hash_password = "pass";

//     // EXPECT_CALL(*db, IsOpen())
//     // .WillRepeatedly(true);

//     EXPECT_TRUE(client_rep.Insert(client));
// }

// // // TEST(RepositoryTest, InsertClientDataCase){
// // //     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
// // //     db->SendQuery("DELETE FROM client");
// // //     ClientRepository client_rep(db);
// // //     auto client = std::make_unique<ClientData>();
// // //     client->login = "test";
// // //     client->email = "test@email.com";
// // //     client->hash_password = "pass";
    
// // //     EXPECT_TRUE(client_rep.Insert(client));
// // // }

// // TEST(RepositoryTest, DeleteClientDataCase){
// //     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
// //     db->SendQuery("DELETE FROM client");
// //     ClientRepository client_rep(db);
// //     auto client = std::make_unique<ClientData>();
// //     client->login = "test";
// //     client->email = "test@email.com";
// //     client->hash_password = "pass";

// //     client_rep.Insert(client);    
// //     EXPECT_TRUE(client_rep.Delete(client->login));
// // }

// // TEST(RepositoryTest, GetUserDataCase){
// //     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
// //     db->SendQuery("DELETE FROM client");
// //     ClientRepository client_rep(db);
// //     auto client = std::make_unique<ClientData>();
// //     client->login = "test";
// //     client->email = "test@email.com";
// //     client->hash_password = "pass";
// //     client_rep.Insert(client);

// //     auto result = client_rep.GetByKey(client->login);
// //     EXPECT_EQ(result->login, client->login);
// // }

// // TEST(RepositoryTest, BadUserCase){
// //     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
// //     db->SendQuery("DELETE FROM client");
// //     ClientRepository client_rep(db);
// //     auto client = std::make_unique<ClientData>();
// //     client->login = "test";

// //     EXPECT_FALSE(client_rep.Insert(client));

// // }

// // TEST(RepositoryTest, UpdateClientDataCase){
// //     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
// //     db->SendQuery("DELETE FROM client");
// //     ClientRepository client_rep(db);
// //     auto client = std::make_unique<ClientData>();
// //     client->login = "test";
// //     client->email = "test@email.com";
// //     client->hash_password = "pass";
// //     client_rep.Insert(client);
    
// //     // Меняем имя клиента

// //     auto update_client = std::make_unique<ClientData>();
// //     update_client->login = "updated_name";
// //     update_client->email = "test@email.com";
// //     update_client->hash_password = "pass";


// //     client_rep.Update(client->login, update_client);
    
// //     auto result = client_rep.GetByKey(client->login);
// //     EXPECT_EQ(result->login, update_client->login);
// // }

// // // TimeSeries

// // TEST(RepositoryTest, TimeSeriesInsertCase){
// //     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
// //     db->SendQuery("DELETE FROM timeseries");
// //     TimeSeriesRepository timeseries_rep(db);
// //     auto time = std::make_unique<TimeSeriesData>();
// //     time->name_stock = "test";
// //     time->date = "2021-01-02";
// //     std::string json_string = "{\"test\": 1}";
// //     Json::Reader reader;
// //     reader.parse(json_string, time->param);
// //     EXPECT_TRUE(timeseries_rep.Insert(time));
// // }


// // TEST(RepositoryTest, TimeSeriesGetCase){
// //     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
// //     db->SendQuery("DELETE FROM timeseries");
// //     TimeSeriesRepository timeseries_rep(db);
// //     auto time = std::make_unique<TimeSeriesData>();
// //     time->name_stock = "test";
// //     time->date = "2021-01-02";
// //     std::string json_string = "{\"test\": 1}";
// //     Json::Reader reader;
// //     reader.parse(json_string, time->param);
// //     timeseries_rep.Insert(time);

// //     size_t len_lags = 1;
// //     auto result = timeseries_rep.GetByKey(time->name_stock, len_lags);
// //     EXPECT_EQ(result->name_stock, time->name_stock);
// // }


// // TEST(RepositoryTest, TimeSeriesDeleteCase){
// //     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
// //     db->SendQuery("DELETE FROM timeseries");
// //     TimeSeriesRepository timeseries_rep(db);
// //     auto time = std::make_unique<TimeSeriesData>();
// //     time->name_stock = "test";
// //     time->date = "2021-01-02";
// //     std::string json_string = "{\"test\": 1}";
// //     Json::Reader reader;
// //     reader.parse(json_string, time->param);
// //     timeseries_rep.Insert(time);
    
// //     EXPECT_TRUE(timeseries_rep.Delete(time->name_stock));
// // }

// // // TEST(RepositoryTest, TimeSeriesUpdateCase){
// // //     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
// // //     db->SendQuery("DELETE FROM timeseries");
// // //     TimeSeriesRepository timeseries_rep(db);
// // //     auto time = std::make_unique<TimeSeriesData>();
// // //     time->name_stock = "test";
// // //     time->date = "2021-01-02";
// // //     std::string json_string = "{\"test\": 1}";
// // //     Json::Reader reader;
// // //     reader.parse(json_string, time->param);
// // //     timeseries_rep.Insert(time);

// // //     TimeSeriesData new_time;
// // //     new_time.name = "new_test";
// // //     json_string = "{\"new_test\": 1}";
// // //     reader.parse(json_string, new_time.param);

// // //     EXPECT_TRUE(timeseries_rep.Update(time->name_stock, new_time));
// // // }


// // // SubscriptionRepository

// // TEST(RepositoryTest, SubscriptionRepositoryInsert) {
// //     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
// //     db->SendQuery("DELETE FROM subscription");    
// //     SubscriptionRepository sub_rep(db);
// //     auto data = std::make_unique<SubscriptionData>();
// //     data->name = "test";
// //     data->count = 1;
// //     data->cost = 1;

// //     EXPECT_TRUE(sub_rep.Insert(data));
// // } 

// // TEST(RepositoryTest, SubscriptionRepositoryGetByKey) {
// //     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
// //     db->SendQuery("DELETE FROM subscription");    
// //     SubscriptionRepository sub_rep(db);
// //     auto data = std::make_unique<SubscriptionData>();
// //     data->name = "test";
// //     data->count = 1;
// //     data->cost = 1;
// //     sub_rep.Insert(data);

// //     auto result = sub_rep.GetByKey(data->name);
// //     EXPECT_EQ(result->name, data->name);
// // } 

// // TEST(RepositoryTest, SubscriptionRepositoryGetAll) {
// //     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
// //     db->SendQuery("DELETE FROM subscription");    
// //     SubscriptionRepository sub_rep(db);
// //     auto data_0 = std::make_unique<SubscriptionData>();
// //     data_0->name = "test";
// //     data_0->count = 1;
// //     data_0->cost = 1;
// //     sub_rep.Insert(data_0);

// //     auto data_1 = std::make_unique<SubscriptionData>();
// //     data_1->name = "test2";
// //     data_1->count = 2;
// //     data_1->cost = 2;
// //     sub_rep.Insert(data_1);

// //     const int kValueId = 1;
// //     auto result = sub_rep.GetAll();
// //     EXPECT_EQ(result->data[kValueId].name, data_1->name);
// // } 

