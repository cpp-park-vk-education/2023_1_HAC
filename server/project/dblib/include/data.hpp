#pragma once // NO_LINT
#include <iostream>
#include <jsoncpp/json/json.h>

struct ClientData {
    std::string login;
    std::string hash_password;
    std::string email;
};

struct AuthorizeData {
    std::string login;
    std::string hash_password;
};

struct TimeSeriesData {
    std::string name_stock;
    std::string date;
    Json::Value param;    
};

struct TimeSeriesRequest {
    std::string name_stock;
    size_t len_lags;
};

struct SubscriptionData {
    std::string name;
    size_t count;
    size_t cost;
};

struct AllSubscription {
    std::vector<SubscriptionData> data;
};
