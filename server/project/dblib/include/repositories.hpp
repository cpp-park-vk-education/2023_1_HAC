#pragma once // NO_LINT
#include <string>
#include <memory>
#include <jsoncpp/json/json.h>
#include "data.hpp"
#include "dbexception.hpp"
#include "postgresserver.hpp"
#include "repositorycache.hpp"

using namespace database;
using namespace cache;

//enum Status{}

namespace repository {
    class IClientRepository {
    public:
        virtual ~IClientRepository() {};
        virtual bool Insert(const std::shared_ptr<ClientData>& data) = 0;
        virtual bool Delete(const std::string& key) = 0;
        virtual bool Update(const std::string& key, const std::shared_ptr<ClientData>& data) = 0;
        virtual std::shared_ptr<ClientData> GetByKey(const std::string& key) = 0;
    };


    class ITimeSeriesRepository {
    public:
        virtual ~ITimeSeriesRepository() {};
        virtual bool Insert(const std::shared_ptr<TimeSeriesData>& data) = 0;
        virtual std::shared_ptr<TimeSeriesData> GetByKey(const std::string& name_stock, const size_t& len_lags) = 0;
    };


    class ISubscriptionRepository {
    public:
        virtual ~ISubscriptionRepository() {};
        virtual std::shared_ptr<SubscriptionData> GetByKey(const std::string& key) = 0;
        virtual std::shared_ptr<AllSubscription> GetAll() = 0;
    };


    class ClientRepository: public IClientRepository{
    public:
        ClientRepository();
        ClientRepository(const std::shared_ptr<IDataBase>& db);
        
        bool Insert(const std::shared_ptr<ClientData>& data) override;
        bool Delete(const std::string& key) override;
        bool Update(const std::string& key, const std::shared_ptr<ClientData>& data) override;
        std::shared_ptr<ClientData> GetByKey(const std::string& key) override;

    private:
        std::shared_ptr<ClientData> DatabaseResponseParse(const Json::Value& db_response);

        std::shared_ptr<IDataBase> database_;
        std::shared_ptr<IRepositoryCache<std::string, ClientData>> client_cache_;
    };


    class TimeSeriesRepository: public ITimeSeriesRepository{
    public:
        TimeSeriesRepository();
        TimeSeriesRepository(const std::shared_ptr<IDataBase>& db);

        bool Insert(const std::shared_ptr<TimeSeriesData>& data) override;
        std::shared_ptr<TimeSeriesData> GetByKey(const std::string& name_stock, const size_t& len_lags) override;

    private:
        std::shared_ptr<TimeSeriesData> DatabaseResponseParse(const Json::Value& db_response);

        std::shared_ptr<IDataBase> database_; 
        std::shared_ptr<IRepositoryCache<std::string, TimeSeriesData>> timeseries_cache_;
    };


    class SubscriptionRepository: public ISubscriptionRepository {
    public:
        SubscriptionRepository();
        SubscriptionRepository(const std::shared_ptr<IDataBase>& db);
        
        std::shared_ptr<SubscriptionData> GetByKey(const std::string& key) override;
        std::shared_ptr<AllSubscription> GetAll() override;

    private:
        std::shared_ptr<SubscriptionData> DatabaseResponseParse(const Json::Value& db_response);
        
        std::shared_ptr<IDataBase> database_; 
        std::shared_ptr<RepositoryCache<std::string, SubscriptionData>> subscription_cache_;
    };

}