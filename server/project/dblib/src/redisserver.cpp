#include "redisserver.hpp"

using namespace database;

RedisServer::RedisServer(): host_("127:0:0:1"), port_(6369), password_("marketmentor_password") {
}

RedisServer::RedisServer(const std::string& host, int port, const std::string& password): 
        host_(host), port_(port), password_(password) {
}

bool RedisServer::IsOpen() {
    try {
        redis_->ping();    
    }
    catch (sw::redis::IoError const &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}

bool RedisServer::Connect() {
    sw::redis::ConnectionOptions connection_options;
    connection_options.host = "127.0.0.1"; 
    connection_options.port = 6379; 
//  connection_options.password = "auth"; 
    redis_ = std::make_shared<sw::redis::Redis>(connection_options);
    return IsOpen();
}

bool RedisServer::Insert(const std::string& key, const std::string& value, const size_t& ttl) {
    if (!IsOpen() || Has(key)) {
        return false;
    }

    try {
        redis_->set(key, value); 
        redis_->expire(key, std::chrono::seconds(ttl));
    }
    catch (sw::redis::Error const &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}


bool RedisServer::Delete(const std::string& key) {
    if (!IsOpen() || !Has(key)) {
        return false;
    }

    try {
        redis_->del(key);
    }
    catch (sw::redis::Error const &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}



bool RedisServer::Update(const std::string& key, const std::string& value, const size_t& ttl){
    if (!IsOpen() || !Has(key)) {
        return false;
    }

    try {
        redis_->del(key);
        redis_->set(key, value); 
        redis_->expire(key, std::chrono::seconds(ttl));
    }
    catch (sw::redis::Error const &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}

bool RedisServer::Has(const std::string& key) { 
    if (!IsOpen()) {
        return false;
    }

    sw::redis::Optional<std::string> value;
    try {
        value = redis_->get(key);
    }
    catch (sw::redis::Error const &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    if (!value.has_value()) {
        return false;
    }

    return true;
}

std::string RedisServer::Get(const std::string& key) {
    if (!IsOpen()) {
        return "NULL";
    }

    sw::redis::Optional<std::string> value;
    try {
        value = redis_->get(key);
    }
    catch (sw::redis::Error const &e) {
        std::cerr << e.what() << std::endl;
        return "NULL";
    }

    std::string result = value.value_or("NULL");
    return result;
}