// #include <iostream>
#include "server.h"
// #include "listener.h"
// // #include <boost/config.hpp>


Config Server::parseConfigFhomFile(const std::string& path_to_config_file) {
    Config config;
    config.address = "127.0.0.1";
    config.port = 8889;
    config.threads = 1;
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
    // std::unique_ptr<IRouterAdapter> router_adapter = std::make_unique<RouterAdapter>
/*
    config_ = Server::parseConfigFhomFile(path_to_config_file);
    net::io_context ioc{config_.threads};    

    std::make_shared<Listener>(
       ioc,
       tcp::endpoint{net::ip::make_address(config_.address), config_.port})->Run();*/

    //router_();
    //listner(&router_);

    auto const address = net::ip::make_address("0.0.0.0");
    auto const port = static_cast<unsigned short>(8080);
    auto const doc_root = std::make_shared<std::string>(".");
    auto const threads = std::max<int>(1, std::atoi("1"));

    // The io_context is required for all I/O
    net::io_context ioc{threads};

    // Create and launch a listening port
    std::make_shared<Listener>(
        ioc,
        tcp::endpoint{address, port},
        doc_root)->run();

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i)
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
