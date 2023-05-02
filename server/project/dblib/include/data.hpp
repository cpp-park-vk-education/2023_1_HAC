#pragma once
#include <iostream>
#include <jsoncpp/json/json.h>

struct ClientData {
 //   ClientData();
    std::string login;
    std::string hash_password;
    std::string email;
//     std::string status;
//     std::string date;
//     std::string validity;   
};

struct AuthorizeData {
   // AuthorizeData();
    std::string login;
    std::string hash_password;
};

struct TimeSeriesData {
  //  TimeSeriesData();
    std::string name_stock;
    std::string date;
    Json::Value param;    
};

struct TimeSeriesRequest {
//    TimeSeriesRequest();
    std::string name_stock;
    size_t len_lags;
};

struct SubscriptionData {
  //  SubscriptionData();
    std::string name;
    size_t count;
    size_t cost;
};

struct AllSubscription {
    std::vector<SubscriptionData> data;
};

// struct ClientData: public IData {
//     ClientData();
//     int id;
//     std::string username;
//     std::string email;
//     std::string password;
//     std::string local_salt;
//     std::string status;
//     std::string date;
//     std::string validity;    
// };

// struct TimeSeriesData: public IData {
//     TimeSeriesData();
//     int id;
//     std::string name;
//     std::string date;
//     Json::Value param;
// };

// struct SubscriptionData: public IDataRequest {
//     SubscriptionData();
//     std::string name;
//     int count;
//     int cost;
// };
