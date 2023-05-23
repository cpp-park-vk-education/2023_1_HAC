#pragma once // NO_LINT
#include <string>
#include <jsoncpp/json/json.h>

struct ClientData {
    std::string login;
    std::string hash;
    std::string email;
    size_t session_id;
    std::string token;
    std::string token_start_date;
    std::string token_finish_date;
};

struct TimeSeriesData {
    std::string name_stock;
    std::string date;
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