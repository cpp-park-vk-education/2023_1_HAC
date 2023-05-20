#include "dbcontroller.hpp"

#include <iostream>

using namespace dbcontroller;

DataBaseController::DataBaseController(): database_ (std::make_shared<PostgresServer>(host_addr_, port_, 
                                    db_name_, user_, password_)) {
}

DataBaseController::DataBaseController(const std::shared_ptr<IClientRepository>& client_rep, 
        const std::shared_ptr<ITimeSeriesRepository>& timeseries_rep, const std::shared_ptr<ISubscriptionRepository>& subscription_rep): 
        clien_rep_(client_rep), timeseries_rep_(timeseries_rep), subscription_rep_(subscription_rep), database_(nullptr) {
}

bool DataBaseController::ConnectToDatabase() {
    if (!database_->Connect()) {
        return false;
    }

    clien_rep_ = std::make_shared<ClientRepository>(database_);
    timeseries_rep_ = std::make_shared<TimeSeriesRepository>(database_);
    subscription_rep_ = std::make_shared<SubscriptionRepository>(database_);
    return true;
}


void DataBaseController::SetDatabaseConfig(const std::string&  addr, const std::string&  port,
         const std::string&  db_name, const std::string&  user, const std::string&  pass) {
    host_addr_ = addr;
    port_ = port;
    db_name_ = db_name;
    user_ = user;
    password_ = pass;
}


Json::Value DataBaseController::DataRequest(const Json::Value& request) {
    Json::Value response;
    
    if (!database_.get()) {
        response["DatabaseIsOpen"] = false;
        response["status"] = false;
        return response;
    }

    if (!database_->IsOpen()) {  
        response["DatabaseIsOpen"] = false;
        response["status"] = false;
        return response;
    }

    if (request["Type"] == GET_REQUEST) {
        GetRequestRouter(request, response);
    }

    if (request["Type"] == POST_REQUEST) {
        PostRequestRouter(request, response);
    }
    
    response["DatabaseIsOpen"] = true;
    return response;
}


void DataBaseController::GetRequestRouter(const Json::Value& request, Json::Value& response) {
    // Получить таймсерии
    if (request["TypeData"] == TIMESERIES_REQUEST) {
        response = TimeSeriesGet(request);
    }

    // Получить список акций
    if (request["TypeData"] == STOCKS_REQUEST) {
        response = StocksGet();
    }            
}


void DataBaseController::PostRequestRouter(const Json::Value& request, Json::Value& response) {
    // Получить данные пользователя
    if (request["TypeData"] == AUTHORIZATION) {
        response = ClientRequestGet(request["login"].asString());
    }

    // Добавить пользователя
    if (request["TypeData"] == REGISTRATION) {
        response = ClientRequestPost(request);
    }

    // Добавить таймсерию 
    if (request["TypeData"] == TIMESERIES_REQUEST) {
        response = TimeSeriesPost(request);
    }       
    
    // Обновить пользователя
    if (request["TypeData"] == CHANGE_USER_SETTINGS) {
        response = ClientRequestUpdate(request); 
    }    
}

// Таймсерии и акции 
Json::Value DataBaseController::TimeSeriesPost(const Json::Value& data) {
    Json::Value response; 

    auto timeseries_data = std::make_shared<TimeSeriesData>();
    timeseries_data->name_stock = data["name_stock"].asString();
    timeseries_data->date = data["date"].asString();
    timeseries_data->param = data["param"];

    response["status"] = timeseries_rep_->Insert(timeseries_data);
    return response;
}


Json::Value DataBaseController::TimeSeriesGet(const Json::Value& data) {
    Json::Value response;
    std::string name_stock = data["name_stock"].asString();
    size_t len_lags = std::stoi(data["len_lags"].asString());
    auto time_series = std::make_shared<TimeSeriesData>();

    if (data["start_date"] == Json::Value::null || data["finish_date"] == Json::Value::null) {
        time_series = timeseries_rep_->GetByKey(name_stock, len_lags);
    }
    else {
        std::string start_date = data["start_date"].asString();
        std::string finish_date = data["finish_date"].asString();
        time_series = timeseries_rep_->GetByKey(name_stock, len_lags, start_date, finish_date);
    }

    if (time_series == nullptr || time_series->param.empty()) {
        response["status"] = false;
    }
    else {
        response["data"] = time_series->param; 
        response["status"] = true;
    }

    return response;
}

Json::Value DataBaseController::StocksGet() {
    Json::Value response;
    auto data = timeseries_rep_->GetAllStocks();
    if (data == nullptr) {
        response["status"] = false;
    }
    else {
        response["param"] = data->list;
        response["status"] = true;
    } 

    return response;
}

// Клиент
Json::Value DataBaseController::ClientRequestPost(const Json::Value& data) {
    Json::Value response;

    auto client_data = std::make_shared<ClientData>();
    client_data->login = data["login"].asString();
    client_data->hash = data["password"].asString();
    client_data->email = data["email"].asString();

    response["status"] = clien_rep_->Insert(client_data);
    return response;
}


Json::Value DataBaseController::ClientRequestGet(const std::string& key) {
    Json::Value response;
    auto data = clien_rep_->GetByKey(key);
    if (data == nullptr) {
        response["status"] = false;
    }
    else {
        response["status"] = true;
        response["login"] = data->login;
        response["password"] = data->hash;                
        response["email"] = data->email;                
    }

    return response;
}


Json::Value DataBaseController::ClientRequestUpdate(const Json::Value& data) {
    Json::Value response;   

    auto client_data = std::make_shared<ClientData>();
    if (data["email"] == Json::Value::null) {
        client_data->email = "";   
    }
    else {
        client_data->email = data["email"].asString();
    }

    client_data->login = data["login"].asString();
    client_data->hash = data["password"].asString();

    response["status"] = clien_rep_->Update(client_data->login, client_data);
    return response;
}