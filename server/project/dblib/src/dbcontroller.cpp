#include "dbcontroller.hpp"

using namespace dbcontroller;

DataBaseController::DataBaseController() {
}

DataBaseController::DataBaseController(const std::shared_ptr<IClientRepository>& client_rep, 
        const std::shared_ptr<ITimeSeriesRepository>& timeseries_rep, const std::shared_ptr<ISubscriptionRepository>& subscription_rep): 
        clien_rep_(client_rep), timeseries_rep_(timeseries_rep), subscription_rep_(subscription_rep), database_(nullptr) {
}

bool DataBaseController::ConnectToDatabase() {
    database_ = std::make_shared<PostgresServer>(host_addr_, port_, db_name_, user_, password_);
    if (!database_->IsOpen()) {
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
    if (!database_->IsOpen()) {   
        response["DatabaseIsOpen"] = false;
        response["status"] = false;
        return response;
    }

    if (request["Type"] == GET_REQUEST) {
        // Получить данные пользователя
        if (request["TypeData"] == CLIENT_DATA) {
            response = ClientRequestGet(request["login"].asString());
        }

        // Получить таймсерии
        if (request["TypeData"] == TIMESERIES_REQUEST) {
            response = TimeSeriesGet(request["name_stock"].asString(), request["len_lags"].asString());
        }        
    }

    if (request["Type"] == POST_REQUEST) {
        // Добавить пользователя
        if (request["TypeData"] == CLIENT_DATA) {
            response = ClientRequestPost(request);
        }
        // Добавить таймсерию 
        if (request["TypeData"] == TIMESERIES_DATA) {
            response = TimeSeriesPost(request);
        }       
    }

    // Обновить пользователя
    if (request["Type"] == UPDATE_REQUEST) {
        if (request["TypeData"] == CLIENT_DATA) {
            response = ClientRequestUpdate(request); 
        }
    }
    
    response["DatabaseIsOpen"] = true;
    return response;
}


Json::Value DataBaseController::TimeSeriesPost(const Json::Value& data) {
    Json::Value response; 

    auto timeseries_data = std::make_shared<TimeSeriesData>();
    timeseries_data->name_stock = data["name_stock"].asString();
    timeseries_data->date = data["date"].asString();
    timeseries_data->param = data["param"];

    response["status"] = timeseries_rep_->Insert(timeseries_data);
    return response;
}


Json::Value DataBaseController::TimeSeriesGet(const std::string& name_stock, const std::string& len_lags) {
    Json::Value response;
    auto data = timeseries_rep_->GetByKey(name_stock, std::stoi(len_lags));
    if (data == nullptr || data->param.empty()) {
        response["status"] = false;
    }
    else {
        response["data"] = data->param; 
        response["status"] = true;
    }

    return response;
}


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
    client_data->login = data["login"].asString();
    client_data->hash = data["password"].asString();
    client_data->email = data["email"].asString();

    response["status"] = clien_rep_->Update(client_data->login, client_data);
    return response;
}