#pragma once
#include <iostream>
#include <memory>
#include <jsoncpp/json/json.h>
#include "data.hpp"
#include "repositorieserrors.hpp"
#include "postgresserver.hpp"

class IClientRepository {
    virtual bool Insert(const std::unique_ptr<ClientData>& data) = 0;
    virtual bool Delete(const std::string& key) = 0;
    virtual bool Update(const std::string& key, const std::unique_ptr<ClientData>& data) = 0;
    virtual std::unique_ptr<ClientData> GetByKey(const std::string& key) = 0;
};

class ITimeSeriesRepository {
    virtual bool Insert(const std::unique_ptr<TimeSeriesData>& data) = 0;
    virtual bool Delete(const std::string& data) = 0;
//  virtual bool Update(const std::string& key, const std::unique_ptr<TimeSeriesData>& data) = 0;
    virtual std::unique_ptr<TimeSeriesData> GetByKey(const std::string& name_stock, const size_t& len_lags) = 0;
};

class ISubscriptionRepository {
    virtual std::unique_ptr<SubscriptionData> GetByKey(const std::string& key) = 0;
    virtual std::unique_ptr<AllSubscription> GetAll() = 0;
    virtual bool Insert(const std::unique_ptr<SubscriptionData>& data) = 0;
};




class ClientRepository: public IClientRepository{
public:
    ClientRepository();
    ClientRepository(const std::shared_ptr<IDataBase> db);
    
    bool Insert(const std::unique_ptr<ClientData>& data) override;
    bool Delete(const std::string& key) override;
    bool Update(const std::string& key, const std::unique_ptr<ClientData>& data) override;
    std::unique_ptr<ClientData> GetByKey(const std::string& key) override;

private:
    std::shared_ptr<IDataBase> database_; 
};

class TimeSeriesRepository: public ITimeSeriesRepository{
public:
    TimeSeriesRepository();
    TimeSeriesRepository(const std::shared_ptr<IDataBase>& db);

    bool Insert(const std::unique_ptr<TimeSeriesData>& data) override;
    bool Delete(const std::string& data) override;
   // bool Update(const std::string& key, const std::unique_ptr<TimeSeriesData>& data) override;
    std::unique_ptr<TimeSeriesData> GetByKey(const std::string& name_stock, const size_t& len_lags) override;

private:
    std::shared_ptr<IDataBase> database_; 
};


class SubscriptionRepository: public ISubscriptionRepository {
public:
    SubscriptionRepository();
    SubscriptionRepository(const std::shared_ptr<IDataBase>& db);
    
    std::unique_ptr<SubscriptionData> GetByKey(const std::string& key) override;
    std::unique_ptr<AllSubscription> GetAll() override;
    bool Insert(const std::unique_ptr<SubscriptionData>& data) override;

private:
    std::shared_ptr<IDataBase> database_; 
};