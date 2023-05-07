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


Server::Server(const std::string& path_to_config_file) {};
void Server::setHandlers(std::map<std::string, handlers::IHandler*> &handlers, 
                        const std::string& header, handlers::IHandler* hendler){};
Server::Config parseConfigFhomFile(const std::string& path_to_config_file){};
handlers::ProtocolAPI Server::parseAPIConfigFhomFile(const std::string& path_to_API_config){};