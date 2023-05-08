#include "repositories.hpp"

using namespace repository;
using namespace database;

ClientRepository::ClientRepository(): database_(nullptr){
}

ClientRepository::ClientRepository(const std::shared_ptr<IDataBase> db) {
    database_ = db;
}

bool ClientRepository::Insert(const std::shared_ptr<ClientData>& data){
    return true;
}

std::shared_ptr<ClientData> ClientRepository::GetByKey(const std::string& key){
    return nullptr;
}

bool ClientRepository::Delete(const std::string& key) {
    return true;
}

bool ClientRepository::Update(const std::string& key, const std::shared_ptr<ClientData>& data) {
    return true;
}


TimeSeriesRepository::TimeSeriesRepository(): database_(nullptr){
}

TimeSeriesRepository::TimeSeriesRepository(const std::shared_ptr<IDataBase>& db) {
    database_ = db;
}


bool TimeSeriesRepository::Insert(const std::shared_ptr<TimeSeriesData>& data){
    return true;
}

std::shared_ptr<TimeSeriesData> TimeSeriesRepository::GetByKey(const std::string& name_stock, const size_t& len_lags) {
    return nullptr;
}


SubscriptionRepository::SubscriptionRepository(): database_(nullptr) {
}

SubscriptionRepository::SubscriptionRepository(const std::shared_ptr<IDataBase>& db): database_(db) {
}

std::shared_ptr<SubscriptionData> SubscriptionRepository::GetByKey(const std::string& key) {
    return nullptr;
}

std::shared_ptr<AllSubscription> SubscriptionRepository::GetAll() {
    return nullptr;
}
