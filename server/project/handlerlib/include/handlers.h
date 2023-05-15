#pragma once // NO_LINT

#include "ihandler.h"
#include "icontrollers.h"
#include <memory>
#include <map>

namespace handlers {

void cutUrlTokens(std::vector<std::string>& tokens, const std::string& error_mess);
std::vector<std::string> splitMessage(const std::string& message, char separator);


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
   Json::Value parseInputHttpRequest(const std::string& message) override;
   void makeResponse(IHTTPResponse_ response, const Json::Value& response_json) override;
   ptrToPredictController controller_;
};


class ShowPlotHandler : public IHandler {
 public:
   explicit ShowPlotHandler(ptrToShowPlotController controller);
   void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;
 private:
   Json::Value parseInputHttpRequest(const std::string& message) override;
   void makeResponse(IHTTPResponse_ response, const Json::Value& response_json) override;

   ptrToShowPlotController controller_;  
};

class RegisterHandler : public IHandler {
 public:
   explicit RegisterHandler(ptrToRegisterController controller);
   void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;
 private:
   Json::Value parseInputHttpRequest(const std::string& message) override;
   void makeResponse(IHTTPResponse_ response, const Json::Value& response_json) override;

   ptrToRegisterController controller_;  
};


class AuthorizeHandler : public IHandler {
 public:
   explicit AuthorizeHandler(ptrToAuthorizeController controller); 
   void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;
 private:

   Json::Value parseInputHttpRequest(const std::string& message) override;
   void makeResponse(IHTTPResponse_ response, const Json::Value& response_json) override;
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