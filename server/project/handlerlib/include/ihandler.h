#pragma once // NO_LINT

#include "http_protocol.h"

using IHTTPRequest_ = IHTTPRequest*;
using IHTTPResponse_ = IHTTPResponse*;


namespace handlers {

class IHandler {
 public:
    virtual void handle(IHTTPRequest_ request, IHTTPResponse_ response) = 0;
 private:
};


} // namespace handlers
