#include "http_protocol.h"

HTTPResponseToBoostAdapter::HTTPResponseToBoostAdapter(){};

void HTTPResponseToBoostAdapter::setStatus(int status_code){};
void HTTPResponseToBoostAdapter::setHeader(std::string header, std::string value){};
void HTTPResponseToBoostAdapter::setBody(std::vector<char> bytes){};