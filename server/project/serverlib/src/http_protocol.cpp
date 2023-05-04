#include "http_protocol.h"

HTTPResponseToBoostAdapter::HTTPResponseToBoostAdapter(){};
httpResponse HTTPResponseToBoostAdapter::toBoost(){};

void HTTPResponseToBoostAdapter::setStatus(int status_code){};
void HTTPResponseToBoostAdapter::setHeader(std::string header, std::string value){};
void HTTPResponseToBoostAdapter::setBody(std::vector<char> bytes){};

HTTPRequestToBoostAdapter::HTTPRequestToBoostAdapter(){};
HTTPRequestToBoostAdapter::HTTPRequestToBoostAdapter(std::string url, 
                                        std::string headers, std::string body){};
HTTPRequestToBoostAdapter::HTTPRequestToBoostAdapter(std::string url, 
                                        std::string headers, std::vector<char> body){};
std::string HTTPRequestToBoostAdapter::getURL(){};
std::map<std::string, std::string> HTTPRequestToBoostAdapter::getHeaders(){};
std::vector<char> HTTPRequestToBoostAdapter::getBoby(){};