#pragma once // NO_LINT
#include <memory>
#include <vector>
#include <thread>
#include <string>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>

namespace net = boost::asio; // from <boost/asio.hpp>

class Server {
 public:
    Server() = delete;
    Server(const std::string& path_to_config_file);

    struct Config {
        std::string address;
        int port;
        int threads;
    };

 private:
    // controllers::DatabaseController database_controller;
    // controllers::PredictController predict_controller;
    // controllers::ShowPlotContoller show_plot_controller;
    // controllers::RegisterController register_controller;
    // controllers::AuthorizeController authorize_controller;

    // hendlers::Hendler predict_hendler;
    // hendlers::Hendler register_hendler;
    // hendlers::Hendler authorize_hendler;
    // hendlers::Hendler update_hendler;

    // Router router_;
    // Listener lister_;
    // net::io_context ioc
    std::map<string, *IHandler> handlers;
    std::vector<std::thread> listeners;
    Config config_;

    void setHandlers();
    Config parseConfigFhomFile(const std::string& path_to_config_file);
    ProtocolAPI makeProtocolAPI(const std::string& path_to_API_config);
};  
