#include "api_stock.h"

api::APIStockRequest::APIStockRequest(){};
Json::Value api::APIStockRequest::getData(const handlers::ProtocolAPI& protocol){};

void api::APIStockRequest::doConnect(std::string path){};
IHTTPResponse* api::APIStockRequest::onConnect(ssl::stream<tcp::socket> stream){};
