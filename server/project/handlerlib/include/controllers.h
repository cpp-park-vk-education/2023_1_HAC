#pragma once // NO_LINT

#include "icontrollers.h"
#include "ihandler.h"

namespace controllers {

class PredictController : public IPredictController {
 public:
    PredictController(const ptrToDBController db_controller, const ptrToModelController model_controller);

    httpResponse makePredict(const httpRequest& request) override;

 private:
    DataDBProtocol& parseInputHttpRequest(const httpRequest& request) override;
    Json::Value& getPlotDataFromDB(const DataDBProtocol& data_protocol) override;
    TimeSeriesPredicts& makeTimeSeries(const Json::Value& samples_data, size_t window_size) override;
    httpResponse& parseModelResponse(const httpResponse& request) override;   

    ptrToDBController db_controller_;
    ptrToModelController model_controller_
};

class ModelController : public IModelController {
 public:
    explicit ModelController(const ptrToAPIModel api_model);

    httpResponse& callModelApi(const TimeSeriesPredicts& samples_data) override;

 private:
    httpRequest& makeHttpForModel(const TimeSeriesPredicts& samples_data) override;

    ptrToAPIModel api_model_;
};


class ShowPlotController : public IShowPlotController {
 public:
    explicit ShowPlotController(const ptrToDBController db_controller);

    httpResponse createPlotData(httpRequest request) override;

 private:
    DBRequestProtocol& parseInputHttpRequest(const httpRequest& request) override;
    Json::Value& getPlotDataFromDB(const DataDBProtocol& data_protocol) override;

    ptrToDBController db_controller_;
};


class RegisterController : public IRegisterController {
 public:
    explicit RegisterController(const ptrToDBController db_controller);

    httpResponse registration(const httpRequest& request) override;
 
 private:
    DBRequestProtocol& parseInputHttpRequest(const httpRequest& request) override;
    hash_ hashPassword(const std::string& password) override;
    bool postDataRegistrDB(const DBRequestProtocol& data_protocol) override;

    ptrToDBController db_controller_;

};


class AuthorizeController : public IAuthorizeController {
 public:
    explicit AuthorizeController(const ptrToDBController db_controller);

    httpResponse registration(const httpRequest& request) override;
    
 private:
    DBRequestProtocol& parseInputHttpRequest(const httpRequest& request) override;
    hash_ hashPassword(const std::string& password) override;
    bool getCheckAuthorData(const DBRequestProtocol& data_protocol) override; 

    ptrToDBController db_controller_;

};


class UpdateDataController : public IUpdateDataController {
 public:
    UpdateDataController(const ptrToDBController db_controller, const ptrToApiStock api_stock);

    bool udateData(const ProtocolAPI& protocol) = 0;

 private:
    std::string parseToApiRequest(const ProtocolAPI& protocol) = 0;

    ptrToDBController db_controller_;
    ptrToApiStock api_stock_;
};



} // namespace controllers 
