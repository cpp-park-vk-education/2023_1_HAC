#include "api_stock.h"
#include "utils_gtest_net.h"
#include "gmock/gmock.h"

TEST(APIModelTests, EmptyPath) {
    EXPECT_NO_THROW(api::APIStockRequest a());
}