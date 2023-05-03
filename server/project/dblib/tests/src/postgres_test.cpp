// #include <gtest/gtest.h>
// #include <gmock/gmock.h>

// #include "postgresserver.hpp"

// //Будем использовать таблицу Test со столбцами id и value(var char)
// std::string kIpAddr = "127.0.0.1";
// std::string kPort = "5432";
// std::string kName = "test";
// std::string kUser = "admin";
// std::string kPass = "admin";


// TEST(PostgresTest, ConnectCase){
//     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
//     EXPECT_TRUE(db->IsOpen());
// }

// TEST(PostgresTest, BadConnectCase){
//     std::string kBadPass = "bad_pass";
//     EXPECT_THROW({
//         std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kBadPass);
//     }, ConnectError);
// }

// TEST(PostgresTest, TransactionCase){
//     std::string query = "DELETE from test";
//     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
//     EXPECT_TRUE(db->SendQuery(query));
// }

// TEST(PostgresTest, BadTransactionCase){
//     std::string query = "DELETE";
//     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
//     EXPECT_FALSE(db->SendQuery(query));
// }

// TEST(PostgresTest, SelectRowCase){
//     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
//     db->SendQuery("DELETE from test");

//     std::string query = "INSERT into test(id, value) VALUES(1,'test')";
//     db->SendQuery(query);

//     std::string new_quary = "SELECT * FROM test where id = 1";
//     std::vector<std::string> result = db->GetRow(new_quary);
//     const int kValueId = 1;
//     EXPECT_EQ(result[kValueId], "test");
// }

// TEST(PostgresTest, BadSelectCase){
//     std::string query = "INSERT into test(id, value) VALUES(1,'test')";
//     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
//     db->SendQuery("DELETE from test");
//     db->SendQuery(query);

//     std::string new_quary = "SELECT * from ";
//     EXPECT_THROW({
//         auto result = db->GetRow(new_quary);
//     }, SqlError);   
// }

// TEST(PostgresTest, SelectTableCase){
//     std::shared_ptr<IDataBase> db = std::make_shared<PostgresServer>(kIpAddr, kPort, kName, kUser, kPass);
//     db->SendQuery("DELETE from test");
//     db->SendQuery("INSERT into test(id, value) VALUES(1,'test')");
//     db->SendQuery("INSERT into test(id, value) VALUES(2, 'new_test')");

//     std::vector<std::vector<std::string>> result = db->GetData("SELECT * from test");
//     const int kRowId = 1;
//     const int kValueId = 1;
//     EXPECT_EQ(result[kRowId][kValueId], "new_test");
// }

