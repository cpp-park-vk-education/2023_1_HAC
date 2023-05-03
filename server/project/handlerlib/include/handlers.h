#pragma once // NO_LINT

#include "ihandler.h"
#include "icontroller.h"
#include <memory>

namespace handlers {

using ptrToPredictController = *controllers::IPredictController;
using ptrToShowPlotController = *controllers::IShowPlotController;
using ptrToRegisterController = *controllers::RegisterController;
using ptrToAuthorizeController = *controllers::AuthorizeController;
using ptrToModelController = *controllers::IModelController;
using ptrToUpdateDataController = *controllers::IUpdateDataController;

class PredictHandler : public IHandler {
 public:
    explicit PredictHandler(ptrToUpdateDataController controller);
    void handle(IHTTPRequest request, IHTTPResponse response) override;
 private:
    ptrToPredictController controller_;
};


class ShowPlotHandler : public IHandler {
 public:
    explicit ShowPlotHandler(ptrToShowPlotController controller);
    void handle(IHTTPRequest request, IHTTPResponse response) override;
 private:
    ptrToShowPlotController controller_;  
};

class RegisterHandler : public IHandler {
 public:
    explicit RegisterHandler(ptrToRegisterController controller);
    void handle(IHTTPRequest request, IHTTPResponse response) override;
 private:
    ptrToRegisterController controller_;  
};


class AuthorizeHandler : public IHandler {
 public:
    explicit AuthorizeHandler(ptrToAuthorizeController controller);
    void handle(IHTTPRequest request, IHTTPResponse response) override;
 private:
    ptrToAuthorizeController controller_; 
};


} // namespace handlers 