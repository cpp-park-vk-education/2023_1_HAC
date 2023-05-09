#pragma once // NO_LINT
#include "ihandler.h"
#include <jsoncpp/json/value.h>
#include "config_handler.h"
#include "http_protocol.h"

#include <string>
#include <memory>

using hash_ = std::string;

namespace controllers {
 

struct TimeSeriesPredicts {
    std::vector<double> matrix_samples;
    size_t rows;
    size_t cols;
};

class IPredictController {
 public:
   virtual Json::Value makePredict(const Json::Value& request) = 0;
 protected:
   virtual TimeSeriesPredicts makeTimeSeries(const Json::Value& samples_data, size_t window_size) = 0;
    
};

class IModelController {
 public:
   virtual Json::Value callModelApi(const TimeSeriesPredicts& samples_data) = 0;
 protected:
   virtual Json::Value parseModelResponse(const IHTTPResponse_ request) = 0;
   virtual IHTTPRequest_ makeHttpForModel(const TimeSeriesPredicts& samples_data) = 0;
};


class IShowPlotController {
 public:
    virtual Json::Value createPlotData(const Json::Value& request) = 0;
};


class IRegisterController {
 public:
    virtual Json::Value registration(const Json::Value& request) = 0;
 protected:
    virtual hash_ hashPassword(const std::string& password) = 0;

};


class IAuthorizeController {
 public:
    virtual Json::Value authorization(const Json::Value& request) = 0;
 protected:
    virtual hash_ hashPassword(const std::string& password) = 0;
};


class IUpdateDataController {
 public:
   virtual bool updateData(const handlers::ProtocolAPI& protocol) = 0;
 protected:
   virtual Json::Value parseHTTPToJson(IHTTPResponse_ response) = 0;
   virtual std::string parseToApiRequest(const handlers::ProtocolAPI& protocol) = 0;

};

} // namespace controllers 
