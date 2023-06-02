#pragma once // NO_LINT
#include <string>
#include <memory>
#include <jsoncpp/json/json.h>

enum TypeRequest {
    GET_REQUEST,
    POST_REQUEST
};

enum TypeData {
    AUTHORIZATION,
    REGISTRATION,
    CHANGE_USER_EMAIL_SETTINGS,
    CHANGE_USER_SESSION,
    CHANGE_USER_PASSWORD_SETTINGS,
    TIMESERIES_REQUEST,
    TIMESERIES_FILL,
    STOCKS_REQUEST, 
    SESSION_REQUEST,
    SESSION_DELETE
};

namespace dbcontroller {
    class IDataBaseController {
    public:
        virtual Json::Value DataRequest(const Json::Value& request) = 0;
        virtual bool ConnectToDatabase() = 0;
        virtual void SetDatabaseConfig(const std::string&  addr, const std::string&  port,
            const std::string&  db_name, const std::string&  user, const std::string&  pass) = 0; 
        virtual void SetMemoryDatabaseConfig(const std::string& addr, const std::string&  port, const std::string&  pass) = 0; 

    protected:
        // TimeSeries
        virtual Json::Value TimeSeriesPost(const Json::Value& data) = 0;
        virtual Json::Value TimeSeriesGet(const Json::Value& data) = 0;
        virtual Json::Value StocksGet() = 0;
        virtual Json::Value TimeSeriesFill(const Json::Value& data) = 0;

        // Client
        virtual Json::Value ClientRequestPost(const Json::Value& data) = 0;
        virtual Json::Value ClientRequestGet(const TypeData& request_type, const std::string& key) = 0;
        virtual Json::Value ClientRequestUpdate(const Json::Value& data) = 0;
        // Token
        virtual Json::Value TokenRequestGet(const Json::Value& request) = 0;
        virtual Json::Value TokenRequestPost(const Json::Value& request) = 0;
        virtual Json::Value TokenRequestDelete(const std::string& key) = 0;

    };

}
