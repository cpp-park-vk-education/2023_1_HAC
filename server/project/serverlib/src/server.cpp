// #include <iostream>
 #include "server.h"
// #include "listener.h"
// // #include <boost/config.hpp>

using prtToHendler = std::unique_ptr<handlers::IHandler>

Server::Config parseConfigFromFile(const std::string& path_to_config_file) {
    Server::Config config;
    config.address = "127.0.0.1";
    config.port = 12;
    config.threads = 10;
    return config;
}


// Server::Server(const std::string& path_to_config_file) {
    
    // std::usique_ptr<controllers::IDatabaseController> database_controller 
    //                = std::make_unique<controllers::DBController>;
    // std::usique_ptr<controllers::IModelController> model_controller 
    //                = std::make_unique<controllers::ModelController>;
    // std::usique_ptr<controllers::IShowPlotController> show_plot_controller 
    //                = std::make_unique<controllers::ShowPlotController>;
    // std::usique_ptr<controllers::IRegisterController> register_contoller
    //                = std::make_unique<controllers::RegisterController>;
    // std::usique_ptr<controllers::IAuthorizeController> aurhorize_controller
    //                = std::make_unique<controllers::AuthorizeController>;

    // prtToHandler predict_handler = std::make_unique<handlers::PredictHandler>(database_controller);
    // prtToHandler register_handler = std::make_unique<handlers::RegisterHandler>(register_contoller);
    // prtToHandler authorize_handler = std::make_unique<handlers::AuthorizeHandler>(authorize_controller);
    // prtToHandler update_handler = std::make_unique<handlers::UpdateHandler>(update_controller);
    
    // config_ = parseConfigFhomFile(path_to_config_file);


     // router_();
     // listner(&router_);
     
     // boost::asio::io_context ioc_{config_.threads};
 // };
