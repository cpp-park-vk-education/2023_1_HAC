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
#include "listener.h"
#include "routers.h"

#include "ihandler.h"
#include "icontrollers.h"
#include "handlers.h"
#include "controllers.h"
#include "config_handler.h"

using prtToIHandler = std::unique_ptr<handlers::IHandler>

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
    std::usique_ptr<controllers::IDatabaseController> database_controller 
    std::usique_ptr<controllers::IModelController> model_controller 
    std::usique_ptr<controllers::IShowPlotController> show_plot_controller 
    std::usique_ptr<controllers::IRegisterController> register_contoller
    std::usique_ptr<controllers::IAuthorizeController> aurhorize_controller

    prtToIHandler predict_handler;
    prtToIHandler register_handler;
    prtToIHandler authorize_handler;
    prtToIHandler update_handler;
    prtToIHandler router;
    
    std::usique_ptr<routers::IRouterAdapter> router_adapter;
    std::shared_ptr<IListener> lister;
    net::io_context ioc
    std::map<string, *IHandler> handlers;
    std::vector<std::thread> listeners;
    Config config_;
    handlers::ProtocolAPI protocol_API;

    void setHandlers();
    Config parseConfigFhomFile(const std::string& path_to_config_file);
    handlers::ProtocolAPI parseAPIConfigFhomFile(const std::string& path_to_API_config);
}; 
