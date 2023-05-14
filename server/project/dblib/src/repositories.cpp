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

std::shared_ptr<ClientData> ClientRepository::DatabaseResponseParse(const Json::Value& db_response) {
    const int kUserId = 0;
    const int kLoginId = 1;
    const int kEmailId = 2;
    const int kPasswordId = 3;

    auto result = std::make_shared<ClientData>();
    result->login = db_response[kLoginId].asString();
    result->email = db_response[kEmailId].asString();
    result->hash_password = db_response[kPasswordId].asString();
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
    catch(SqlError const &e) {
        return nullptr;
    }

    auto result = DatabaseResponseParse(buffer);
    client_cache_->Insert(result->login, *result);
    return result;
}


bool ClientRepository::Delete(const std::string& key) {
    if (!database_->IsOpen()) {
        return false;
    }
    
    client_cache_->Delete(key);

    std::string query = "DELETE from client where login = '" + key + "'";
    bool result = database_->SendQuery(query);
    return result;
}


bool ClientRepository::Update(const std::string& key, const std::shared_ptr<ClientData>& data) {
    if (!database_->IsOpen()) {
        return false;
    }    
    
    client_cache_->Delete(key);
    client_cache_->Insert(key, *data);    

    std::string query = "UPDATE client SET password = '" + data->hash_password + "', email =  '" 
            + data->email + "' WHERE login = '" + key + "'";
    bool result = database_->SendQuery(query);    
    return result;
}


// TimeSeriesRepository

TimeSeriesRepository::TimeSeriesRepository(): database_(nullptr), 
        timeseries_cache_(std::make_shared<RepositoryCache<std::string, TimeSeriesData>>(5)) {
}


TimeSeriesRepository::TimeSeriesRepository(const std::shared_ptr<IDataBase>& db): database_(db),
        timeseries_cache_(std::make_shared<RepositoryCache<std::string, TimeSeriesData>>(5)) {
}


bool TimeSeriesRepository::Insert(const std::shared_ptr<TimeSeriesData>& data){
    Json::FastWriter json_to_string;
    std::string query = "INSERT into timeseries(name,date, param) VALUES ('" + 
        data->name_stock + "', '" + data->date + "', '" + json_to_string.write(data->param) + "')";

    if (!database_->IsOpen()) {
        return false;
    }

    bool result = database_->SendQuery(query);
    if (result) {
        timeseries_cache_->DeleteAll();
    }
    return result;    
}

std::shared_ptr<TimeSeriesData> TimeSeriesRepository::DatabaseResponseParse(const Json::Value& db_response) {
    const int kTimeSeriesId = 0;
    const int kNameId = 1;
    const int kParamId = 2;
    const int kDateId = 3;

    auto result = std::make_shared<TimeSeriesData>();

    result->date = "";
    Json::Value json_param;
    Json::Reader reader;
    for (int i = 0; i < db_response.size(); i++) {  
        reader.parse(db_response[i][kParamId].asString(), json_param[i]);
    };

    result->param = json_param;
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
            + "' ORDER by timeseries_id DESC LIMIT " + std::to_string(len_lags);

    Json::Value buffer;
    try {
        buffer = database_->GetData(query);
    }
    catch(ConnectError const &e) {
        return nullptr;
    }
    catch(ElementNotExist const &e) {
        return nullptr;
    }
    catch(SqlError const &e) {
        return nullptr;
    }

    auto result = DatabaseResponseParse(buffer);
    result->name_stock = name_stock;
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

std::shared_ptr<SubscriptionData> SubscriptionRepository::DatabaseResponseParse(const Json::Value& db_response) {
    const int kSubId = 0;
    const int kNameId = 1;
    const int kCountId = 2;
    const int kCostId = 3;

    auto row = std::make_shared<SubscriptionData>();
    row->name = db_response[kNameId].asString();
    row->cost = db_response[kCostId].asInt();
    row->count = db_response[kCountId].asInt();

    return row;
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
    catch(SqlError const &e) {
        return nullptr;
    }

    auto result = DatabaseResponseParse(buffer);
    subscription_cache_->Insert(result->name, *result);
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
    catch(SqlError const &e) {
        return nullptr;
    }
    
    std::shared_ptr<SubscriptionData> row;
    std::shared_ptr<AllSubscription> result;
    for (int i = 0; i < buffer.size(); i++) {
        row = DatabaseResponseParse(buffer[i]);
        result->data.push_back(*row);
    };

    return result;    
}