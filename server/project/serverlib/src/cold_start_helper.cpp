#include "cold_start_helper.h"
#include <jsoncpp/json/value.h>

ColdStartHelper::ColdStartHelper(dbcontroller::IDataBaseController* ptr_to_database):
                            ptr_to_database_ (ptr_to_database){};

void ColdStartHelper::updateData(controllers::IGetStocksController* prt_to_getstocks_controller, 
                                                        api::IAPIStockRequest* ptr_to_api_stock){
    // Time init
    std::time_t now = std::time(nullptr);
    std::tm* local_time_UTC = std::localtime(&now);
    std::time_t local_now = std::mktime(local_time_UTC);
    local_now += 3 * 3600;
    std::tm* local_time = std::localtime(&local_now);

    Json::Value stock_names_json = prt_to_getstocks_controller->getNameStocks();
    std::cout << stock_names_json.toStyledString();
    std::vector<std::pair<std::string, std::string>> stock_names;
    // Парснг json в вектор
    // Переделать под конфиг
    handlers::ProtocolAPI protocol;
    protocol.name_stock_hub = "finnhub.io";
    protocol.token = "chmd799r01qjapkrdjngchmd799r01qjapkrdjo0";
    for (auto i : stock_names) {
        std::time_t stock_update_prev_time = convertDateTimeToTimeT(i.second);
        protocol.name_packet_stock = i.first;
        protocol.end_time = std::to_string(std::mktime(local_time));
        if (stock_update_prev_time - std::mktime(local_time) < 60 * 60 * 24 * 30){
            protocol.start_time = std::to_string(stock_update_prev_time);
        } else {
            protocol.start_time = std::to_string(std::mktime(local_time) - 60 * 60 * 24 * 30);
        }
        //Json::Value actual_price = ptr_to_api_stock->getSeveralStockPrice(protocol);
    }
};

std::time_t ColdStartHelper::convertDateTimeToTimeT(const std::string& date_time_str) {
    std::tm tm = {};
    std::istringstream ss(date_time_str);

    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    std::time_t time = std::mktime(&tm);

    return time;
}