#pragma once // NO_LINT

#include "ihandler.h"
#include "icontrollers.h"
#include <memory>
#include <map>



namespace handlers {

std::vector<std::string> cutUrlTokens(std::vector<std::string>& tokens, const std::string& error_mess);
std::vector<std::string> splitMessage(const std::string& message, char separator);

using cookie_map = std::map<std::string, std::string>;
using ptrToPredictController = controllers::IPredictController*;
using ptrToShowPlotController = controllers::IShowPlotController*;
using ptrToRegisterController = controllers::IRegisterController*;
using ptrToAuthorizeController = controllers::IAuthorizeController*;
using ptrToModelController = controllers::IModelController*;
using ptrToUpdateDataController = controllers::IUpdateDataController*;
using ptrToMiddleWare = controllers::IMiddleWare*;
using ptrToExitController = controllers::IExitController*;
using ptrToGetStocksController = controllers::IGetStocksController*;
using ptrToChangePasswordController = controllers::IChangePasswordController*;
using ptrToChangeEmailController = controllers::IChangeEmailController*;

class PredictHandler : public IHandler {
 public:
   explicit PredictHandler(ptrToPredictController controller);
   void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;
 private:
   Json::Value parseInputHttpRequest(const std::string& message);
   void makeResponse(IHTTPResponse_ response, const Json::Value& response_json);
   ptrToPredictController controller_;
};


class ShowPlotHandler : public IHandler {
 public:
   explicit ShowPlotHandler(ptrToShowPlotController controller);
   void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;
 private:
   Json::Value parseInputHttpRequest(const std::string& message);
   void makeResponse(IHTTPResponse_ response, const Json::Value& response_json);

   ptrToShowPlotController controller_;  
};

class RegisterHandler : public IHandler {
 public:
   explicit RegisterHandler(ptrToRegisterController controller);
   void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;
 private:
   Json::Value parseInputHttpRequest(const std::string& message);
   void makeResponse(IHTTPResponse_ response, const Json::Value& response_json);

   ptrToRegisterController controller_;  
};


class AuthorizeHandler : public IHandler {
 public:
   explicit AuthorizeHandler(ptrToAuthorizeController controller); 
   void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;
 private:
   Json::Value parseInputHttpRequest(const std::string& message);
   void makeResponse(IHTTPResponse_ response, const Json::Value& response_json);
   ptrToAuthorizeController controller_; 
};


class ExitHandler : public IHandler {
 public:
  explicit ExitHandler(ptrToExitController controller); 
  void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;
 private:
  Json::Value parseInputHttpRequest(const std::string& message);
  void makeResponse(IHTTPResponse_ response, const Json::Value& response_json);

  ptrToExitController controller_; 
};

class GetStocksHandler : public IHandler {
 public:
  explicit GetStocksHandler(ptrToGetStocksController controller); 
  void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;
 private:
  void makeResponse(IHTTPResponse_ response, const Json::Value& response_json);

  ptrToGetStocksController controller_; 
};


class ChangePasswordHandler : public IHandler {
 public:
  explicit ChangePasswordHandler(ptrToChangePasswordController controller); 
  void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;
 private:
  Json::Value parseInputHttpRequest(const std::string& message);
  void makeResponse(IHTTPResponse_ response, const Json::Value& response_json);

  ptrToChangePasswordController controller_; 
};

class ChangeEmailHandler : public IHandler {
 public:
  explicit ChangeEmailHandler(ptrToChangeEmailController controller); 
  void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;
 private:

  Json::Value parseInputHttpRequest(const std::string& message);
  void makeResponse(IHTTPResponse_ response, const Json::Value& response_json);

  ptrToChangeEmailController controller_; 
};


class Router : public IHandler {
 public:
  Router(const std::map<std::string, IHandler*>& handlers, ptrToMiddleWare middleware);
  void handle(IHTTPRequest_ request, IHTTPResponse_ response) override;

 private:
  std::map<std::string, IHandler*> handlers_;
  ptrToMiddleWare middleware_;
};


} // namespace handlers 