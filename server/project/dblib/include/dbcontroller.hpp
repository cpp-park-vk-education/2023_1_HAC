#pragma once // NO_LINT
#include "idatabasecontroller.hpp"
#include "idatabase.hpp"
#include "imemorydatabase.hpp"
#include "irepositories.hpp"

namespace dbcontroller {

class DataBaseController: public IDataBaseController {
public:
    DataBaseController();
    DataBaseController(const std::shared_ptr<database::IDataBase>& sql_db, 
                    const std::shared_ptr<database::IMemoryDataBase>& no_sqldb,
                    const std::shared_ptr<repository::IClientRepository>& client_rep, 
                    const std::shared_ptr<repository::ITimeSeriesRepository>& timeseries_rep, 
                    const std::shared_ptr<repository::ISubscriptionRepository>& subscription_rep,
                    const std::shared_ptr<repository::ITokenRepository>& token_rep);

    bool ConnectToDatabase() override;
    void SetDatabaseConfig(const std::string&  addr, const std::string&  port,
            const std::string&  db_name, const std::string&  user, const std::string&  pass) override; 
    void SetMemoryDatabaseConfig(const std::string& addr, const std::string&  port, const std::string&  pass) override; 
    Json::Value DataRequest(const Json::Value& request) noexcept override;


private:
    bool DatabaseIsOpen();
    bool MemoryDatabaseIsOpen();
    
    // Route
    void GetRequestRouter(const Json::Value& request, Json::Value& response);
    void PostRequestRouter(const Json::Value& request, Json::Value& response);

    // TimeSeries
    Json::Value TimeSeriesPost(const Json::Value& data) override;
    Json::Value TimeSeriesGet(const Json::Value& data) override;
    Json::Value StocksGet() override;
    Json::Value TimeSeriesFill(const Json::Value& data) override;
    
    // Client
    Json::Value ClientRequestPost(const Json::Value& data) override;
    Json::Value ClientRequestGet(const TypeData& request_type, const std::string& key) override;
    Json::Value ClientRequestUpdate(const Json::Value& data) override;

    // Token 
    Json::Value TokenRequestGet(const Json::Value& request) override;
    Json::Value TokenRequestPost(const Json::Value& request) override;
    Json::Value TokenRequestDelete(const std::string& key) override;

    // Postgres
    std::string postgres_host_addr_ = "62.84.127.93";
    std::string postgres_port_ = "5433";
    std::string postgres_db_name_ = "marketmentor";
    std::string postgres_user_ = "marketmentor_server";
    std::string postgres_password_ = "marketmentor_password";

    // Redis
    std::string redis_host_addr_ = "62.84.127.93";
    int redis_port_ = 6379;
    std::string redis_password_ = "marketmentor_password";

    // pointers
    std::shared_ptr<database::IDataBase> postgres_database_;
    std::shared_ptr<database::IMemoryDataBase> redis_database_;
    std::shared_ptr<repository::ISubscriptionRepository> subscription_rep_;
    std::shared_ptr<repository::ITimeSeriesRepository> timeseries_rep_;
    std::shared_ptr<repository::IClientRepository> clien_rep_;
    std::shared_ptr<repository::ITokenRepository> token_rep_;
};

} // namespace dbcontroller
