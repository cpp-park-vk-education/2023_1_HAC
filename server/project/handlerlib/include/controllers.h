#pragma once // NO_LINT
#include "handlers.h"
#include "icontrollers.h"
#include "api_model.h"
#include "api_stock.h"
#include "config_handler.h"
#include "dbcontroller.hpp"


namespace controllers {

using namespace api;

using ptrToDBController = dbcontroller::IDataBaseController*;
using ptrToModelController = controllers::IModelController*;
using ptrToAPIModel = api::IAPIModelRequest*;

// PROTOCOL AND ERROR
Json::Value makeJsonError(const std::string& error_mes, bool server_error);
Json::Value makeProtocolSendCookie(const std::string& cookie, const std::string& login);
Json::Value makeProtocolAuthorization(const Json::Value& request);

// SUPPORT FUNC
hash_ hashing(const std::string& buffer);
std::string makeCookie();
bool isNighttime(int hour);
bool isWeekend(int day_of_week);

// class PredictController
class PredictController : public IPredictController {
 public:
   PredictController(const ptrToDBController db_controller, const ptrToModelController model_controller);
   
   Json::Value makePredict(const Json::Value& request) override;
   

 private:
    TimeSeriesPredicts makeTimeSeries(const std::vector<double>& samples_data, size_t lenpredict, const std::string& name_stock);
    Json::Value makeDBProtocol(const Json::Value& request);
    std::vector<double> parseDBProtocol(const Json::Value& response);
    std::vector<std::string> makeDateSequence(const std::string& start_time, size_t cnt_hours);
    ptrToDBController db_controller_;
    ptrToModelController model_controller_;
};

// class ModelController
class ModelController : public IModelController {
 public:
  explicit ModelController(ptrToAPIModel api_model);
  Json::Value callModelApi(const TimeSeriesPredicts& samples_data) override;

 private:
  ptrToAPIModel api_model_;
};

// class ShowPlotController 
class ShowPlotController : public IShowPlotController {
 public:
  explicit ShowPlotController(const ptrToDBController db_controller);

  Json::Value createPlotData(const Json::Value& request) override;

 private:
  Json::Value makeDBProtocol(const Json::Value& request);

  ptrToDBController db_controller_;
};

// class RegisterController
class RegisterController : public IRegisterController {
 public:
  explicit RegisterController(const ptrToDBController db_controller);

  Json::Value registration(Json::Value& request) override; 
 private:
  Json::Value makeDBProtocol(const Json::Value& request);

  ptrToDBController db_controller_;
};

// class AuthorizeController
class AuthorizeController : public IAuthorizeController {
 public:
  explicit AuthorizeController(const ptrToDBController db_controller);

  Json::Value authorization(Json::Value& request) override;
 private:
  Json::Value checkPassword(const Json::Value& db_response, const Json::Value& request);


  ptrToDBController db_controller_;
};

// class UpdateDataController
class UpdateDataController : public IUpdateDataController {
 public:
  UpdateDataController(const ptrToDBController db_controller, const ptrToAPIStock api_stock);

  bool updateData(const handlers::ProtocolAPI& protocol) override;

 private:
  Json::Value makeDBProtocol(const Json::Value& request);

  ptrToDBController db_controller_;
  ptrToAPIStock api_stock_;
};

// class ExitController
class ExitController : public IExitController {
 public:
  explicit ExitController(const ptrToDBController db_controller);
  Json::Value deleteCookie(const Json::Value& request) override;
 private:
  Json::Value makeDBProtocol(const Json::Value& request);

  ptrToDBController db_controller_;
};



// class ChangeEmailController
class ChangeEmailController : public IChangeEmailController {
 public:
  explicit ChangeEmailController(const ptrToDBController db_controller);
  Json::Value changeEmail(const Json::Value& request) override;
 private:
  Json::Value makeDBProtocol(const Json::Value& request);

  ptrToDBController db_controller_;
};



// class ChangePasswordController
class ChangePasswordController : public IChangePasswordController {
 public:
  explicit ChangePasswordController(const ptrToDBController db_controller);
  Json::Value changePassword(const Json::Value& request) override;
 private:
  Json::Value makeDBProtocol(const Json::Value& request);

  ptrToDBController db_controller_;
};


// GetStocksController
class GetStocksController : public IGetStocksController {
 public:
  explicit GetStocksController(const ptrToDBController db_controller);
  Json::Value getNameStocks() override;
 private:
  Json::Value makeDBProtocol();

  ptrToDBController db_controller_;
};


// class MiddleWare
class MiddleWare : public IMiddleWare {
 public:
  explicit MiddleWare(const ptrToDBController db_controller);
  
  cookie_map checkCookieFile(const std::string& cookie) override;
 private:
  Json::Value makeDBProtocol(const std::string& cookie);
  cookie_map paseDBResponse(const Json::Value& response, const std::string& cookie);

  ptrToDBController db_controller_;
};


} // namespace controllers 
