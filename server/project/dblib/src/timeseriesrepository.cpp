#include <iostream>
#include "repositories.hpp"
#include "repositorycache.hpp"

const int kTimeSeriesCacheSize = 5;
const int kStocksCacheSize = 1;

repository::TimeSeriesRepository::TimeSeriesRepository(): database_(nullptr), 
        timeseries_cache_(std::make_shared<cache::RepositoryCache<std::string, TimeSeriesData>>(kTimeSeriesCacheSize)),
        stocks_cache_(std::make_shared<cache::RepositoryCache<std::string, AllStocks>>(kStocksCacheSize)) {
}


repository::TimeSeriesRepository::TimeSeriesRepository(const std::shared_ptr<database::IDataBase>& db): database_(db),
        timeseries_cache_(std::make_shared<cache::RepositoryCache<std::string, TimeSeriesData>>(kTimeSeriesCacheSize)),
        stocks_cache_(std::make_shared<cache::RepositoryCache<std::string, AllStocks>>(kStocksCacheSize)) {
}


bool repository::TimeSeriesRepository::Insert(const std::shared_ptr<TimeSeriesData>& data){
    if (!database_->IsOpen()) {
        return false;
    }    
    
    Json::FastWriter json_to_string;
    std::string query = "INSERT into timeseries(name, date_, param) VALUES ('" + 
        data->name_stock + "', '" + data->date[0].asString() + "', " + data->param[0].asString() + ")";

    bool result = database_->SendQuery(query);
    if (result) {
        timeseries_cache_->DeleteAll();
        stocks_cache_->DeleteAll();
    }

    return result;    
}

bool repository::TimeSeriesRepository::InsertArray(const std::shared_ptr<TimeSeriesData>& data) {
    if (!database_->IsOpen()) {
        return false;
    }    
    
    std::string query = "INSERT into timeseries(name, date_, param) VALUES";
    std::string values;
    for (int i = 0; i < data->param.size(); i++) {
        if (data->date[i] == Json::Value::null || data->param[i] == Json::Value::null) {
            return false;
        }

        values = "( '" + data->name_stock + "' , '" + data->date[i].asString() + "' , " + data->param[i].asString() + " ) ";
        if (i != data->param.size() - 1) {
            values += ",";  
        }
        else{
            values += ";";
        }

        query += values;
    }

    bool result = database_->SendQuery(query);
    if (result) {
        timeseries_cache_->DeleteAll();
        stocks_cache_->DeleteAll();
    }
    return result;
}


std::shared_ptr<TimeSeriesData> repository::TimeSeriesRepository::TimeSeriesResponseParse(const Json::Value& db_response) {
    const int kTimeSeriesId = 0;
    const int kNameId = 1;
    const int kParamId = 3;
    const int kDateId = 2;

    auto result = std::make_shared<TimeSeriesData>();
    Json::Value json_param, json_date;
    Json::Reader reader;
    std::string number;
    for (int i = 0; i < db_response.size(); i++) {  
        if (db_response[i][kParamId] != Json::Value::null) {
            json_date[i] = db_response[i][kDateId].asString();
            number = db_response[i][kParamId].asString();
            try {
               json_param[i] = std::stod(number);
            }
            catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                return nullptr;
            }
        }
    };

    result->param = json_param;
    result->date = json_date;
    return result;
}


std::shared_ptr<TimeSeriesData> repository::TimeSeriesRepository::GetByKey(const std::string& name_stock,
                                const size_t& len_lags, const std::string& start_date /* = "" */, 
                                const std::string& finish_date /* = "" */) {

    std::string key = name_stock + "::" + std::to_string(len_lags);
    if (timeseries_cache_->Has(key)) {
        auto result = std::make_shared<TimeSeriesData>(timeseries_cache_->Get(key));
        return result;    
    }

    if (!database_->IsOpen()) {
        return nullptr;
    }

    std::string query;
    if (start_date == "" || finish_date == "")
        query = "SELECT * FROM timeseries where name = '" + name_stock 
                + "' ORDER by timeseries_id DESC LIMIT " + std::to_string(len_lags);
    else {
        query = " SELECT * FROM timeseries where name = '" + name_stock + "' and date_ > '" + start_date + "' and date_ < '" +
                finish_date + "' ORDER by timeseries_id DESC LIMIT " + std::to_string(len_lags);       
    }

    Json::Value buffer;
    try {
        buffer = database_->GetData(query);
    }
    catch(ConnectError const &e) {
        return nullptr;
    }
    catch(ElementNotExist const &e) {
        return nullptr;
    }
    catch(SqlError const &e) {
        return nullptr;
    }

    auto result = TimeSeriesResponseParse(buffer);
    result->name_stock = name_stock;
    key = name_stock + "::" + std::to_string(name_stock.size());
    timeseries_cache_->Insert(key, *result);
    return result;
}

std::shared_ptr<AllStocks> repository::TimeSeriesRepository::StocksResponseParse(const Json::Value& db_response) {
    if (db_response.size() == 0) {
        return nullptr;
    }

    auto result = std::make_shared<AllStocks>();
    Json::Value json_param;
    Json::Reader reader;

    for (int i = 0; i < db_response.size(); i++) {  
        if (db_response[i] != Json::Value::null) {
            json_param[i] = db_response[i][0].asString();
        }
    };

    result->list = json_param;
    return result;
}

std::shared_ptr<AllStocks> repository::TimeSeriesRepository::GetAllStocks() { 
    if (!database_->IsOpen()) {
        return nullptr;
    }   
    
    std::string key = "list";
    if (stocks_cache_->Has(key)) {
        auto result = std::make_shared<AllStocks>(stocks_cache_->Get(key));
        return result;    
    }

    std::string query = "SELECT DISTINCT name FROM timeseries";
    Json::Value buffer;
    try {
        buffer = database_->GetData(query);
    }
    catch(ConnectError const &e) {
        return nullptr;
    }
    catch(ElementNotExist const &e) {
        return nullptr;
    }
    catch(SqlError const &e) {
        return nullptr;
    }

    auto result = StocksResponseParse(buffer);
    if (result != nullptr) {
        stocks_cache_->Insert(key, *result);
    }
    
    return result;
}
