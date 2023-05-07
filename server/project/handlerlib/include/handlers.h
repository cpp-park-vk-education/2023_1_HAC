#pragma once // NO_LINT

#include "ihandler.h"
#include "icontrollers.h"
#include <memory>
#include <map>

namespace handlers {

using ptrToPredictController = controllers::IPredictController*;
using ptrToShowPlotController = controllers::IShowPlotController*;
using ptrToRegisterController = controllers::IRegisterController*;
using ptrToAuthorizeController = controllers::IAuthorizeController*;
using ptrToModelController = controllers::IModelController*;
using ptrToUpdateDataController = controllers::IUpdateDataController*;

class PredictHandler : public IHandler {
 public:
    explicit PredictHandler(ptrToPredictController controller);
    void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;
 private:
    ptrToPredictController controller_;
};


class ShowPlotHandler : public IHandler {
 public:
    explicit ShowPlotHandler(ptrToShowPlotController controller);
    void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;
 private:
    ptrToShowPlotController controller_;  
};

class RegisterHandler : public IHandler {
 public:
    explicit RegisterHandler(ptrToRegisterController controller);
    void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;
 private:
    ptrToRegisterController controller_;  
};


class AuthorizeHandler : public IHandler {
 public:
    explicit AuthorizeHandler(ptrToAuthorizeController controller);
    void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;
 private:
    ptrToAuthorizeController controller_; 
};

class Router : public IHandler {
 public:
   explicit Router(const std::map<std::string, IHandler*>& handlers);
   void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;

 private:
   std::map<std::string, IHandler*> handlers_;
};




} // namespace handlers 