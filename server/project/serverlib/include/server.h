#pragma once // NO_LINT
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/config.hpp>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <thread>
#include <string>

#include "listener.h"
#include "routers.h"
#include "ihandler.h"
#include "icontrollers.h"
#include "handlers.h"
#include "controllers.h"
#include "config_handler.h"
#include "dbcontroller.hpp"
#include "api_model.h"
#include "api_stock.h"
#include "server_exception.h"

using prtToIHandler = std::unique_ptr<handlers::IHandler>;
using IDatabaseController = std::string;

namespace net = boost::asio; // from <boost/asio.hpp>

struct Config {
    std::string address;
    unsigned short port;
    int threads;
};


class Server : public std::enable_shared_from_this<Server>{
 public:
    Server() = delete;
    Server(const std::string& path_to_config_file);
    
private:  
    std::unique_ptr<IDatabaseController> database_controller;
    std::unique_ptr<controllers::IModelController> model_controller;
    std::unique_ptr<controllers::IShowPlotController> show_plot_controller;
    std::unique_ptr<controllers::IRegisterController> register_contoller;
    std::unique_ptr<controllers::IAuthorizeController> aurhorize_controller;
    std::unique_ptr<controllers::IPredictController> predict_controller;

    prtToIHandler predict_handler;
    prtToIHandler register_handler;
    prtToIHandler authorize_handler;
    prtToIHandler show_plot_handler;
    prtToIHandler global_router;
    
    std::map<std::string, handlers::IHandler*> handlers_;
    std::vector<std::thread> listeners;
    Config config_;
    handlers::ProtocolAPI protocol_API;
    std::vector<std::string> router_paths;

    void setHandlers(const std::string& header, handlers::IHandler* hendler);
    Config parseConfigFhomFile(const std::string& path_to_config_file);
    handlers::ProtocolAPI parseAPIConfigFhomFile(const std::string& path_to_API_config);
}; 
