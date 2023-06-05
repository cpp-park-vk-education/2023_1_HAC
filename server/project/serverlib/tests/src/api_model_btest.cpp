#include "api_model.h"
#include <fstream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "utils_gtest_net.h"
#include "icontrollers.h"

TEST(APIModelTest, CreateApiUnitWrongPath) {
    EXPECT_THROW(api::APIModelRequest api_model(""), market_mentor::InvalidServerConfig);
}

TEST(APIModelTest, CreateApiUnitWrongConfig) {
    const std::string path_to_config_file = "test_config.conf";
    if (!std::ifstream(path_to_config_file)) {
        std::ofstream create_file(path_to_config_file);
        create_file.close();
    }

    std::ofstream file(path_to_config_file);
    file << "0.0.0.0" << "\n" << "mmm" << "\n";
    file.close();
    EXPECT_THROW(api::APIModelRequest api_model(path_to_config_file), market_mentor::InvalidServerConfig);
}

TEST(APIModelTest, CreateApiUnitCorrectConfig) {
    const std::string path_to_config_file = "test_config.conf";
    if (!std::ifstream(path_to_config_file)) {
        std::ofstream create_file(path_to_config_file);
        create_file.close();
    }

    std::ofstream file(path_to_config_file);
    file << "0.0.0.0" << "\n" << "8080" << "\n";
    file.close();
    EXPECT_NO_THROW(api::APIModelRequest api_model(path_to_config_file));
}

TEST(APIModelTest, CreateApiUnitGetDataWrongIP) {
    const std::string path_to_config_file = "test_config.conf";
    if (!std::ifstream(path_to_config_file)) {
        std::ofstream create_file(path_to_config_file);
        create_file.close();
    }

    std::ofstream file(path_to_config_file);
    file << "266.266.266.266" << "\n" << "8080" << "\n";
    file.close();
    api::IAPIModelRequest* api_model = new api::APIModelRequest(path_to_config_file);

    controllers::TimeSeriesPredicts samples_data;
    EXPECT_THROW(api_model->getData(samples_data), market_mentor::ConnectionException);
}
