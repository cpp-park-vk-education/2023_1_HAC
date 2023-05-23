#include "tokenrepository.hpp"

using namespace repository;
using namespace database;

TokenRepository::TokenRepository(): database_(nullptr) {
} 


TokenRepository::TokenRepository (const std::shared_ptr<IMemoryDataBase>& db): database_(db) {
}

bool TokenRepository::Insert(const TokenData& data) {
    std::string key = data.token;
    std::string value = data.login;
    size_t ttl = data.time_live;

    return database_->Insert(key, value, ttl);
}

std::shared_ptr<TokenData> TokenRepository::Get(const std::string key) {
    std::string value = database_->Get(key);
    auto result = std::make_shared<TokenData>();
    if (value == "NULL") {
        return nullptr;
    }

    result->login = key;
    result->token = value;
    result->time_live = 0;

    return result;
}
