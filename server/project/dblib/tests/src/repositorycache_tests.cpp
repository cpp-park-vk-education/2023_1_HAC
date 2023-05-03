// #include <gtest/gtest.h>
// #include <gmock/gmock.h>
// #include "repositorycache.hpp"

// struct TestStruct {
//     std::string name;
// };

// TEST(RepositoryCacheTest, InsertCase) {

//     struct TestStruct {
//         std::string name;
//     };

//     std::unique_ptr<IRepositoryCache<std::string, TestStruct>> cache = std::make_unique<RepositoryCache<std::string, TestStruct>>();
//     TestStruct data;
//     data.name = "test";
//      EXPECT_NO_THROW({
//          cache->Insert(data.name, data);
//      }); 
// }

// TEST(RepositoryCacheTest, DeleteCase) {
// struct TestStruct {
//     std::string name;
// };

//     std::unique_ptr<IRepositoryCache<std::string, TestStruct>> cache = std::make_unique<RepositoryCache<std::string, TestStruct>>();
//     TestStruct data;
//     data.name = "test";
//     cache->Insert(data.name, data);    
//     EXPECT_NO_THROW({
//         cache->Delete(data.name);
//     });
// }

// TEST(RepositoryCacheTest, NotExistDeleteCase) {

//     struct TestStruct {
//     std::string name;
// };

//     std::unique_ptr<IRepositoryCache<std::string, TestStruct>> cache = std::make_unique<RepositoryCache<std::string, TestStruct>>();
//     std::string random_key = "test";
//     EXPECT_NO_THROW({
//         cache->Delete(random_key);
//     });
// }

// TEST(RepositoryCacheTest, HasCase) {
//     struct TestStruct {
//     std::string name;
// };

//     std::unique_ptr<IRepositoryCache<std::string, TestStruct>> cache = std::make_unique<RepositoryCache<std::string, TestStruct>>();
//     TestStruct data;
//     data.name = "test";
//     cache->Insert(data.name, data);
//     EXPECT_TRUE(cache->Has(data.name));
// }

// TEST(RepositoryCacheTest, NotHasCase) {
//     struct TestStruct {
//     std::string name;
// };

//     std::unique_ptr<IRepositoryCache<std::string, TestStruct>> cache = std::make_unique<RepositoryCache<std::string, TestStruct>>();
//     std::string random_key = "test";
//     EXPECT_FALSE(cache->Has(random_key));
// }

// TEST(RepositoryCacheTest, GetCase) {
//     struct TestStruct {
//     std::string name;
// };

//     std::unique_ptr<IRepositoryCache<std::string, TestStruct>> cache = std::make_unique<RepositoryCache<std::string, TestStruct>>();
//     TestStruct data;
//     data.name = "test";
//     cache->Insert(data.name, data);
//     TestStruct result = cache->Get(data.name);
//     EXPECT_EQ(result.name, data.name);     
// }

