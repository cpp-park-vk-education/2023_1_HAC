#include <iostream>
#include "server.h"
#include "listener.h"
// #include <boost/config.hpp>

Server::Config parseConfigFromFile(const std::string& path_to_config_file) {
    Server::Config config;
    config.address = "127.0.0.1";
    config.port = 12;
    config.threads = 10;
    return config;
}

Server::Server(const std::string& path_to_config_file) {
    
    // database_controller();
    // predict_controller();
    // show_plot_controller();
    // register_controller();
    // authorize_controller();

    // predict_hendler(&database_controller);
    // register_hendler(&database_controller);
    // authorize_hendler(&database_controller);
    // update_hendler(&database_controller);
    
    config_ = parseConfigFhomFile(path_to_config_file);

    // router_();
    // listner(&router_);

    boost::asio::io_context ioc_{config_.threads};

};