#include "update_data_job.h"
#include <ctime>
#include <iomanip>
#include <chrono>
#include <iostream>
#include <thread>

void getNewDataByTimer(controllers::IGetStocksController* prt_to_get_stocks_controller, 
                        dbcontroller::IDataBaseController* ptr_to_database, 
                        api::IAPIStockRequest* ptr_to_api_stock) {
    std::time_t now = std::time(nullptr);
    std::tm* local_time_UTC = std::localtime(&now);
    std::time_t local_now = std::mktime(local_time_UTC);
    local_now += 3 * 3600;
    std::tm* local_time = std::localtime(&local_now);
    int time_to_sleep_first = 61 - local_time->tm_min; // 61 минута, чтобы ходить на биржу в 01 минуту следующего часа 
    std::this_thread::sleep_for(std::chrono::minutes(time_to_sleep_first));

    // сделать с конфигом
    handlers::ProtocolAPI protocol;
    protocol.name_stock_hub = "finnhub.io";
    protocol.token = "chmd799r01qjapkrdjngchmd799r01qjapkrdjo0";
    Json::Value stock_names_json = prt_to_get_stocks_controller->getNameStocks();
    std::cout << stock_names_json.toStyledString();
    std::vector<std::string> stock_names;
    for (auto i : stock_names_json["param"]){
        stock_names.push_back(i.asString()); 
    }

    while (true) {
        for (auto stock_name : stock_names) {
            std::time_t current_time = std::time(nullptr);
            std::time_t two_minutes_ago = current_time - 60 * 60;
            protocol.name_packet_stock = stock_name;
            protocol.start_time = std::to_string(two_minutes_ago);
            protocol.end_time = std::to_string(current_time);
            Json::Value actual_price = ptr_to_api_stock->getSeveralStockPrice(protocol);
            std::cout << actual_price;
            Json::Value db_protocol_to_fill;
            db_protocol_to_fill["Type"] = TypeRequest::POST_REQUEST;
            db_protocol_to_fill["TypeData"] = TypeData::TIMESERIES_FILL;
            db_protocol_to_fill["name_stock"] = stock_name;
            db_protocol_to_fill["param"] = actual_price["data"];
            db_protocol_to_fill["date"] = actual_price["date"];
            ptr_to_database->DataRequest(db_protocol_to_fill);
        }
        std::this_thread::sleep_for(std::chrono::minutes(60));
    }
}

