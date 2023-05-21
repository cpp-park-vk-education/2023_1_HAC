#include "http_protocol.h"
#include <iostream>

HTTPResponseToBoostAdapter::HTTPResponseToBoostAdapter(httpResponse &response):response_(&response){}

httpResponse HTTPResponseToBoostAdapter::toBoost(){};

void HTTPResponseToBoostAdapter::setStatus(int status_code){
    response_->result(boost::beast::http::status(status_code));
};
void HTTPResponseToBoostAdapter::setHeader(const std::string &header, const std::string &value){
    response_->set(header, value);
};
void HTTPResponseToBoostAdapter::setBody(const std::string &bytes){
    response_->body() = bytes; 
};
std::string HTTPResponseToBoostAdapter::getURL(){};
int HTTPResponseToBoostAdapter::getStatus(){
    return response_->result_int();
};
std::map<std::string, std::string> HTTPResponseToBoostAdapter::getHeaders(){};
std::string HTTPResponseToBoostAdapter::getBoby(){
    return response_->body();
};



HTTPRequestToBoostAdapter::HTTPRequestToBoostAdapter(httpRequest &request): request_(&request){};
HTTPRequestToBoostAdapter::HTTPRequestToBoostAdapter(const std::string &url, 
                                        const std::string &headers, const std::string &body){};
HTTPRequestToBoostAdapter::HTTPRequestToBoostAdapter(const std::string &url, 
                                        const std::string &headers, std::string body){};

void HTTPRequestToBoostAdapter::setStatus(int status_code){};
void HTTPRequestToBoostAdapter::setUrl(const std::string& url){};
void HTTPRequestToBoostAdapter::setHeader(const std::string &header, const std::string &value){};
void HTTPRequestToBoostAdapter::setBody(const std::string &bytes){};
std::string HTTPRequestToBoostAdapter::getURL(){
    return std::string(request_->target());
};
std::map<std::string, std::string> HTTPRequestToBoostAdapter::getHeaders(){
    std::map<std::string, std::string> headers;
    
    const auto& base = request_->base();
        
    headers["method"] = boost::beast::http::to_string(base.method());
    for (const auto& headerField : base)
    {
        std::string name(headerField.name_string().data(), headerField.name_string().size());
        std::string value(headerField.value().data(), headerField.value().size());
        headers[std::move(name)] = std::move(value);
    }
    if (headers.find("Content-Disposition") != headers.end()){
        headers["actions"] = headers["Content-Disposition"];
    } else {
        headers["actions"] = "null";
    }
    
    return headers;
};
std::string HTTPRequestToBoostAdapter::getBoby(){
    return request_->body();
};
void HTTPRequestToBoostAdapter::toIRequest(httpRequest){};
