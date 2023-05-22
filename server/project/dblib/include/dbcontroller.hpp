#pragma once // NO_LINT
#include <string>
#include <memory>
#include <jsoncpp/json/json.h>
#include "postgresserver.hpp"
#include "repositories.hpp"
#include "data.hpp"

using namespace repository;
using namespace database;

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
    STOCKS_REQUEST, 
    SESSION_REQUEST
};

namespace dbcontroller {
    class IDataBaseController {
    public:
        virtual Json::Value DataRequest(const Json::Value& request) = 0;
        virtual bool ConnectToDatabase() = 0;
        virtual void SetDatabaseConfig(const std::string&  addr, const std::string&  port,
            const std::string&  db_name, const std::string&  user, const std::string&  pass) = 0; 

    protected:
        // TimeSeries
        virtual Json::Value TimeSeriesPost(const Json::Value& data) = 0;
        virtual Json::Value TimeSeriesGet(const Json::Value& data) = 0;
        virtual Json::Value StocksGet() = 0;
        // Client
        virtual Json::Value ClientRequestPost(const Json::Value& data) = 0;
        virtual Json::Value ClientRequestGet(const TypeData& request_type, const std::string& key) = 0;
        virtual Json::Value ClientRequestUpdate(const Json::Value& data) = 0;
    };


    class DataBaseController: public IDataBaseController {
    public:
        DataBaseController();
        DataBaseController(const std::shared_ptr<IDataBase>& db, const std::shared_ptr<IClientRepository>& client_rep, 
            const std::shared_ptr<ITimeSeriesRepository>& timeseries_rep, const std::shared_ptr<ISubscriptionRepository>& subscription_rep);
        bool ConnectToDatabase() override;
        void SetDatabaseConfig(const std::string&  addr, const std::string&  port,
            const std::string&  db_name, const std::string&  user, const std::string&  pass) override; 

        Json::Value DataRequest(const Json::Value& request) override;

    private:
        // Route
        void GetRequestRouter(const Json::Value& request, Json::Value& response);
        void PostRequestRouter(const Json::Value& request, Json::Value& response);

        // TimeSeries
        Json::Value TimeSeriesPost(const Json::Value& data) override;
        Json::Value TimeSeriesGet(const Json::Value& data) override;
        virtual Json::Value StocksGet() override;
        
        // Client
        Json::Value ClientRequestPost(const Json::Value& data) override;
        Json::Value ClientRequestGet(const TypeData& request_type, const std::string& key) override;
        Json::Value ClientRequestUpdate(const Json::Value& data) override;
        Json::Value SessionRequestGet(const std::string& key);

        std::string host_addr_ = "25.21.238.202";
        std::string port_ = "5433";
        std::string db_name_ = "marketmentor";
        std::string user_ = "marketmentor_server";
        std::string password_ = "marketmentor_password";
        std::shared_ptr<IDataBase> database_;
        std::shared_ptr<ISubscriptionRepository> subscription_rep_;
        std::shared_ptr<ITimeSeriesRepository> timeseries_rep_;
        std::shared_ptr<IClientRepository> clien_rep_;
    };

}
