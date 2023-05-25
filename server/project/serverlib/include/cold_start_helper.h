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
    virtual void updateData(controllers::IGetStocksController* prt_to_getstocks_controller,
                                                api::IAPIStockRequest* ptr_to_api_stock) = 0;
};

class ColdStartHelper : public IColdStartHelper{
 public:
    ColdStartHelper(dbcontroller::IDataBaseController* ptr_to_database);
    void updateData(controllers::IGetStocksController* prt_to_getstocks_controller, 
                                api::IAPIStockRequest* ptr_to_api_stock) override;
 
 private:
    dbcontroller::IDataBaseController* ptr_to_database_;
    std::time_t convertDateTimeToTimeT(const std::string& date_time_str);
};