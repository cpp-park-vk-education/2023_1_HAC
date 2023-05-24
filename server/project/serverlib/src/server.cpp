#include "server.h"
#include "logger.h"

Config Server::parseConfigFhomFile(const std::string& path_to_config_file) {
    std::ifstream config_stream (path_to_config_file);
    if (!config_stream) {
        throw market_mentor::InvalidServerConfig("Wrong config path");
    }
    std::string address, port, threads;
    config_stream >> address >> port >> threads;
    if (!address.length() || !port.length() || !threads.length()) {
        throw market_mentor::InvalidServerConfig("Invalid config data");
    }  
    Config config;
    try {
        config.address = address;
        config.port = stoi(port);
        config.threads = stoi(threads);
    } 
    catch(std::invalid_argument){
        throw market_mentor::InvalidServerConfig("Invalid config data");
    } 
    return config;
}

Server::Server(const std::string& path_to_config_file) {
    
    std::unique_ptr<api::IAPIModelRequest> api_model 
                   = std::make_unique<api::APIModelRequest>();
    std::unique_ptr<api::IAPIStockRequest> api_stock
                   = std::make_unique<api::APIStockRequest>();
    std::unique_ptr<dbcontroller::IDataBaseController> database_controller 
                   = std::make_unique<dbcontroller::DataBaseController>();

    if (!api_model || !database_controller) {
        throw market_mentor::CreatingNullptr("Creating server error");
    }

    try {
        database_controller->ConnectToDatabase();
    } catch(...){
        throw market_mentor::ConnectDatabaseException();
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
    std::unique_ptr<controllers::IUpdateDataController> update_controller 
                   = std::make_unique<controllers::UpdateDataController>(database_controller.get(), api_stock.get());
    std::unique_ptr<controllers::IMiddleWare> middleware_controller 
                   = std::make_unique<controllers::MiddleWare>(database_controller.get());
    std::unique_ptr<controllers::IExitController> exit_controller 
                   = std::make_unique<controllers::ExitController>(database_controller.get());

    if (!show_plot_controller || !register_contoller || !aurhorize_controller || !model_controller || !predict_controller) {
        throw market_mentor::CreatingNullptr("Creating server error");
    }

    std::thread jobThread([update_controller = std::move(update_controller)]() {
        getNewDataByTimer(update_controller.get());
    });

    prtToIHandler predict_handler = std::make_unique<handlers::PredictHandler>(predict_controller.get());
    prtToIHandler register_handler = std::make_unique<handlers::RegisterHandler>(register_contoller.get());
    prtToIHandler authorize_handler = std::make_unique<handlers::AuthorizeHandler>(aurhorize_controller.get());
    prtToIHandler show_plot_handler = std::make_unique<handlers::ShowPlotHandler>(show_plot_controller.get());
    prtToIHandler exit_handler = std::make_unique<handlers::ExitHandler>(exit_controller.get());
    
    if (!predict_handler || !register_handler || !authorize_handler || !show_plot_handler || !exit_handler) {
        throw market_mentor::CreatingNullptr("Creating server error");
    }

    setHandlers("POST:AUTHORIZATION", authorize_handler.get());
    setHandlers("POST:REGISTRATION",  register_handler.get());
    setHandlers("POST:DELETECOOKIE",  exit_handler.get());
    setHandlers("GET:PREDICT",  predict_handler.get());
    setHandlers("GET:PLOT",  show_plot_handler.get());
    setHandlers("POST:CHECKCOOKIE", nullptr);

    prtToIHandler global_router = std::make_unique<handlers::Router>(handlers_, middleware_controller.get());

    std::unique_ptr<IRouterAdapter> router_adapter = std::make_unique<RouterAdapter>(global_router.get());

    if (!global_router || !router_adapter){
        throw market_mentor::CreatingNullptr("Creating server error");
    }
    FileLogger& logger = FileLogger::getInstance();   
    config_ = Server::parseConfigFhomFile(path_to_config_file); 
    net::io_context ioc{config_.threads};

    std::make_shared<Listener>(
        ioc,
        tcp::endpoint{net::ip::make_address(config_.address), config_.port},router_adapter.get())->run();

    logger.log("Server started with params: ip " + config_.address + " port " + std::to_string(config_.port) + " threads " + std::to_string(config_.threads));
    std::vector<std::thread> v;
    v.reserve(config_.threads - 1);
    for(auto i = config_.threads - 1; i > 0; --i)
        v.emplace_back(
        [&ioc]
        {
            ioc.run();
        });
    ioc.run();
    jobThread.join(); 
};
void Server::setHandlers(const std::string& header, handlers::IHandler* hendler) {
    handlers_.insert(std::pair<std::string, handlers::IHandler*>(header, hendler));
};
handlers::ProtocolAPI Server::parseAPIConfigFhomFile(const std::string& path_to_API_config){};
