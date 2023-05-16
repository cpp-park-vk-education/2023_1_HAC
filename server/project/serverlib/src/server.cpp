// #include <iostream>
#include "server.h"
// #include "listener.h"
// // #include <boost/config.hpp>


Config Server::parseConfigFhomFile(const std::string& path_to_config_file) {
    Config config;
    config.address = "0.0.0.0";
    config.port = 8080;
    config.threads = 1;
    return config;
}

Server::Server(const std::string& path_to_config_file) {
    std::unique_ptr<api::IAPIModelRequest> api_model 
                  = std::make_unique<api::APIModelRequest>();
            
    controllers::TimeSeriesPredicts samples;
    //api_model->getData(samples);
    

    std::unique_ptr<dbcontroller::IDataBaseController> database_controller 
                   = std::make_unique<dbcontroller::DataBaseController>();

    if(!database_controller->ConnectToDatabase()) {
        std::cerr << "DB is not open" << std::endl;
        exit(1);
    }
                
    std::unique_ptr<controllers::IShowPlotController> show_plot_controller 
                    = std::make_unique<controllers::ShowPlotController>(database_controller.get());
    std::unique_ptr<controllers::IRegisterController> register_contoller
                     = std::make_unique<controllers::RegisterController>(database_controller.get());
    std::unique_ptr<controllers::IAuthorizeController> aurhorize_controller
                    = std::make_unique<controllers::AuthorizeController>(database_controller.get());
    std::unique_ptr<controllers::IModelController> model_controller 
                   = std::make_unique<controllers::ModelController>(api_model.get());
    std::unique_ptr<controllers::IPredictController> predict_controller 
                   = std::make_unique<controllers::PredictController>(database_controller.get(), model_controller.get());

    prtToIHandler predict_handler = std::make_unique<handlers::PredictHandler>(predict_controller.get());
    prtToIHandler register_handler = std::make_unique<handlers::RegisterHandler>(register_contoller.get());
    prtToIHandler authorize_handler = std::make_unique<handlers::AuthorizeHandler>(aurhorize_controller.get());
    prtToIHandler show_plot_handler = std::make_unique<handlers::ShowPlotHandler>(show_plot_controller.get());
    

    // Переделать с чтением из конфига
    handlers_.insert(std::pair<std::string, handlers::IHandler*>("POST:AUTHORIZATION", authorize_handler.get()));
    handlers_.insert(std::pair<std::string, handlers::IHandler*>("POST:REGISTRATION", register_handler.get()));
    handlers_.insert(std::pair<std::string, handlers::IHandler*>("GET:PREDICT", predict_handler.get()));
    handlers_.insert(std::pair<std::string, handlers::IHandler*>("GET:PLOT", show_plot_handler.get()));

    prtToIHandler global_router = std::make_unique<handlers::Router>(handlers_);

    std::unique_ptr<IRouterAdapter> router_adapter = std::make_unique<RouterAdapter>(global_router.get());


    config_ = Server::parseConfigFhomFile(path_to_config_file);
    net::io_context ioc{config_.threads};

    // Create and launch a listening port
    std::make_shared<Listener>(
        ioc,
        tcp::endpoint{net::ip::make_address(config_.address), config_.port},router_adapter.get())->run();

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(config_.threads - 1);
    for(auto i = config_.threads - 1; i > 0; --i)
        v.emplace_back(
        [&ioc]
        {
            ioc.run();
        });
    ioc.run();

};
void Server::setHandlers(std::map<std::string, handlers::IHandler*> &handlers, 
                        const std::string& header, handlers::IHandler* hendler) {
    //handlers.insert(std::make_pair(header, header));
};
//Server::Config parseConfigFhomFile(const std::string& path_to_config_file){};
handlers::ProtocolAPI Server::parseAPIConfigFhomFile(const std::string& path_to_API_config){};
