#pragma once // NO_LINT
#include "ihandler.h"
#include <jsoncpp/json/value.h>
#include "config_handler.h"
#include "http_protocol.h"

#include <string>
#include <memory>
#include <map>

using hash_ = std::string;
using cookie_map = std::map<std::string, std::string>;

namespace controllers {
 

struct TimeSeriesPredicts {
public:
    std::vector<double> matrix_samples;
    size_t lenpredict;

    bool operator==(const TimeSeriesPredicts& other) const {
        return matrix_samples == other.matrix_samples && lenpredict == other.lenpredict;
    }
};

class IPredictController {
 public:
  virtual Json::Value makePredict(const Json::Value& request) = 0;
 protected:
  virtual TimeSeriesPredicts makeTimeSeries(const std::vector<double>& samples_data, size_t lenpredict) = 0;
  virtual Json::Value makeDBProtocol(const Json::Value& request) = 0;
  virtual std::vector<double> parseDBProtocol(const Json::Value& response) = 0;
};

class IModelController {
 public:
  virtual Json::Value callModelApi(const TimeSeriesPredicts& samples_data) = 0;
};


class IShowPlotController {
 public:
  virtual Json::Value createPlotData(const Json::Value& request) = 0;
 protected:
  virtual Json::Value makeDBProtocol(const Json::Value& request) = 0;
};


class IRegisterController {
 public:
  virtual Json::Value registration(Json::Value& request) = 0;
 protected:
  virtual Json::Value makeDBProtocol(const Json::Value& request) = 0;

};


class IAuthorizeController {
 public:
  virtual Json::Value authorization(Json::Value& request) = 0;
 protected:
  virtual Json::Value makeDBProtocol(const Json::Value& request) = 0;
  virtual Json::Value checkPassword(const Json::Value& db_response, const Json::Value& request) = 0;
};


class IUpdateDataController {
 public:
  virtual bool updateData(const handlers::ProtocolAPI& protocol) = 0;
 protected:
  // virtual Json::Value parseHTTPToJson(IHTTPResponse_ response) = 0;
  virtual Json::Value makeDBProtocol(const Json::Value& request) = 0;

};

class IMiddleWare {
 public:
  virtual cookie_map checkCookieFile(const std::string& cookie) = 0;
 protected:
  virtual Json::Value makeDBProtocol(const std::string& cookie) = 0;
  virtual cookie_map paseDBResponse(const Json::Value& response, const std::string& cookie) = 0;
};

} // namespace controllers
