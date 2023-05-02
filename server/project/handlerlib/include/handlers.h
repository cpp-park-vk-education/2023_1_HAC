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
    httpRequest handle(httpRequest request) override;
 private:
    ptrToPredictController controller_;
};


class ShowPlotHandler : public IHandler {
 public:
    explicit ShowPlotHandler(ptrToShowPlotController controller);
    httpRequest handle(httpRequest request) override;
 private:
    ptrToShowPlotController controller_;  
};

class RegisterHandler : public IHandler {
 public:
    explicit RegisterHandler(ptrToRegisterController controller);
    httpRequest handle(httpRequest request) override;
 private:
    ptrToRegisterController controller_;  
};


class AuthorizeHandler : public IHandler {
 public:
    explicit AuthorizeHandler(ptrToAuthorizeController controller);
    httpRequest handle(httpRequest request) override;
 private:
    ptrToAuthorizeController controller_; 
};


} // namespace handlers 