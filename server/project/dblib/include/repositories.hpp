#pragma once // NO_LINT
#include "irepositories.hpp"
#include "idatabase.hpp"
#include "imemorydatabase.hpp"
#include "irepositorycache.hpp"


namespace repository {

class ClientRepository: public IClientRepository{
public:
    ClientRepository();
    ClientRepository(const std::shared_ptr<database::IDataBase>& db);
    
    bool Insert(const std::shared_ptr<ClientData>& data) override;
    bool Delete(const std::string& key) override;
    bool Update(const ClientUpdateType& type, const std::string& key, const std::shared_ptr<ClientData>& data) override;
    std::shared_ptr<ClientData> GetByKey(const ClientGetType& type, const std::string& key) override;

private:
    std::shared_ptr<ClientData> DatabaseResponseParse(const Json::Value& db_response);
    void CacheUpdate(const ClientUpdateType& type, const std::string& key, const std::shared_ptr<ClientData>& data);
    bool SpecialCharacterCheck(const std::string& word);

    std::shared_ptr<database::IDataBase> database_;
    std::shared_ptr<cache::IRepositoryCache<std::string, ClientData>> client_cache_;
};


class TimeSeriesRepository: public ITimeSeriesRepository{
public:
    TimeSeriesRepository();
    TimeSeriesRepository(const std::shared_ptr<database::IDataBase>& db);

    bool Insert(const std::shared_ptr<TimeSeriesData>& data) override;
    bool InsertArray(const std::shared_ptr<TimeSeriesData>& data) override;

    std::shared_ptr<TimeSeriesData> GetByKey(const std::string& name_stock, const size_t& len_lags, 
                            const std::string& date = "", const std::string& finish_date = "") override;
    std::shared_ptr<AllStocks> GetAllStocks() override;

private:
    std::shared_ptr<TimeSeriesData> TimeSeriesResponseParse(const Json::Value& db_response);
    std::shared_ptr<AllStocks> StocksResponseParse(const Json::Value& db_response);

    std::shared_ptr<database::IDataBase> database_; 
    std::shared_ptr<cache::IRepositoryCache<std::string, TimeSeriesData>> timeseries_cache_;
    std::shared_ptr<cache::IRepositoryCache<std::string, AllStocks>> stocks_cache_;
};


class SubscriptionRepository: public ISubscriptionRepository {
public:
    SubscriptionRepository();
    SubscriptionRepository(const std::shared_ptr<database::IDataBase>& db);
    
    std::shared_ptr<SubscriptionData> GetByKey(const std::string& key) override;
    std::shared_ptr<AllSubscription> GetAll() override;

private:
    std::shared_ptr<SubscriptionData> DatabaseResponseParse(const Json::Value& db_response);
    
    std::shared_ptr<database::IDataBase> database_; 
    std::shared_ptr<cache::IRepositoryCache<std::string, SubscriptionData>> subscription_cache_;
};


class TokenRepository: public ITokenRepository {
public:
    TokenRepository();
    TokenRepository(const std::shared_ptr<database::IMemoryDataBase>& db);

    bool Insert(const TokenData& data) override;
    bool Delete(const std::string& key) override;
    std::shared_ptr<TokenData> Get(const std::string& key) override;

private:
    std::shared_ptr<database::IMemoryDataBase> database_;
};

} // namespace repository