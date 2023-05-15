#include "api_model.h"

api::APIModelRequest::APIModelRequest(){};
Json::Value api::APIModelRequest::getData(const controllers::TimeSeriesPredicts& samples_data){};

void api::APIModelRequest::doConnect(std::string path){};
IHTTPResponse* api::APIModelRequest::onConnect(ssl::stream<tcp::socket> stream){};