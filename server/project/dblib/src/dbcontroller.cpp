#include "dbcontroller.hpp"

using namespace dbcontroller;

// DataBaseController::DataBaseController(): database_(new PostgresServer(host_addr_, port_, db_name_, user_, password_)), 
//         clien_rep_(new ClientRepository(database_)), timeseries_rep_(new TimeSeriesRepository(database_)), 
//         subscription_rep_(new SubscriptionRepository(database_)) {
// }
DataBaseController::DataBaseController() {
 }

DataBaseController::DataBaseController(const std::shared_ptr<IClientRepository>& client_rep, 
        const std::shared_ptr<ITimeSeriesRepository>& timeseries_rep, const std::shared_ptr<ISubscriptionRepository>& subscription_rep): 
        clien_rep_(client_rep), timeseries_rep_(timeseries_rep), subscription_rep_(subscription_rep), database_(nullptr) {
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

        if (request["TypeData"] == AUTHORIZE_DATA) {
            response = ClientRequestGet(request["login"].asString());
            if (response["status"] == true && response["password"] != request["password"]) {
                response["status"] = false;
            }
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
    if (data == nullptr || data->param == Json::Value::null) {
        response["status"] = false;
    }
    else {
        response["param"] = data->param; 
        response["status"] = true;
    
    }

    return response;
}


Json::Value DataBaseController::ClientRequestPost(const Json::Value& data) {
    Json::Value response;

    auto client_data = std::make_shared<ClientData>();
    client_data->login = data["login"].asString();
    client_data->hash_password = data["hash_password"].asString();
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
        response["password"] = data->hash_password;                
        response["email"] = data->email;                
    }

    return response;
}


Json::Value DataBaseController::ClientRequestUpdate(const Json::Value& data) {
    Json::Value response;   

    auto client_data = std::make_shared<ClientData>();
    client_data->login = data["login"].asString();
    client_data->hash_password = data["password"].asString();
    client_data->email = data["email"].asString();

    response["status"] = clien_rep_->Update(client_data->login, client_data);
    return response;
}