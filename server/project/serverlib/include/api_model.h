#pragma once //NO_LINT
#include "http_protocol.h"

#include "ihandler.h"
#include "icontrollers.h"
#include "handlers.h"
#include "controllers.h"
#include "config_handler.h"

class IAPIModelRequest{
 public:
    virtual IHTTPResponse getData(IHTTPRequest req) = 0;
}

class APIModelelRequest{
 public:
    APIModelelRequest();
    IHTTPResponse getData(IHTTPRequest req) override;
 private:
    void doConnect(std::string path);
    IHTTPResponse onConnect(ssl::stream<tcp::socket> stream);   
}