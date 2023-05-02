#pragma once // NO_LINT

#include "idbcontroller.h"
#include <json/value.h>
#include "config_handler.h"

#include <string>
#include <memory>

namespace controllers {
 

struct TimeSeriesPredict {
    std::vector<double> matrix_samples;
    size_t rows;
    size_t cols;
};

class IPredictController {
 public:
    virtual httpResponse makePredict(const httpRequest& request) = 0;
 protected:
    virtual DBRequestProtocol& parseInputHttpRequest(const httpRequest& request) = 0;
    virtual Json::Value& getPlotDataFromDB(const DBRequestProtocol& data_protocol) = 0;
    virtual TimeSeriesPredicts& makeTimeSeries(const Json::Value& samples_data, size_t window_size) = 0;
    virtual httpResponse& parseModelResponse(const httpResponse& request) = 0;
};

class IModelController {
 public:
    virtual httpResponse& callModelApi(const TimeSeriesPredicts& samples_data) = 0;
 protected:
    virtual httpRequest& makeHttpForModel(const TimeSeriesPredicts& samples_data) = 0;
};


class IShowPlotController {
 public:
    virtual httpResponse createPlotData(const httpRequest& request) = 0;

 protected:
    virtual DBRequestProtocol& parseInputHttpRequest(const httpRequest& request) = 0;
    virtual Json::Value& getPlotDataFromDB(const DataDBProtocol& data_protocol) = 0;
};


class IRegisterController {
 public:
    virtual httpResponse registration(const httpRequest& request) = 0;
 protected:
    virtual DBRequestProtocol& parseInputHttpRequest(const httpRequest& request) = 0;
    virtual hash_ hashPassword(const std::string& password) = 0;
    virtual bool postDataRegistrDB(const DBRequestProtocol& data_protocol) = 0; 

};


class IAuthorizeController {
 public:
    virtual httpResponse authorization(const httpRequest& request) = 0;
 protected:
    virtual DBRequestProtocol& parseInputHttpRequest(const httpRequest& request) = 0;
    virtual hash_ hashPassword(const std::string& password) = 0;
    virtual bool getCheckAuthorData(const DBRequestProtocol& data_protocol) = 0; 
};


class IUpdateDataController {
 public:
    virtual bool udateData(const ProtocolAPI& protocol) = 0;
 protected:
    virtual std::string parseToApiRequest(const ProtocolAPI& protocol) = 0;

};

} // namespace controllers 
