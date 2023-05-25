#pragma once // NO_LINT
#include <string>
#include <jsoncpp/json/json.h>

struct ClientData {
    std::string login;
    std::string hash;
    std::string email;
};

struct TimeSeriesData {
    std::string name_stock;
    Json::Value date;
    Json::Value param;    
};

struct TimeSeriesRequest {
    std::string name_stock;
    size_t len_lags;
    std::string start_date;
    std::string finish_date;
};

struct AllStocks {
    Json::Value list;    
};

struct SubscriptionData {
    std::string name;
    size_t count;
    size_t cost;
};

struct AllSubscription {
    Json::Value list;
};

struct TokenData {
    std::string login;
    std::string token;
    size_t time_live;
};