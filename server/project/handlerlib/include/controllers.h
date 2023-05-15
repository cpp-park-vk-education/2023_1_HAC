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


Json::Value makeJsonError(const std::string& error_mes);
Json::Value makeDBProtocolGetTS(const Json::Value& request);
hash_ hashPassword(const std::string& password);


// class PredictController
class PredictController : public IPredictController {
 public:
   PredictController(const ptrToDBController db_controller, const ptrToModelController model_controller);

   Json::Value makePredict(const Json::Value& request) override;
   

 private:
    TimeSeriesPredicts makeTimeSeries(const std::vector<double>& samples_data, size_t window_size) override;
    
    std::vector<double> parseDBProtocol(const Json::Value& response) override;

    ptrToDBController db_controller_;
    ptrToModelController model_controller_;
};

// class ModelController
class ModelController : public IModelController {
 public:
  explicit ModelController(api::IAPIModelRequest* api_model){};
  Json::Value callModelApi(const TimeSeriesPredicts& samples_data) override;

 private:
  api::IAPIModelRequest* api_model_;
};

// class ShowPlotController 
class ShowPlotController : public IShowPlotController {
 public:
   explicit ShowPlotController(const ptrToDBController db_controller);

   Json::Value createPlotData(const Json::Value& request) override;

 private:
   ptrToDBController db_controller_;
};

// class RegisterController
class RegisterController : public IRegisterController {
 public:
  explicit RegisterController(const ptrToDBController db_controller);

  Json::Value registration(const Json::Value& request) override; 
 private:
  Json::Value makeDBProtocol(const Json::Value& request) override;

  ptrToDBController db_controller_;
};

// class AuthorizeController
class AuthorizeController : public IAuthorizeController {
 public:
  explicit AuthorizeController(const ptrToDBController db_controller);

  Json::Value authorization(const Json::Value& request) override;
 private:
  Json::Value checkPassword(const Json::Value& db_response, const Json::Value& request) override;
  Json::Value makeDBProtocol(const Json::Value& request) override;

  ptrToDBController db_controller_;
};

// class UpdateDataController
class UpdateDataController : public IUpdateDataController {
 public:
  UpdateDataController(const ptrToDBController db_controller, const ptrToAPIStock api_stock);

  bool updateData(const handlers::ProtocolAPI& protocol) override;

 private:
  // Json::Value parseHTTPToJson(IHTTPResponse_ response) override;
  Json::Value makeDBProtocol(const Json::Value& request) override;

  ptrToDBController db_controller_;
  ptrToAPIStock api_stock_;
};

} // namespace controllers 
