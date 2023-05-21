#include "utils_gtest_net.h"
#include "gmock/gmock.h"
#include "dbcontroller.hpp"
#include "server.h"

TEST(ServerTest, StartWithoutConfig){
    const std::string path_to_config_file = "config.conf";
    EXPECT_THROW(Server server(path_to_config_file), market_mentor::InvalidServerConfig);
};

TEST(ServerTest, StartWithConfig) {

    const std::string path_to_config_file = "test_config.conf";
    if (!std::ifstream(path_to_config_file)) {
        std::ofstream create_file(path_to_config_file);
        create_file.close();
    }

    std::ofstream file(path_to_config_file);
    file << "0.0.0.0" << "\n" << "mmm" << "\n" << 2 << "\n";
    file.close();

    EXPECT_THROW(Server server(path_to_config_file), market_mentor::InvalidServerConfig);
};
