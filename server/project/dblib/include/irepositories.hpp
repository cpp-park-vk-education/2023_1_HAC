#pragma once // NO_LINT
#include <string>
#include <memory>
#include <jsoncpp/json/json.h>
#include "data.hpp"
#include "dbexception.hpp"

enum ClientUpdateType {
    UPDATE_EMAIL,
    UPDATE_PASSWORD,
    UPDATE_SESSION
};

enum ClientGetType {
    LOGIN_KEY
};

namespace repository {

class IClientRepository {
public:
    virtual ~IClientRepository() {};
    virtual bool Insert(const std::shared_ptr<ClientData>& data) = 0;
    virtual bool Delete(const std::string& key) = 0;
    virtual bool Update(const ClientUpdateType& type, const std::string& key, 
                        const std::shared_ptr<ClientData>& data) = 0;

    virtual std::shared_ptr<ClientData> GetByKey(const ClientGetType& type, const std::string& key) = 0;
};


class ITimeSeriesRepository {
public:
    virtual ~ITimeSeriesRepository() {};
    virtual bool Insert(const std::shared_ptr<TimeSeriesData>& data) = 0;
    virtual bool InsertArray(const std::shared_ptr<TimeSeriesData>& data) = 0;

    virtual std::shared_ptr<TimeSeriesData> GetByKey(const std::string& name_stock, const size_t& len_lags, 
                            const std::string& date = "", const std::string& finish_date = "") = 0;

    virtual std::shared_ptr<AllStocks> GetAllStocks() = 0;
};


class ISubscriptionRepository {
public:
    virtual ~ISubscriptionRepository() {};
    virtual std::shared_ptr<SubscriptionData> GetByKey(const std::string& key) = 0;
    virtual std::shared_ptr<AllSubscription> GetAll() = 0;
};


class  ITokenRepository {
public:
    virtual ~ITokenRepository() {};
    virtual bool Insert(const TokenData& data) = 0;
    virtual bool Delete(const std::string& key) = 0;
    virtual std::shared_ptr<TokenData> Get(const std::string& key) = 0;
};

} // namespace repository