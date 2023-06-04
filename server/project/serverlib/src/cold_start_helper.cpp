#include "cold_start_helper.h"
#include <jsoncpp/json/value.h>
#include <fstream>
#include <thread>
#include "logger.h"

const std::string path_to_default_stocks = "../../project/utils/default_stocks.txt";
const int OFFSET_HOURS = 3;
const std::string JSON_STOCK_NAMES_STATUS = "status";
const std::string JSON_STOCK_NAMES_DATA = "param";
const std::string DB_PROTOCOL_REQUEST_TYPE = "Type";
const std::string DB_PROTOCOL_DATA_TYPE = "TypeData";
const std::string DB_PROTOCOL_NAME_STOCK = "name_stock";
const std::string DB_PROTOCOL_LEN_LAGS = "len_lags";
const std::string DB_PROTOCOL_DATE = "date";
const std::string DB_RESPONSE_DATA = "param";
const std::string STOCK_REUQEST_DATA = "data";
const std::string STOCK_REUQEST_DATE = "date";
const std::string MODEL_FIT_ACTION = "fit";
FileLogger& logger = FileLogger::getInstance(); 

ColdStartHelper::ColdStartHelper(dbcontroller::IDataBaseController* ptr_to_database,
                                api::IAPIStockRequest* ptr_to_api_stock, api::IAPIModelRequest* ptr_to_api_model):
                                ptr_to_database_ (ptr_to_database),
                                ptr_to_api_stock_ (ptr_to_api_stock),
                                ptr_to_api_model_ (ptr_to_api_model) {};

void ColdStartHelper::updateData(controllers::IGetStocksController* prt_to_getstocks_controller){
    // Time init
    logger.log("Cold start activated");
    std::tm* local_time = getLocalTime();

    Json::Value stock_names_json = prt_to_getstocks_controller->getNameStocks();
    std::vector<std::string> stock_names;

    if (stock_names_json[JSON_STOCK_NAMES_STATUS] == false){
        std::ifstream config_stream (path_to_default_stocks);
        if (!config_stream) {
           throw market_mentor::InvalidServerConfig("Wrong default stock config path");
        }
        std::string input;
        while (config_stream >> input){
            stock_names.push_back(input);
        }
    }

    for (auto i : stock_names_json[JSON_STOCK_NAMES_DATA]){
        stock_names.push_back(i.asString()); 
    }

    handlers::ProtocolAPI protocol;
    for (auto stock_name : stock_names) {
        Json::Value db_protocol;
        std::string stock_time;
        try{
            if (stock_names_json[JSON_STOCK_NAMES_STATUS] == true) {
                db_protocol[DB_PROTOCOL_REQUEST_TYPE] = TypeRequest::GET_REQUEST;
                db_protocol[DB_PROTOCOL_DATA_TYPE] = TypeData::TIMESERIES_REQUEST;
                db_protocol[DB_PROTOCOL_NAME_STOCK] = stock_name;
                db_protocol[DB_PROTOCOL_LEN_LAGS] = 1;
                Json::Value db_response = ptr_to_database_->DataRequest(db_protocol);
                stock_time = db_response[DB_PROTOCOL_DATE][0].asString();
            } else {
                stock_time = std::to_string(std::mktime(local_time) - 60 * 60 * 24 * 29 * 14);
            } 
        } catch (std::exception& e) {
            logger.log(std::string(std::string("Faled to get stock names in cold start: ") + e.what()));
            throw market_mentor::ColdStartException();
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
            try{
                protocol.start_time = std::to_string(start_update_time);
                protocol.end_time = std::to_string(start_update_time + 60 * 60 * 24 * 29);
                getAndInsertData(protocol, stock_name);
                start_update_time += 60 * 60 * 24 * 29;
            } catch(std::exception& e){
                logger.log(std::string(std::string("Faled to update stock prices in cold start: ") + e.what()));
                throw market_mentor::ColdStartException();
            }
        }
        // У одного токена максимум 10 запросов в секунду
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    for (auto stock_name : stock_names) {
        controllers::TimeSeriesPredicts* samples_data = new controllers::TimeSeriesPredicts;
        Json::Value db_protocol;
        try{
            db_protocol[DB_PROTOCOL_REQUEST_TYPE] = TypeRequest::GET_REQUEST;
            db_protocol[DB_PROTOCOL_DATA_TYPE] = TypeData::TIMESERIES_REQUEST;
            db_protocol[DB_PROTOCOL_NAME_STOCK] = stock_name;
            db_protocol[DB_PROTOCOL_LEN_LAGS] = 3945;
            Json::Value db_response = ptr_to_database_->DataRequest(db_protocol);

            samples_data->stock_name = stock_name;
            samples_data->action = MODEL_FIT_ACTION;
            for (int i = 0; i < db_response[DB_RESPONSE_DATA].size(); ++i) {
                samples_data->matrix_samples.push_back(std::stod(db_response[DB_RESPONSE_DATA][i].asString()));
            }
            ptr_to_api_model_->getData(*samples_data);
        } catch (std::exception& e){
            logger.log(std::string(std::string("Faled to fit model in cold start: ") + e.what()));
            throw market_mentor::ColdStartException();
        }
    }
};

void ColdStartHelper::getAndInsertData(handlers::ProtocolAPI protocol, std::string stock_name){
    Json::Value actual_price = ptr_to_api_stock_->getSeveralStockPrice(protocol);
    Json::Value db_protocol_to_fill;
    db_protocol_to_fill[DB_PROTOCOL_REQUEST_TYPE] = TypeRequest::POST_REQUEST;
    db_protocol_to_fill[DB_PROTOCOL_DATA_TYPE] = TypeData::TIMESERIES_FILL;
    db_protocol_to_fill[DB_PROTOCOL_NAME_STOCK] = stock_name;
    db_protocol_to_fill[DB_RESPONSE_DATA] = actual_price[STOCK_REUQEST_DATA];
    db_protocol_to_fill[DB_PROTOCOL_DATE] = actual_price[STOCK_REUQEST_DATE];
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

std::tm* ColdStartHelper::getLocalTime(){
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::tm* local_time_UTC = std::localtime(&now_time);

    auto local_now = std::chrono::system_clock::from_time_t(now_time);
    local_now += std::chrono::hours(OFFSET_HOURS);

    std::time_t local_now_time = std::chrono::system_clock::to_time_t(local_now);
    return std::localtime(&local_now_time);
}