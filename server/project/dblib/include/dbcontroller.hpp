#pragma once // NO_LINT
#include <string>
#include <memory>
#include <jsoncpp/json/json.h>
#include "postgresserver.hpp"
#include "repositories.hpp"
#include "tokenrepository.hpp"
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
        // Client
        virtual Json::Value ClientRequestPost(const Json::Value& data) = 0;
        virtual Json::Value ClientRequestGet(const TypeData& request_type, const std::string& key) = 0;
        virtual Json::Value ClientRequestUpdate(const Json::Value& data) = 0;
        // Token
        virtual Json::Value TokenRequestGet(const Json::Value& request) = 0;
        virtual Json::Value TokenRequestPost(const Json::Value& request) = 0;
        virtual Json::Value TokenRequestDelete(const std::string& key) = 0;

    };


    class DataBaseController: public IDataBaseController {
    public:
        DataBaseController();
        DataBaseController(const std::shared_ptr<IDataBase>& sql_db, const std::shared_ptr<IMemoryDataBase>& no_sqldb,
                     const std::shared_ptr<IClientRepository>& client_rep, const std::shared_ptr<ITimeSeriesRepository>& timeseries_rep, 
                     const std::shared_ptr<ISubscriptionRepository>& subscription_rep);

        bool ConnectToDatabase() override;
        void SetDatabaseConfig(const std::string&  addr, const std::string&  port,
            const std::string&  db_name, const std::string&  user, const std::string&  pass) override; 

        void SetMemoryDatabaseConfig(const std::string& addr, const std::string&  port, const std::string&  pass) override; 


        Json::Value DataRequest(const Json::Value& request) noexcept override;

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

        // Token 
        Json::Value TokenRequestGet(const Json::Value& request) override;
        Json::Value TokenRequestPost(const Json::Value& request) override;
        Json::Value TokenRequestDelete(const std::string& key) override;

        // Postgres
        std::string postgres_host_addr_ = "25.21.238.202";
        std::string postgres_port_ = "5433";
        std::string postgres_db_name_ = "marketmentor";
        std::string postgres_user_ = "marketmentor_server";
        std::string postgres_password_ = "marketmentor_password";

        // Redis
        std::string redis_host_addr_ = "127.0.0.1";
        int redis_port_ = 6379;
        std::string redis_password_ = "marketmentor_password";

        std::shared_ptr<IDataBase> postgres_database_;
        std::shared_ptr<IMemoryDataBase> redis_database_;
        std::shared_ptr<ISubscriptionRepository> subscription_rep_;
        std::shared_ptr<ITimeSeriesRepository> timeseries_rep_;
        std::shared_ptr<IClientRepository> clien_rep_;
        std::shared_ptr<ITokenRepository> token_rep_;
    };

}
