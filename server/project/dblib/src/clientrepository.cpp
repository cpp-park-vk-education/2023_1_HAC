#include <iostream>
#include "repositories.hpp"
#include "repositorycache.hpp"

using namespace repository;

const int kClienCacheSize = 20;

// ClientRepository

ClientRepository::ClientRepository(): database_(nullptr), 
        client_cache_(std::make_shared<RepositoryCache<std::string, ClientData>>(kClienCacheSize)) {
}


ClientRepository::ClientRepository(const std::shared_ptr<IDataBase>& db): database_(db),
         client_cache_(std::make_shared<RepositoryCache<std::string, ClientData>>(kClienCacheSize)) {
}

// Защита от специальных символов в запросе пользователя
bool ClientRepository::SpecialCharacterCheck(const std::string& word) {
    // Если символ найден, то возвращаем true
    if (word.find('\'') != std::string::npos) {
        return true;
    }

    return false;
}

bool ClientRepository::Insert(const std::shared_ptr<ClientData>& data) {
    if (client_cache_->Has(data->login)) {
        return false;
    }

    if (!database_->IsOpen()) {
        return false;
    }

    if (SpecialCharacterCheck(data->email) || SpecialCharacterCheck(data->login)) {
        return false;
    }

    std::string query = "INSERT into client(login,password, email) VALUES ('" + 
        data->login + "', '" + data->hash + "', '" + data->email + "')";

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
    result->hash = db_response[kPasswordId].asString();

    return result;
}

std::shared_ptr<ClientData> ClientRepository::GetByKey(const ClientGetType& type, const std::string& key){
    if (client_cache_->Has(key)) {
        std::shared_ptr<ClientData> result = std::make_shared<ClientData>(client_cache_->Get(key));
        return result;
    }

    if (!database_->IsOpen()) {
        return nullptr;
    }

    if (SpecialCharacterCheck(key)) {
        return nullptr;
    }

    std::string query;
    switch (type)
    {
    case LOGIN_KEY:
        query = "SELECT * from client where login = '" + key + "'";
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

    if (SpecialCharacterCheck(key)) {
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

    if (SpecialCharacterCheck(key)) {
        return false;
    }

    std::string query;
    switch (type) {
    case UPDATE_EMAIL:
        if (SpecialCharacterCheck(data->email)) {
            return false;
        }

        query = "UPDATE client SET email =  '" + data->email + "' WHERE login = '" + key + "'"; 
        break;

    case UPDATE_PASSWORD:
        query = "UPDATE client SET password = '" + data->hash + "' WHERE login = '" + key + "'";    
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

    default:
        return;
    }    
 
    client_cache_->Delete(key);
    client_cache_->Insert(key, *cache_data);
}
