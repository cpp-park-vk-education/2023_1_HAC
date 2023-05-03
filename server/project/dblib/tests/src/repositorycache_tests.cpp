#include <gtest/gtest.h>
#include "repositorycache.hpp"

using namespace cache;

struct TestStruct {
    std::string name;
};
class RepositoryCacheTest: public ::testing::Test {
public:
    RepositoryCacheTest(): cache_(new RepositoryCache<std::string, TestStruct>){}

protected:
    std::unique_ptr<IRepositoryCache<std::string, TestStruct>> cache_;
};

TEST_F(RepositoryCacheTest, InsertCase) {
    TestStruct data;
    data.name = "test";
     EXPECT_NO_THROW({
         cache_->Insert(data.name, data);
     }); 
}

TEST_F(RepositoryCacheTest, HasCase) {
    TestStruct data;
    data.name = "test";
    cache_->Insert(data.name, data);
    EXPECT_TRUE(cache_->Has(data.name));
}

TEST_F(RepositoryCacheTest, GetCase) {
    std::string key = "test";
    TestStruct result = cache_->Get(key);
    EXPECT_EQ(result.name, key);     
}

TEST_F(RepositoryCacheTest, DeleteCase) {
    TestStruct data;
    data.name = "test";
    EXPECT_TRUE(cache_->Delete(data.name));
}

TEST_F(RepositoryCacheTest, NotExistDeleteCase) {
    std::string random_key = "test";
    EXPECT_FALSE(cache_->Delete(random_key));

}

TEST_F(RepositoryCacheTest, NotHasCase) {
    std::string random_key = "test";
    EXPECT_FALSE(cache_->Has(random_key));
}



