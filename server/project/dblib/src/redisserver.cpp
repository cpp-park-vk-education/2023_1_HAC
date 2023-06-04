#include <iostream>
#include "redisserver.hpp"


database::RedisServer::RedisServer(): host_("62.84.127.93"), port_(6369), password_("marketmentor_password") {
}

database::RedisServer::RedisServer(const std::string& host, int port, const std::string& password): 
        host_(host), port_(port), password_(password) {
}

bool database::RedisServer::IsOpen() {
    try {
        redis_->ping();    
    }
    catch (sw::redis::IoError const &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}

bool database::RedisServer::Connect() {
    sw::redis::ConnectionOptions connection_options;
    connection_options.host = host_; 
    connection_options.port = port_;
//  connection_options.password = password_; 
    redis_ = std::make_shared<sw::redis::Redis>(connection_options);
    return IsOpen();
}

bool database::RedisServer::Insert(const std::string& key, const std::string& value, const size_t& ttl) {
    if (Has(key)) {
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


bool database::RedisServer::Delete(const std::string& key) {
    if (!Has(key)) {
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



bool database::RedisServer::Update(const std::string& key, const std::string& value, const size_t& ttl){
    if (!Has(key)) {
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

bool database::RedisServer::Has(const std::string& key) { 
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

std::string database::RedisServer::Get(const std::string& key) {
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
