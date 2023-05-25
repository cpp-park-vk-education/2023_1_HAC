#include "cold_start_helper.h"
#include <jsoncpp/json/value.h>

ColdStartHelper::ColdStartHelper(dbcontroller::IDataBaseController* ptr_to_database,
                                api::IAPIStockRequest* ptr_to_api_stock):
                                ptr_to_database_ (ptr_to_database),
                                ptr_to_api_stock_ (ptr_to_api_stock) {};

void ColdStartHelper::updateData(controllers::IGetStocksController* prt_to_getstocks_controller){
    // Time init
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm* local_time_UTC = std::localtime(&now_time);

    auto local_now = std::chrono::system_clock::from_time_t(now_time);
    local_now += std::chrono::hours(3);

    std::time_t local_now_time = std::chrono::system_clock::to_time_t(local_now);
    std::tm* local_time = std::localtime(&local_now_time);

    Json::Value stock_names_json = prt_to_getstocks_controller->getNameStocks();
    std::cout << stock_names_json.toStyledString();
    std::vector<std::string> stock_names;
    for (auto i : stock_names_json["param"]){
        stock_names.push_back(i.asString()); 
    }
    // Парснг json в вектор
    // Переделать под конфиг
    handlers::ProtocolAPI protocol;
    protocol.name_stock_hub = "finnhub.io";
    protocol.token = "chmd799r01qjapkrdjngchmd799r01qjapkrdjo0";
    for (auto stock_name : stock_names) {
        Json::Value db_protocol;
        db_protocol["Type"] = TypeRequest::GET_REQUEST;
        db_protocol["TypeData"] = TypeData::TIMESERIES_REQUEST;
        db_protocol["name_stock"] = stock_name;
        db_protocol["len_lags"] = 1;
        Json::Value db_response = ptr_to_database_->DataRequest(db_protocol);
        std::string stock_time = db_response["date"][0].asString();
        std::time_t stock_update_prev_time = convertDateTimeToTimeT(stock_time);
        protocol.name_packet_stock = stock_name;
        // Апи биржи даёт вытащить значения только за последние 30 дней
        if (std::mktime(local_time) - stock_update_prev_time < 60 * 60 * 24 * 30){
            std::cerr << "ABBA" << stock_update_prev_time - std::mktime(local_time);
            protocol.start_time = std::to_string(stock_update_prev_time + 1);
            protocol.end_time = std::to_string(std::mktime(local_time));
            getAndInsertData(protocol, stock_name);
        } else {
            if (std::mktime(local_time) - stock_update_prev_time  < 60 * 60 * 24 * 60){
                protocol.start_time = std::to_string(stock_update_prev_time);
                protocol.end_time = std::to_string(stock_update_prev_time + 60 * 60 * 24 * 30);
                getAndInsertData(protocol, stock_name);
                protocol.start_time = std::to_string(stock_update_prev_time + 60 * 60 * 24 * 30);
                protocol.end_time = std::to_string(std::mktime(local_time));
                getAndInsertData(protocol, stock_name);
            } else {
                protocol.start_time = std::to_string(std::mktime(local_time) - 60 * 60 * 24 * 90);
                protocol.end_time = std::to_string(std::mktime(local_time) - 60 * 60 * 24 * 60);
                 std::cerr << "@@@" << std::ctime(&now_time) << " - " <<  60 * 60 * 24 * 90 << " = "<< protocol.start_time << " " << protocol.end_time << "@@@"<< std::endl;
                 std::cerr << "@@@" << std::ctime(&now_time) << " - " <<  60 * 60 * 24 * 90 << " = "<< protocol.start_time << " " << protocol.end_time << "@@@"<< std::endl;
                getAndInsertData(protocol, stock_name);
                std::cerr << "@@@" << std::ctime(&now_time) << " - ";
                protocol.start_time = std::to_string(std::mktime(local_time) - 60 * 60 * 24 * 60);
                protocol.end_time = std::to_string(std::mktime(local_time) - 60 * 60 * 24 * 30);
                std::cerr << "@@@" << std::mktime(local_time_UTC) << " - " <<  60 * 60 * 24 * 60 << " = "<< protocol.start_time << " " << protocol.end_time << "@@@"<< std::endl;
                getAndInsertData(protocol, stock_name);
                //protocol.start_time = std::to_string(std::mktime(local_time) - 60 * 60 * 24 * 30);
                //protocol.end_time = std::to_string(std::mktime(local_time));
                //getAndInsertData(protocol, stock_name);
            }
        }
        // У одного токена максимум 10 запросов в секунду
        // std::sleep_for(std::chrono::seconds(1));
    }
};

void ColdStartHelper::getAndInsertData(handlers::ProtocolAPI protocol, std::string stock_name){
    Json::Value actual_price = ptr_to_api_stock_->getSeveralStockPrice(protocol);
    Json::Value db_protocol_to_fill;
    db_protocol_to_fill["Type"] = TypeRequest::POST_REQUEST;
    db_protocol_to_fill["TypeData"] = TypeData::TIMESERIES_FILL;
    db_protocol_to_fill["name_stock"] = stock_name;
    db_protocol_to_fill["param"] = actual_price["data"];
    db_protocol_to_fill["date"] = actual_price["date"];
    ptr_to_database_->DataRequest(db_protocol_to_fill);
};

std::time_t ColdStartHelper::convertDateTimeToTimeT(const std::string& date_time_str) {
    std::tm tm = {};
    std::istringstream ss(date_time_str);

    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    std::chrono::seconds utc_offset(0);
    utc_offset = std::chrono::hours(-3);
    std::time_t time = std::mktime(&tm)  + utc_offset.count();

    return time;
}