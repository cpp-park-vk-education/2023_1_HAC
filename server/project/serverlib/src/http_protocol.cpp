#include "http_protocol.h"

HTTPResponseToBoostAdapter::HTTPResponseToBoostAdapter(){};
httpResponse HTTPResponseToBoostAdapter::toBoost(){};

void HTTPResponseToBoostAdapter::setStatus(int status_code){};
void HTTPResponseToBoostAdapter::setHeader(const std::string &header, const std::string &value){};
void HTTPResponseToBoostAdapter::setBody(std::string bytes){};
std::string HTTPResponseToBoostAdapter::getURL(){};
int HTTPResponseToBoostAdapter::getStatus(){};
std::map<std::string, std::string> HTTPResponseToBoostAdapter::getHeaders(){};
std::string HTTPResponseToBoostAdapter::getBoby(){};

HTTPRequestToBoostAdapter::HTTPRequestToBoostAdapter(){};
HTTPRequestToBoostAdapter::HTTPRequestToBoostAdapter(const std::string &url, 
                                        const std::string &headers, const std::string &body){};
HTTPRequestToBoostAdapter::HTTPRequestToBoostAdapter(const std::string &url, 
                                        const std::string &headers, std::string body){};

void HTTPRequestToBoostAdapter::setStatus(int status_code){};
void HTTPRequestToBoostAdapter::setUrl(const std::string& url){};
void HTTPRequestToBoostAdapter::setHeader(const std::string &header, const std::string &value){};
void HTTPRequestToBoostAdapter::setBody(std::string bytes){};
int HTTPRequestToBoostAdapter::getStatus(){};
std::string HTTPRequestToBoostAdapter::getURL(){};
std::map<std::string, std::string> HTTPRequestToBoostAdapter::getHeaders(){};
std::string HTTPRequestToBoostAdapter::getBoby(){};
void HTTPRequestToBoostAdapter::toIRequest(httpRequest){};
