#pragma once // NO_LINT
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <string>
#include "icontrollers.h"
#include "dbcontroller.hpp"
#include "api_stock.h"

class IColdStartHelper{
 public:
    virtual void updateData(controllers::IGetStocksController* prt_to_getstocks_controller) = 0;
};

class ColdStartHelper : public IColdStartHelper{
 public:
    ColdStartHelper(dbcontroller::IDataBaseController* ptr_to_database, api::IAPIStockRequest* ptr_to_api_stock_);
    void updateData(controllers::IGetStocksController* prt_to_getstocks_controller) override;
 
 private:
    std::time_t convertDateTimeToTimeT(const std::string& date_time_str);
    void getAndInsertData(handlers::ProtocolAPI protocol, std::string stock_name);
    dbcontroller::IDataBaseController* ptr_to_database_;
    api::IAPIStockRequest* ptr_to_api_stock_;
};