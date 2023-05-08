#include "utils_gtest_net.h"
#include "gmock/gmock.h"
#include "dbcontroller.hpp"
#include "server.h"

TEST(ServerTest, StartWithConfig) {
    Server::Config config;
    config.address = "127.0.0.1";
    config.port = 8000;
    config.threads = 2;
    
    // Создаём временный файл с конфигурацией
    const std::string path_to_config_file = "/tmp/test_config.conf";
    std::ofstream file(path_to_config_file);
    file << "[Server]\n"
         << "address=" << config.address << "\n"
         << "port=" << config.port << "\n"
         << "threads=" << config.threads << "\n";
    file.close();

    EXPECT_NO_THROW(Server server(path_to_config_file));
}

TEST(ServerTest, StartWithoutConfig){
    const std::string path_to_config_file = "/tmp/test_config.conf";
    EXPECT_THROW(Server server(path_to_config_file), std::runtime_error);
}
