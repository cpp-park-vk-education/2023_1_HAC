#include "api_model.h"

api::APIModelRequest::APIModelRequest(){};
IHTTPResponse* api::APIModelRequest::getData(IHTTPRequest* req){};

void api::APIModelRequest::doConnect(std::string path){};
IHTTPResponse* api::APIModelRequest::onConnect(ssl::stream<tcp::socket> stream){};