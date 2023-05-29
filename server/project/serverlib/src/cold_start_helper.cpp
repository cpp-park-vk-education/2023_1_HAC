#include "cold_start_helper.h"
#include <jsoncpp/json/value.h>
#include <fstream>
#include <thread>

ColdStartHelper::ColdStartHelper(dbcontroller::IDataBaseController* ptr_to_database,
                                api::IAPIStockRequest* ptr_to_api_stock, api::IAPIModelRequest* ptr_to_api_model):
                                ptr_to_database_ (ptr_to_database),
                                ptr_to_api_stock_ (ptr_to_api_stock),
                                ptr_to_api_model_ (ptr_to_api_model) {};

void ColdStartHelper::updateData(controllers::IGetStocksController* prt_to_getstocks_controller){
    // Time init
    std::string path_to_config_file = "../../project/utils/default_stocks.txt";
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm* local_time_UTC = std::localtime(&now_time);

    auto local_now = std::chrono::system_clock::from_time_t(now_time);
    local_now += std::chrono::hours(3);

    std::time_t local_now_time = std::chrono::system_clock::to_time_t(local_now);
    std::tm* local_time = std::localtime(&local_now_time);

    Json::Value stock_names_json = prt_to_getstocks_controller->getNameStocks();
    std::vector<std::string> stock_names;

    if (stock_names_json["status"] == false){
        std::ifstream config_stream (path_to_config_file);
        std::string n;
        std::string input;
        while (config_stream >> input){
            stock_names.push_back(input);
        }
    }
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
        std::string stock_time;
        if (stock_names_json["status"] == true) {
            db_protocol["Type"] = TypeRequest::GET_REQUEST;
            db_protocol["TypeData"] = TypeData::TIMESERIES_REQUEST;
            db_protocol["name_stock"] = stock_name;
            db_protocol["len_lags"] = 1;
            Json::Value db_response = ptr_to_database_->DataRequest(db_protocol);
            stock_time = db_response["date"][0].asString();
        } else {
            stock_time = std::to_string(std::mktime(local_time) - 60 * 60 * 24 * 29 * 14);
        }
        std::time_t stock_update_prev_time = convertDateTimeToTimeT(stock_time);
        protocol.name_packet_stock = stock_name;
        // Апи биржи даёт вытащить значения только за 30 дней
        auto p1 = std::chrono::system_clock::now();
        auto now_time_unix = std::chrono::duration_cast<std::chrono::seconds>(
                                                p1.time_since_epoch()).count();
        auto start_update_time = stock_update_prev_time + 1;
        if (now_time_unix - stock_update_prev_time > 60 * 60 * 24 * 29 * 13) {
            start_update_time = now_time_unix - 60 * 60 * 24 * 29 * 13;
        };

        while (start_update_time < now_time_unix) {
            protocol.start_time = std::to_string(start_update_time);
            protocol.end_time = std::to_string(start_update_time + 60 * 60 * 24 * 29);
            getAndInsertData(protocol, stock_name);
            start_update_time += 60 * 60 * 24 * 29;
        }
        // У одного токена максимум 10 запросов в секунду
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    controllers::TimeSeriesPredicts samples_data;
    for (auto stock_name : stock_names) {
        Json::Value db_protocol;
        db_protocol["Type"] = TypeRequest::GET_REQUEST;
        db_protocol["TypeData"] = TypeData::TIMESERIES_REQUEST;
        db_protocol["name_stock"] = stock_name;
        db_protocol["len_lags"] = 3945;
        Json::Value db_response = ptr_to_database_->DataRequest(db_protocol);

        samples_data.stock_name = stock_name;
        samples_data.action = "fit";
        for (int i = 0; i < db_response["param"].size(); ++i) {
            samples_data.matrix_samples.push_back(std::stod(db_response["param"][i].asString()));
        }
        ptr_to_api_model_->getData(samples_data);
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
};

void parseStockConfig() {
    std::string path_to_config_file = "../../project/utils/stock_config.txt";
}