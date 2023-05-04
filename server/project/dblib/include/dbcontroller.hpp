#pragma once // NO_LINT
#include <iostream>
#include <memory>
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
        virtual bool TimeSeriesPost(const std::shared_ptr<TimeSeriesData>& data) = 0;
        virtual std::shared_ptr<TimeSeriesData> TimeSeriesGet(const std::string& name_stock, const std::string& len_lags) = 0;
        // Client
        virtual bool ClientRequestPost(const std::shared_ptr<ClientData>& data) = 0;
        virtual std::shared_ptr<ClientData> ClientRequestGet(const std::string& key) = 0;
        virtual bool ClientRequestUpdate(const std::shared_ptr<ClientData> data) = 0;
    };


    class DataBaseController: public IDataBaseController {
    public:
        DataBaseController();
        DataBaseController(const std::shared_ptr<IClientRepository>& client_rep, 
            const std::shared_ptr<ITimeSeriesRepository>& timeseries_rep, const std::shared_ptr<ISubscriptionRepository>& subscription_rep);

        Json::Value DataRequest(const Json::Value& request) override;
  
    private:
        // TimeSeries
        bool TimeSeriesPost(const std::shared_ptr<TimeSeriesData>& data) override;
        std::shared_ptr<TimeSeriesData> TimeSeriesGet(const std::string& name_stock, const std::string& len_lags) override;
        // Client
        bool ClientRequestPost(const std::shared_ptr<ClientData>& data) override;
        std::shared_ptr<ClientData> ClientRequestGet(const std::string& key) override;
        bool ClientRequestUpdate(const std::shared_ptr<ClientData> data) override;

        static std::string host_addr_;
        static std::string port_;
        static std::string db_name_;
        static std::string user_;
        static std::string password_;
        std::shared_ptr<IDataBase> database_;
        std::shared_ptr<ISubscriptionRepository> subscription_rep_;
        std::shared_ptr<ITimeSeriesRepository> timeseries_rep_;
        std::shared_ptr<IClientRepository> clien_rep_;
    };

}

