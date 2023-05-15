#pragma once // NO_LINT

#include "http_protocol.h"
#include <jsoncpp/json/value.h>

using IHTTPRequest_ = IHTTPRequest*;
using IHTTPResponse_ = IHTTPResponse*;


namespace handlers {

class IHandler {
 public:
    virtual void handle(IHTTPRequest_ request, IHTTPResponse_ response) = 0;
 private:
    virtual Json::Value parseInputHttpRequest(const std::string& message) = 0;
    virtual void makeResponse(IHTTPResponse_ response, const Json::Value& response_json) = 0;
};


} // namespace handlers
