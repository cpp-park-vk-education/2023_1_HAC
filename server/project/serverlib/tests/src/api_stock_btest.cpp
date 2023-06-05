#include "api_stock.h"
#include "server_exception.h"
#include "http_protocol.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <fstream>

TEST(APIStocksTest, CreateApiUnitWrongPath) {
    EXPECT_THROW(api::APIStockRequest api_stock(""), market_mentor::InvalidServerConfig);
}

TEST(APIStockTest, CreateApiUnitCorrectConfig) {
    const std::string path_to_config_file = "test_config.conf";
    if (!std::ifstream(path_to_config_file)) {
        std::ofstream create_file(path_to_config_file);
        create_file.close();
    }

    std::ofstream file(path_to_config_file);
    file << "finnhub.io" << "\n" << "31415dooejf" << "\n";
    file.close();
    EXPECT_NO_THROW(api::APIStockRequest api_stock(path_to_config_file));
}

TEST(APIStockTest, CreateApiUnitGetDataWrongIP) {
    const std::string path_to_config_file = "test_config.conf";
    if (!std::ifstream(path_to_config_file)) {
        std::ofstream create_file(path_to_config_file);
        create_file.close();
    }

    std::ofstream file(path_to_config_file);
    file << "finnnnnnnnnhub.io" << "\n" << "31415dooejf" << "\n";
    file.close();

    api::IAPIStockRequest* api_stock = new api::APIStockRequest(path_to_config_file);
    
    handlers::ProtocolAPI protocol;
    EXPECT_THROW(api_stock->getSeveralStockPrice(protocol), market_mentor::ConnectionException);
}

TEST(APIStockTest, CorrectApiIP) {
    const std::string path_to_config_file = "test_config.conf";
    if (!std::ifstream(path_to_config_file)) {
        std::ofstream create_file(path_to_config_file);
        create_file.close();
    }

    std::ofstream file(path_to_config_file);
    file << "finnhub.io" << "\n" << "31415dooejf" << "\n";
    file.close();

    api::IAPIStockRequest* api_stock = new api::APIStockRequest(path_to_config_file);
    
    handlers::ProtocolAPI protocol;
    EXPECT_NO_THROW(api_stock->getSeveralStockPrice(protocol));
}