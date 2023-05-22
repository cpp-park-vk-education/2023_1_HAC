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

    std::string query = "INSERT into client(login,password, email, session_id, token) VALUES ('" + 
        data->login + "', '" + data->hash + "', '" + data->email +", " + std::to_string(data->session_id) + "' " + 
                    data->token + "')";

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
    const int kSessionId = 7;
    const int kTokenID = 8;

    auto result = std::make_shared<ClientData>();
    result->login = db_response[kLoginId].asString();
    result->email = db_response[kEmailId].asString();
    result->hash = db_response[kPasswordId].asString();
    result->session_id = std::stoi(db_response[kSessionId].asString());
    result->token = db_response[kTokenID].asString();

    return result;
}

std::shared_ptr<ClientData> ClientRepository::GetByKey(const ClientGetType& type, const std::string& key){
    if (client_cache_->Has(key) && type == LOGIN_KEY) {
        std::shared_ptr<ClientData> result = std::make_shared<ClientData>(client_cache_->Get(key));
        return result;
    }

    if (!database_->IsOpen()) {
        return nullptr;
    }

    std::string query;
    switch (type)
    {
    case LOGIN_KEY:
        query = "SELECT * from client where login = '" + key + "'";
        break;
    case TOKEN_KEY: 
        query = "SELECT * from client where token = '" + key + "'";
        break;
            
    default:
        return nullptr;
    }

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


bool ClientRepository::Update(const ClientUpdateType& type, const std::string& key, const std::shared_ptr<ClientData>& data) {
    if (!database_->IsOpen()) {
        return false;
    }    

    std::shared_ptr<ClientData> cache_data;
    if (client_cache_->Has(key)) {
        cache_data = std::make_shared<ClientData>(client_cache_->Get(key));        
    }

    std::string query;
    switch (type) {
    case UPDATE_EMAIL:
        query = "UPDATE client SET email =  '" + data->email + "' WHERE login = '" + key + "'"; 
        break;

    case UPDATE_PASSWORD:
        query = "UPDATE client SET password = '" + data->hash + "' WHERE login = '" + key + "'";    
        break;

    case UPDATE_SESSION:
        if (data->token == "NULL") {
            query = "UPDATE client SET session_id = NULL, token = NULL WHERE login = '" + key + "'";              
        }
        else {
            query = "UPDATE client SET session_id = '" + std::to_string(data->session_id) + 
                    + "', token = '" + data->token + "' WHERE login = '" + key + "'";                     
        }

        break;

    default:
        return false;
    }

    bool result = database_->SendQuery(query);

    if (result && client_cache_->Has(key)) {
        CacheUpdate(type, key, data);
    }    

    return result;
}

void ClientRepository::CacheUpdate(const ClientUpdateType& type, const std::string& key, const std::shared_ptr<ClientData>& data) {
    std::shared_ptr<ClientData> cache_data;
    cache_data = std::make_shared<ClientData>(client_cache_->Get(key));  

    switch (type) {
    case UPDATE_EMAIL:
        cache_data->email = data->email;
        break;

    case UPDATE_PASSWORD:
        cache_data->hash = data->hash;
        break;

    case UPDATE_SESSION:
        cache_data->session_id = data->session_id;
        cache_data->token = data->token;
        break;

    default:
        return;
    }    

    client_cache_->Delete(key);
    client_cache_->Insert(key, *cache_data);
}

// TimeSeriesRepository

TimeSeriesRepository::TimeSeriesRepository(): database_(nullptr), 
        timeseries_cache_(std::make_shared<RepositoryCache<std::string, TimeSeriesData>>(5)) {
}


TimeSeriesRepository::TimeSeriesRepository(const std::shared_ptr<IDataBase>& db): database_(db),
        timeseries_cache_(std::make_shared<RepositoryCache<std::string, TimeSeriesData>>(5)),
        stocks_cache_(std::make_shared<RepositoryCache<std::string, AllStocks>>(1)) {
}


bool TimeSeriesRepository::Insert(const std::shared_ptr<TimeSeriesData>& data){
    Json::FastWriter json_to_string;
    std::string query = "INSERT into timeseries(name,date, param) VALUES ('" + 
        data->name_stock + "', '" + data->date + "', " + data->param[0].asString() + ")";

    if (!database_->IsOpen()) {
        return false;
    }

    bool result = database_->SendQuery(query);
    if (result) {
        timeseries_cache_->DeleteAll();
    }

    return result;    
}


std::shared_ptr<TimeSeriesData> TimeSeriesRepository::TimeSeriesResponseParse(const Json::Value& db_response) {
    const int kTimeSeriesId = 0;
    const int kNameId = 1;
    const int kParamId = 3;
    const int kDateId = 2;

    auto result = std::make_shared<TimeSeriesData>();

    result->date = "";
    Json::Value json_param;
    Json::Reader reader;
    std::string number;
    for (int i = 0; i < db_response.size(); i++) {  
        if (db_response[i][kParamId] != Json::Value::null) {
            number = db_response[i][kParamId].asString();
            json_param[i] = std::stod(number);
        }
    };

    result->param = json_param;
    return result;
}


std::shared_ptr<TimeSeriesData> TimeSeriesRepository::GetByKey(const std::string& name_stock, const size_t& len_lags, 
                                const std::string& start_date /* = "" */, const std::string& finish_date /* = "" */) {

    std::string key = name_stock + "::" + std::to_string(len_lags);
    if (timeseries_cache_->Has(key)) {
        auto result = std::make_shared<TimeSeriesData>(timeseries_cache_->Get(key));
        return result;    
    }

    if (!database_->IsOpen()) {
        return nullptr;
    }

    std::string query;
    if (start_date == "" || finish_date == "")
        query = "SELECT * FROM timeseries where name = '" + name_stock 
                + "' ORDER by timeseries_id DESC LIMIT " + std::to_string(len_lags);
    else {
        query = " SELECT * FROM timeseries where name = '" + name_stock + "' and date > '" + start_date + "' and date < '" +
                finish_date + "' ORDER by timeseries_id DESC LIMIT " + std::to_string(len_lags);       
    }

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

    auto result = TimeSeriesResponseParse(buffer);
    result->name_stock = name_stock;
    key = name_stock + "::" + std::to_string(name_stock.size());
    timeseries_cache_->Insert(key, *result);
    return result;
}

std::shared_ptr<AllStocks> TimeSeriesRepository::StocksResponseParse(const Json::Value& db_response) {
    const int kTimeSeriesId = 0;
    const int kNameId = 1;
    const int kParamId = 3;
    const int kDateId = 2;

    auto result = std::make_shared<AllStocks>();

    Json::Value json_param;
    Json::Reader reader;
    for (int i = 0; i < db_response.size(); i++) {  
        if (db_response[i][kNameId] != Json::Value::null) {
            json_param[i] = db_response[i][kNameId].asString();
        }
    };

    result->list = json_param;
    return result;
}

std::shared_ptr<AllStocks> TimeSeriesRepository::GetAllStocks() { 
    std::string key = "list";
    if (stocks_cache_->Has(key)) {
        auto result = std::make_shared<AllStocks>(stocks_cache_->Get(key));
        return result;    
    }

    if (!database_->IsOpen()) {
        return nullptr;
    }

    std::string query = "SELECT DISTINCT name FROM timeseries";

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

    auto result = StocksResponseParse(buffer);
    stocks_cache_->Insert(key, *result);
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
    row->cost = std::stoi(db_response[kCostId].asString());
    row->count = std::stoi(db_response[kCountId].asString());

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
    
    std::shared_ptr<SubscriptionData> row;
    std::shared_ptr<AllSubscription> result = std::make_shared<AllSubscription>();
    Json::Value data;

    for (int i = 0; i < buffer.size(); i++) {
        row = DatabaseResponseParse(buffer[i]);
        data[i]["name"] = row->name;
        data[i]["count"] = row->count;
        data[i]["cost"] = row->cost;
    };

    result->list = data;
    return result;    
}
