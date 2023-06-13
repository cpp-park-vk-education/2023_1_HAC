#include "update_data_job.h"
#include <ctime>
#include <iomanip>
#include <chrono>
#include <iostream>
#include <thread>
#include "logger.h"
#include "server_exception.h"

const std::string JSON_STOCK_NAMES_DATA = "param";
const std::string DB_PROTOCOL_REQUEST_TYPE = "Type";
const std::string DB_PROTOCOL_DATA_TYPE = "TypeData";
const std::string DB_PROTOCOL_NAME_STOCK = "name_stock";
const std::string DB_PROTOCOL_LEN_LAGS = "len_lags";
const std::string DB_PROTOCOL_DATE = "date";
const std::string DB_RESPONSE_DATA = "param";
const std::string STOCK_REUQEST_DATA = "data";
const std::string STOCK_REUQEST_DATE = "date";
const int SLEEP_MINUTES = 60;

void getNewDataByTimer(controllers::IGetStocksController* prt_to_get_stocks_controller, 
                        dbcontroller::IDataBaseController* ptr_to_database, 
                        api::IAPIStockRequest* ptr_to_api_stock) {
    FileLogger& logger = FileLogger::getInstance(); 
    std::time_t now = std::time(nullptr);
    std::tm* local_time_UTC = std::localtime(&now);
    std::time_t local_now = std::mktime(local_time_UTC);
    local_now += 3 * 3600;
    std::tm* local_time = std::localtime(&local_now);
    int time_to_sleep_first = 61 - local_time->tm_min; // 61 минута, чтобы ходить на биржу в 01 минуту следующего часа 
    
    std::vector<std::string> stock_names;
    handlers::ProtocolAPI protocol;

    try{
        Json::Value stock_names_json = prt_to_get_stocks_controller->getNameStocks();
        std::cout << stock_names_json.toStyledString();
        for (auto i : stock_names_json[JSON_STOCK_NAMES_DATA]){
            stock_names.push_back(i.asString()); 
        }
    } catch(std::exception& e) {
        logger.log(std::string(std::string("Faled to get stock names in update data job: ") + e.what()));
        throw market_mentor::UpdateDataException();
    }

    logger.log("Update data job created successfully. Wating for " + std::to_string(time_to_sleep_first) + " minutes");
    std::this_thread::sleep_for(std::chrono::minutes(time_to_sleep_first));

    while (true) {
        for (auto stock_name : stock_names) {
            logger.log("Updating data...");
            try{
                std::time_t current_time = std::time(nullptr);
                std::time_t two_minutes_ago = current_time - 60 * 60;
                protocol.name_packet_stock = stock_name;
                protocol.start_time = std::to_string(two_minutes_ago);
                protocol.end_time = std::to_string(current_time);
                Json::Value actual_price = ptr_to_api_stock->getSeveralStockPrice(protocol);
                std::cout << actual_price;
                Json::Value db_protocol_to_fill;
                db_protocol_to_fill[DB_PROTOCOL_REQUEST_TYPE] = TypeRequest::POST_REQUEST;
                db_protocol_to_fill[DB_PROTOCOL_DATA_TYPE] = TypeData::TIMESERIES_FILL;
                db_protocol_to_fill[DB_PROTOCOL_NAME_STOCK] = stock_name;
                db_protocol_to_fill[DB_RESPONSE_DATA] = actual_price[STOCK_REUQEST_DATA];
                db_protocol_to_fill[DB_PROTOCOL_DATE] = actual_price[STOCK_REUQEST_DATE];
                ptr_to_database->DataRequest(db_protocol_to_fill);
            } catch(std::exception& e) {
                logger.log(std::string("Faled to update data: ") + e.what());
            }
        }
        std::this_thread::sleep_for(std::chrono::minutes(SLEEP_MINUTES));
    }
}

