#include <iostream>
#include "repositories.hpp"
#include "repositorycache.hpp"

// Subscription. dbcontoroller не вызывает данный репозиторий, так как пока не была реализована система подписок

const int kSubCacheSize = 5;

repository::SubscriptionRepository::SubscriptionRepository(): database_(nullptr), 
        subscription_cache_(std::make_shared<cache::RepositoryCache<std::string, SubscriptionData>>(kSubCacheSize)) {
}


repository::SubscriptionRepository::SubscriptionRepository(const std::shared_ptr<database::IDataBase>& db): database_(db), 
        subscription_cache_(std::make_shared<cache::RepositoryCache<std::string, SubscriptionData>>(kSubCacheSize)) {
}

std::shared_ptr<SubscriptionData> repository::SubscriptionRepository::DatabaseResponseParse(const Json::Value& db_response) {
    const int kSubId = 0;
    const int kNameId = 1;
    const int kCountId = 2;
    const int kCostId = 3;

    auto row = std::make_shared<SubscriptionData>();
    row->name = db_response[kNameId].asString();
    try {
        row->cost = std::stoi(db_response[kCostId].asString());
        row->count = std::stoi(db_response[kCountId].asString());       
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return nullptr;
    }

    return row;
}

std::shared_ptr<SubscriptionData> repository::SubscriptionRepository::GetByKey(const std::string& key) {
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


std::shared_ptr<AllSubscription> repository::SubscriptionRepository::GetAll() {

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
