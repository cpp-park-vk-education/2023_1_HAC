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

class PredictController : public IPredictController {
 public:
    PredictController(const ptrToDBController db_controller, const ptrToModelController model_controller);

    void makePredict(IHTTPRequest_ request, IHTTPResponse_ response) override;

 private:
    Json::Value parseInputHttpRequest(const IHTTPRequest_ request) override;
    Json::Value getPlotDataFromDB(const Json::Value& data_protocol) override;
    TimeSeriesPredicts& makeTimeSeries(const Json::Value& samples_data, size_t window_size) override;
    IHTTPResponse_ parseModelResponse(const IHTTPResponse_ request) override;   

    ptrToDBController db_controller_;
    ptrToModelController model_controller_;
};

class ModelController : public IModelController {
 public:
    explicit ModelController(const ptrToAPIModel api_model);

    IHTTPResponse_ callModelApi(const TimeSeriesPredicts& samples_data) override;

 private:
    IHTTPRequest_ makeHttpForModel(const TimeSeriesPredicts& samples_data) override;

    ptrToAPIModel api_model_;
};


class ShowPlotController : public IShowPlotController {
 public:
    explicit ShowPlotController(const ptrToDBController db_controller);

    IHTTPResponse_ createPlotData(IHTTPRequest_ request) override;

 private:
    Json::Value parseInputHttpRequest(const IHTTPRequest_ request) override;
    Json::Value getPlotDataFromDB(const Json::Value& data_protocol) override;

    ptrToDBController db_controller_;
};


class RegisterController : public IRegisterController {
 public:
    explicit RegisterController(const ptrToDBController db_controller);

    IHTTPResponse_ registration(const IHTTPRequest_ request) override;
 
 private:
    Json::Value parseInputHttpRequest(const IHTTPRequest_ request) override;
    hash_ hashPassword(const std::string& password) override;
    bool postDataRegistrDB(const Json::Value& data_protocol) override;

    ptrToDBController db_controller_;

};


class AuthorizeController : public IAuthorizeController {
 public:
    explicit AuthorizeController(const ptrToDBController db_controller);

    IHTTPResponse_ authorization(const IHTTPRequest_ request) override;
    
 private:
    Json::Value parseInputHttpRequest(const IHTTPRequest_ request) override;
    hash_ hashPassword(const std::string& password) override;
    bool getCheckAuthorData(const Json::Value& data_protocol) override; 

    ptrToDBController db_controller_;

};


class UpdateDataController : public IUpdateDataController {
 public:
    UpdateDataController(const ptrToDBController db_controller, const ptrToAPIStock api_stock);

    bool udateData(const handlers::ProtocolAPI& protocol) = 0;

 private:
    std::string parseToApiRequest(const handlers::ProtocolAPI& protocol) = 0;

    ptrToDBController db_controller_;
    ptrToAPIStock api_stock_;
};



} // namespace controllers 
