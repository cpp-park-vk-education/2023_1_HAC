// #include <iostream>
#include "server.h"
// #include "listener.h"
// // #include <boost/config.hpp>


Server::Config parseConfigFromFile(const std::string& path_to_config_file) {
    Server::Config config;
    config.address = "127.0.0.1";
    config.port = 12;
    config.threads = 10;
    return config;
}

Server::Server(const std::string& path_to_config_file) {
    std::unique_ptr<api::IAPIModelRequest> api_model 
                  = std::make_unique<api::APIModelRequest>();

    std::unique_ptr<dbcontroller::IDataBaseController> database_controller 
                   = std::make_unique<dbcontroller::DataBaseController>();
    std::unique_ptr<controllers::IShowPlotController> show_plot_controller 
                    = std::make_unique<controllers::ShowPlotController>(database_controller.get());
    // std::unique_ptr<controllers::IRegisterController> register_contoller
    //                 = std::make_unique<controllers::RegisterController>(database_controller.get());
    // std::unique_ptr<controllers::IAuthorizeController> aurhorize_controller
    //                 = std::make_unique<controllers::AuthorizeController>(database_controller.get());
    // std::unique_ptr<controllers::IModelController> model_controller 
    //                = std::make_unique<controllers::ModelController>(api_model.get());
    // std::unique_ptr<controllers::IPredictController> predict_controller 
    //                = std::make_unique<controllers::PredictController>(database_controller.get(), model_controller.get());

    // prtToIHandler predict_handler = std::make_unique<handlers::PredictHandler>(predict_controller.get());
    // prtToIHandler register_handler = std::make_unique<handlers::RegisterHandler>(register_contoller.get());
    // prtToIHandler authorize_handler = std::make_unique<handlers::AuthorizeHandler>(aurhorize_controller.get());
    // prtToIHandler show_plot_handler = std::make_unique<handlers::ShowPlotHandler>(show_plot_controller.get());
    
    // config_ = parseConfigFhomFile(path_to_config_file);

    // router_();
    // listner(&router_);
    //  
    //  boost::asio::io_context ioc_{config_.threads};



};
void Server::setHandlers(std::map<std::string, handlers::IHandler*> &handlers, 
                        const std::string& header, handlers::IHandler* hendler) {
    //handlers.insert(std::make_pair(header, header));
};
Server::Config parseConfigFhomFile(const std::string& path_to_config_file){};
handlers::ProtocolAPI Server::parseAPIConfigFhomFile(const std::string& path_to_API_config){};