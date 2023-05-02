#include "repositories.hpp"

ClientRepository::ClientRepository(): database_(nullptr){
}

ClientRepository::ClientRepository(const std::shared_ptr<IDataBase> db) {
    database_ = db;
}

bool ClientRepository::Insert(const std::unique_ptr<ClientData>& data){
    std::string query = "INSERT into client(login,password, email) VALUES ('" + 
        data->login + "', '" + data->hash_password + "', '" + data->email + "')";
    database_->SendQuery(query);
    return true;
}

std::unique_ptr<ClientData> ClientRepository::GetByKey(const std::string& key){
    std::string query = "SELECT * from client where email = '" + key + "'";
    std::vector<std::string> buffer;
    try {
        buffer = database_->GetRow(query);
    }
    catch(ConnectError const &e) {
        throw UnavailableDataBase(e.what());
    }

    // Везде конст
    const int kUserId = 0;
    int kLoginId = 1;
    int kEmailId = 2;
    int kPasswordId = 3;
    int kSaltId = 4;
    int kStatusId = 5;
    int kDateId = 6;
    int kValidityId = 7;

    auto result = std::make_unique<ClientData>();
    result->login = buffer[kLoginId];
    result->email = buffer[kEmailId];
    result->hash_password = buffer[kPasswordId];

    return result;
}

bool ClientRepository::Delete(const std::string& key) {
    return true;
}

bool ClientRepository::Update(const std::string& key, const std::unique_ptr<ClientData>& data) {
    return true;
}



TimeSeriesRepository::TimeSeriesRepository(): database_(nullptr){
}

TimeSeriesRepository::TimeSeriesRepository(const std::shared_ptr<IDataBase>& db) {
    database_ = db;
}


// Добавить количество поинтов
bool TimeSeriesRepository::Insert(const std::unique_ptr<TimeSeriesData>& data){
    Json::FastWriter writer;
    std::string string_json = writer.write(data->param);
    std::string query = "INSERT into timeseries(name, param) VALUES ('" + data->name_stock + "', '" + string_json+ "')"; 
    database_->SendQuery(query);
    return true;
}

// SQL запрос поменять

std::unique_ptr<TimeSeriesData> TimeSeriesRepository::GetByKey(const std::string& name_stock, const size_t& len_lags) {
    std::string query = "SELECT * from timeseries where name = '" + name_stock + "' and date = '" + std::to_string(len_lags) + "'";

// Добавить исключенияи и переименовать время
    std::vector<std::string> buffer = database_->GetRow(query);

    const int kTimeSeriesId = 0;
    const int kNameId = 1;
    const int kJsonId = 2;
    const int kDateId = 3;
    auto result = std::make_unique<TimeSeriesData>();
    Json::Reader reader;

    //result->id = std::stoi(buffer[kTimeSeriesId]);
    result->name_stock = buffer[kNameId];
    reader.parse(buffer[kJsonId], result->param);
    // result->len_lags = static_cast<size_t>(buffer[kDateId]);

    return result;
}


bool TimeSeriesRepository::Delete(const std::string& data) {
    return true;
}


SubscriptionRepository::SubscriptionRepository(): database_(nullptr) {
}

SubscriptionRepository::SubscriptionRepository(const std::shared_ptr<IDataBase>& db): database_(db) {
}

std::unique_ptr<SubscriptionData> SubscriptionRepository::GetByKey(const std::string& key) {
    auto a = std::make_unique<SubscriptionData>();
    return a;
}

std::unique_ptr<AllSubscription> SubscriptionRepository::GetAll() {
    return nullptr;
}

bool SubscriptionRepository::Insert(const std::unique_ptr<SubscriptionData>& data) {
    return true;    
}