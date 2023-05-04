#pragma once // NO_LINT

#include <jsoncpp/json/value.h>
#include "config_handler.h"
#include "http_protocol.h"

#include <string>
#include <memory>


#include <string>
using DataDBProtocol = std::string;
using hash_ = std::string;

namespace controllers {
 

struct TimeSeriesPredicts {
    std::vector<double> matrix_samples;
    size_t rows;
    size_t cols;
};

class IPredictController {
 public:
    virtual void makePredict(IHTTPRequest_ request, IHTTPResponse_ response) = 0;
 protected:
    virtual DBRequestProtocol& parseInputHttpRequest(const IHTTPRequest_ request) = 0;
    virtual Json::Value& getPlotDataFromDB(const DBRequestProtocol& data_protocol) = 0;
    virtual TimeSeriesPredicts& makeTimeSeries(const Json::Value& samples_data, size_t window_size) = 0;
    virtual IHTTPResponse_ parseModelResponse(const IHTTPResponse_ request) = 0;
};

class IModelController {
 public:
    virtual IHTTPResponse_ callModelApi(const TimeSeriesPredicts& samples_data) = 0;
 protected:
    virtual IHTTPRequest_ makeHttpForModel(const TimeSeriesPredicts& samples_data) = 0;
};


class IShowPlotController {
 public:
    virtual IHTTPResponse_ createPlotData(const IHTTPRequest_ request) = 0;

 protected:
    virtual DBRequestProtocol& parseInputHttpRequest(const IHTTPRequest_ request) = 0;
    virtual Json::Value& getPlotDataFromDB(const DBRequestProtocol& data_protocol) = 0;
};


class IRegisterController {
 public:
    virtual IHTTPResponse_ registration(const IHTTPRequest_ request) = 0;
 protected:
    virtual DBRequestProtocol& parseInputHttpRequest(const IHTTPRequest_ request) = 0;
    virtual hash_ hashPassword(const std::string& password) = 0;
    virtual bool postDataRegistrDB(const DBRequestProtocol& data_protocol) = 0; 

};


class IAuthorizeController {
 public:
    virtual IHTTPResponse_ authorization(const IHTTPRequest_ request) = 0;
 protected:
    virtual DBRequestProtocol& parseInputHttpRequest(const IHTTPRequest_ request) = 0;
    virtual hash_ hashPassword(const std::string& password) = 0;
    virtual bool getCheckAuthorData(const DBRequestProtocol& data_protocol) = 0; 
};


class IUpdateDataController {
 public:
    virtual bool udateData(const handlers::ProtocolAPI& protocol) = 0;
 protected:
    virtual std::string parseToApiRequest(const handlers::ProtocolAPI& protocol) = 0;

};

} // namespace controllers 
