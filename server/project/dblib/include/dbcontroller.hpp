#pragma once // NO_LINT
#include <iostream>
#include <memory>
#include <jsoncpp/json/json.h>
#include "postgresserver.hpp"
#include "repositories.hpp"
#include "data.hpp"

using namespace repository;
using namespace database;

enum TypeRequest {
    GET_REQUEST,
    POST_REQUEST, 
    UPDATE_REQUEST
};

enum TypeData {
    CLIENT_DATA,
    AUTHORIZE_DATA,
    TIMESERIES_DATA,
    TIMESERIES_REQUEST
};

namespace dbcontroller {
    class IDataBaseController {
    public:
        virtual Json::Value DataRequest(const Json::Value& request) = 0;
        
    protected:
        // TimeSeries
        virtual Json::Value TimeSeriesPost(const Json::Value& data) = 0;
        virtual Json::Value TimeSeriesGet(const std::string& name_stock, const std::string& len_lags) = 0;
        // Client
        virtual Json::Value ClientRequestPost(const Json::Value& data) = 0;
        virtual Json::Value ClientRequestGet(const std::string& key) = 0;
        virtual Json::Value ClientRequestUpdate(const Json::Value& data) = 0;
    };


    class DataBaseController: public IDataBaseController {
    public:
        DataBaseController();
        DataBaseController(const std::shared_ptr<IClientRepository>& client_rep, 
            const std::shared_ptr<ITimeSeriesRepository>& timeseries_rep, const std::shared_ptr<ISubscriptionRepository>& subscription_rep);

        Json::Value DataRequest(const Json::Value& request) override;

    private:
        // TimeSeries
        Json::Value TimeSeriesPost(const Json::Value& data) override;
        Json::Value TimeSeriesGet(const std::string& name_stock, const std::string& len_lags) override;
        // Client
        Json::Value ClientRequestPost(const Json::Value& data) override;
        Json::Value ClientRequestGet(const std::string& key) override;
        Json::Value ClientRequestUpdate(const Json::Value& data) override;

        std::string host_addr_ = "127.0.0.1";
        std::string port_ = "5432";
        std::string db_name_ = "marketmentor";
        std::string user_ = "marketmentor_server";
        std::string password_ = "marketmentor_password";
        std::shared_ptr<IDataBase> database_;
        std::shared_ptr<ISubscriptionRepository> subscription_rep_;
        std::shared_ptr<ITimeSeriesRepository> timeseries_rep_;
        std::shared_ptr<IClientRepository> clien_rep_;
    };

}
