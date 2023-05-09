#include "repositories.hpp"

using namespace repository;

// ClientRepository

ClientRepository::ClientRepository(): database_(nullptr), client_cache_(std::make_shared<RepositoryCache<std::string, ClientData>>(20)) {
}


ClientRepository::ClientRepository(const std::shared_ptr<IDataBase>& db): database_(db),
         client_cache_(std::make_shared<RepositoryCache<std::string, ClientData>>(20)) {
}


bool ClientRepository::Insert(const std::shared_ptr<ClientData>& data) {
    if (client_cache_->Has(data->login)) {
        return false;
    }

    if (!database_->IsOpen()) {
        return false;
    }

    std::string query = "INSERT into client(login,password, email) VALUES ('" + 
        data->login + "', '" + data->hash_password + "', '" + data->email + "')";

    bool result = database_->SendQuery(query);
    if (result) {
        client_cache_->Insert(data->login, *data);
    }    
    return result;
}


std::shared_ptr<ClientData> ClientRepository::GetByKey(const std::string& key){
    if (client_cache_->Has(key)) {
        std::shared_ptr<ClientData> result = std::make_shared<ClientData>(client_cache_->Get(key));
        return result;
    }

    if (!database_->IsOpen()) {
        return nullptr;
    }

    std::string query = "SELECT * from client where login = '" + key + "'";
    Json::Value buffer;
    try {
        buffer = database_->GetRow(query);
    }
    catch(ConnectError const &e) {
        return nullptr;
    }
    catch(ElementNotExist const &e) {
        return nullptr;
    }

    const int kUserId = 0;
    const int kLoginId = 1;
    const int kEmailId = 2;
    const int kPasswordId = 3;

    Json::FastWriter convert_to_string;
    auto result = std::make_shared<ClientData>();
    result->login = convert_to_string.write(buffer[kLoginId]);
    result->email = convert_to_string.write(buffer[kEmailId]);
    result->hash_password = convert_to_string.write(buffer[kPasswordId]);
    return result;
}


bool ClientRepository::Delete(const std::string& key) {
    client_cache_->Delete(key);

    if (!database_->IsOpen()) {
        return false;
    }

    std::string query = "DELETE from client where login = '" + key + "'";
    bool result = database_->SendQuery(query);
    return result;
}


bool ClientRepository::Update(const std::string& key, const std::shared_ptr<ClientData>& data) {
    client_cache_->Delete(key);
    client_cache_->Insert(key, *data);    
    if (!database_->IsOpen()) {
        return false;
    }

    std::string query = "UPDATE client SET password = " + data->hash_password + ", email =  " 
            + data->email + " WHERE login = '" + key + "'";
    bool result = database_->SendQuery(query);    
    return true;
}


// TimeSeriesRepository

TimeSeriesRepository::TimeSeriesRepository(): database_(nullptr), 
        timeseries_cache_(std::make_shared<RepositoryCache<std::string, TimeSeriesData>>(5)) {
}


TimeSeriesRepository::TimeSeriesRepository(const std::shared_ptr<IDataBase>& db): database_(db),
        timeseries_cache_(std::make_shared<RepositoryCache<std::string, TimeSeriesData>>(5)) {
}


bool TimeSeriesRepository::Insert(const std::shared_ptr<TimeSeriesData>& data){
    Json::FastWriter convert_to_string;
    std::string query = "INSERT into timeseries(name_stock,date, param) VALUES ('" + 
        data->name_stock + "', '" + data->date + "', '" + convert_to_string.write(data->param) + "')";

    if (!database_->IsOpen()) {
        return false;
    }

    bool result = database_->SendQuery(query);
    if (result) {
        timeseries_cache_->DeleteAll();
    }
    return result;    
}


std::shared_ptr<TimeSeriesData> TimeSeriesRepository::GetByKey(const std::string& name_stock, const size_t& len_lags) {
    std::string key = name_stock + "::" + std::to_string(len_lags);
    if (timeseries_cache_->Has(key)) {
        auto result = std::make_shared<TimeSeriesData>(timeseries_cache_->Get(key));
        return result;    
    }

    if (!database_->IsOpen()) {
        return nullptr;
    }

    std::string query = "SELECT * from timeseries where name = '" + name_stock 
            + " ORDER by timeseries_id DESC LIMIT " + std::to_string(len_lags);

    Json::Value buffer;
    try {
        buffer = database_->GetRow(query);
    }
    catch(ConnectError const &e) {
        return nullptr;
    }
    catch(ElementNotExist const &e) {
        return nullptr;
    }

    const int kTimeSeriesId = 0;
    const int kNameId = 1;
    const int kParamId = 2;
    const int kDateId = 3;

    auto result = std::make_shared<TimeSeriesData>();

    result->name_stock = name_stock;
    result->date = "";
    Json::Value json_param;
    for (int i = 0; i < buffer.size(); i++) {
        json_param[i] = buffer[i][kParamId];
    };

    result->param = json_param;
    timeseries_cache_->Insert(key, *result);
    return result;
}


// Subscription

SubscriptionRepository::SubscriptionRepository(): database_(nullptr), 
        subscription_cache_(std::make_shared<RepositoryCache<std::string, SubscriptionData>>(5)) {
}


SubscriptionRepository::SubscriptionRepository(const std::shared_ptr<IDataBase>& db): database_(db), 
        subscription_cache_(std::make_shared<RepositoryCache<std::string, SubscriptionData>>(5)) {
}


std::shared_ptr<SubscriptionData> SubscriptionRepository::GetByKey(const std::string& key) {
    if (subscription_cache_->Has(key)) {
        std::shared_ptr<SubscriptionData> result = std::make_shared<SubscriptionData>(subscription_cache_->Get(key));
        return result;
    }

    if (!database_->IsOpen()) {
        return nullptr;
    }

    std::string query = "SELECT * from subscription where login = '" + key + "'";
    Json::Value buffer;
    try {
        buffer = database_->GetRow(query);
    }
    catch(ConnectError const &e) {
        return nullptr;
    }
    catch(ElementNotExist const &e) {
        return nullptr;
    }

    const int kSubId = 0;
    const int kNameId = 1;
    const int kCountId = 2;
    const int kCostId = 3;

    auto result = std::make_shared<SubscriptionData>();
    result->name = buffer[kNameId].asString();
    result->count = buffer[kCountId].asInt();
    result->cost = buffer[kCostId].asInt();
    return result;
}


std::shared_ptr<AllSubscription> SubscriptionRepository::GetAll() {

    if (!database_->IsOpen()) {
        return nullptr;
    }

    std::string query = "SELECT * from subscription";

    Json::Value buffer;
    try {
        buffer = database_->GetRow(query);
    }
    catch(ConnectError const &e) {
        return nullptr;
    }
    catch(ElementNotExist const &e) {
        return nullptr;
    }

    const int kSubId = 0;
    const int kNameId = 1;
    const int kCountId = 2;
    const int kCostId = 3;

    SubscriptionData row;
    std::shared_ptr<AllSubscription> result;
    for (int i = 0; i < buffer.size(); i++) {
        row.name = buffer[i][kNameId].asString();
        row.cost = buffer[i][kCostId].asInt();
        row.count = buffer[i][kCountId].asInt();
        result->data.push_back(row);
    };

    return result;    
}