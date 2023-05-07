#include "api_stock.h"

api::APIStockRequest::APIStockRequest(){};
IHTTPResponse* api::APIStockRequest::getData(std::string path){};

void api::APIStockRequest::doConnect(std::string path){};
IHTTPResponse* api::APIStockRequest::onConnect(ssl::stream<tcp::socket> stream){};
