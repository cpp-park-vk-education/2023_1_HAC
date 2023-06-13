#include <iostream>
#include "repositories.hpp"
#include "repositorycache.hpp"

repository::TokenRepository::TokenRepository(): database_(nullptr) {
} 


repository::TokenRepository::TokenRepository (const std::shared_ptr<database::IMemoryDataBase>& db): database_(db) {
}


bool repository::TokenRepository::Insert(const TokenData& data) {
    if (!database_->IsOpen()) {
        return false;
    }

    std::string key = data.token;
    std::string value = data.login;
    size_t ttl = data.time_live;

    return database_->Insert(key, value, ttl);
}


bool repository::TokenRepository::Delete(const std::string& key) {
    if (!database_->IsOpen()) {
        return false;
    }

    return database_->Delete(key);
}


std::shared_ptr<TokenData> repository::TokenRepository::Get(const std::string& key) {
    if (!database_->IsOpen()) {
        return nullptr;
    }

    std::string value = database_->Get(key);
    auto result = std::make_shared<TokenData>();
    if (value == "NULL") {
        return nullptr;
    }

    result->token = key;
    result->login = value;
    result->time_live = 0;
    return result;
}
