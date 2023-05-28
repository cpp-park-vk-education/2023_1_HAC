#pragma once // NO_LINT
#include <iostream>
#include <thread>
#include <chrono>
#include "icontrollers.h"
#include "dbcontroller.hpp"
#include "api_stock.h"

void getNewDataByTimer(controllers::IGetStocksController* prt_to_getstocks_controller, 
                        dbcontroller::IDataBaseController* ptr_to_database, 
                        api::IAPIStockRequest* ptr_to_api_stock);