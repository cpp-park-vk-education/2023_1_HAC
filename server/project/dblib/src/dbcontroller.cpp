#include "dbcontroller.hpp"

#include <iostream>

using namespace dbcontroller;

DataBaseController::DataBaseController(): postgres_database_ (std::make_shared<PostgresServer>(postgres_host_addr_, 
                postgres_port_, postgres_db_name_, postgres_user_, postgres_password_)), 
                redis_database_(std::make_shared<RedisServer>(redis_host_addr_, redis_port_, redis_password_)) {
}

DataBaseController::DataBaseController(const std::shared_ptr<IDataBase>& sql_db, const std::shared_ptr<IMemoryDataBase>& no_sqldb,
        const std::shared_ptr<IClientRepository>& client_rep, const std::shared_ptr<ITimeSeriesRepository>& timeseries_rep, 
        const std::shared_ptr<ISubscriptionRepository>& subscription_rep): 
        postgres_database_(sql_db), redis_database_(no_sqldb), clien_rep_(client_rep), timeseries_rep_(timeseries_rep), 
        subscription_rep_(subscription_rep) {
}

bool DataBaseController::ConnectToDatabase() {
    if (!postgres_database_->Connect()) {
        std::cerr << "Can't connect to Postgres" << std::endl;
        return false;
    }

    if (!redis_database_->Connect()) {
        std::cerr << "Can't connect to Redis" << std::endl;
        return false;
    }

    clien_rep_ = std::make_shared<ClientRepository>(postgres_database_);
    timeseries_rep_ = std::make_shared<TimeSeriesRepository>(postgres_database_);
    subscription_rep_ = std::make_shared<SubscriptionRepository>(postgres_database_);
    token_rep_ = std::make_shared<TokenRepository>(redis_database_);
    return true;
}


void DataBaseController::SetDatabaseConfig(const std::string&  addr, const std::string&  port,
         const std::string&  db_name, const std::string&  user, const std::string&  pass) {
    postgres_host_addr_ = addr;
    postgres_port_ = port;
    postgres_db_name_ = db_name;
    postgres_user_ = user;
    postgres_password_ = pass;
}

void DataBaseController::SetMemoryDatabaseConfig(const std::string& addr, const std::string&  port, const std::string&  pass) {
    redis_host_addr_ = addr;
    redis_password_ = pass;
    try {
        redis_port_ = std::stoi(port);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

}


Json::Value DataBaseController::DataRequest(const Json::Value& request) noexcept {
    Json::Value response; 
    if (!postgres_database_.get()) {
        response["DatabaseIsOpen"] = false;
        response["status"] = false;
        return response;
    }

    if (!postgres_database_->IsOpen()) {  
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

    // Получить токен
    if (request["TypeData"] == SESSION_REQUEST) {
        // std::string key = response["token"].asString();
        // TypeData type = SESSION_REQUEST;
        // response = ClientRequestGet(type, key);
        response = TokenRequestGet(request);
    }  

    // Удалить токен
    if (request["TypeData"] == SESSION_DELETE) {
        std::string key = request["token"].asString();
        response = TokenRequestDelete(key);
    }               
}


void DataBaseController::PostRequestRouter(const Json::Value& request, Json::Value& response) {
    // Получить данные пользователя
    if (request["TypeData"] == AUTHORIZATION) {
        std::string key = response["token"].asString();
        TypeData type = AUTHORIZATION;
        response = ClientRequestGet(type, request["login"].asString());
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
    if (request["TypeData"] == CHANGE_USER_EMAIL_SETTINGS || 
                request["TypeData"] == CHANGE_USER_PASSWORD_SETTINGS) {
        response = ClientRequestUpdate(request); 
    }    

    // Добавить токен
    if (request["TypeData"] == SESSION_REQUEST) {
        response = TokenRequestPost(request);
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
    // client_data->session_id = std::stoi(data["session_id"].asString());
    // client_data->token = data["token"].asString();
    // client_data->token_start_date = data["token_start_data"].asString();
    // client_data->token_finish_date = data["token_finish_data"].asString();

    response["status"] = clien_rep_->Insert(client_data);
    return response;
}


Json::Value DataBaseController::ClientRequestGet(const TypeData& request_type, const std::string& key) {
    Json::Value response;
    ClientGetType type;
    // if (request_type == SESSION_REQUEST) {
    //     type = TOKEN_KEY;
    // }
    if (request_type == AUTHORIZATION) {
        type = LOGIN_KEY;
    }

    auto data = clien_rep_->GetByKey(type, key);
    if (data == nullptr) {
        response["status"] = false;
    }
    else {
        response["status"] = true;
        response["login"] = data->login;
        response["password"] = data->hash;                
        response["email"] = data->email;    
        // response["session_id"] = data->session_id;
        // response["token"] = data->token;
        // response["token_start_date"] = data->token_start_date;
        // response["token_finish_date"] = data->token_finish_date;

    }

    return response;
}

Json::Value DataBaseController::ClientRequestUpdate(const Json::Value& data) {
    Json::Value response;
    ClientUpdateType type;
    auto client_data = std::make_shared<ClientData>(); 
    client_data->login = data["login"].asString();
    if (data["TypeData"] == CHANGE_USER_EMAIL_SETTINGS) {
        type = UPDATE_EMAIL;
        client_data->email = data["email"].asString();
    }
    if (data["TypeData"] == CHANGE_USER_PASSWORD_SETTINGS) {
        type = UPDATE_PASSWORD;  
        client_data->hash = data["hash"].asString();
    }   
    // if (data["TypeData"] == CHANGE_USER_SESSION) {
    //     type = UPDATE_SESSION;  
    //     client_data->token = data["token"].asString();
    //     client_data->session_id = std::stoi(data["session_id"].asString());
    //     client_data->token_start_date = data["token_start_date"].asString();
    //     client_data->token_finish_date = data["token_finish_date"].asString();
    // }

    response["status"] = clien_rep_->Update(type, client_data->login, client_data);
    return response;
}

// Token

Json::Value DataBaseController::TokenRequestGet(const Json::Value& request) {
    Json::Value response;
    if (!redis_database_.get()) {
        response["DatabaseIsOpen"] = false;
        response["status"] = false;
        return response;
    }

    if (!redis_database_->IsOpen()) {  
        response["DatabaseIsOpen"] = false;
        response["status"] = false;
        return response;
    }

    std::string key = request["token"].asString();
    auto token_data = token_rep_->Get(key);

    if (token_data == nullptr) {
        response["status"] = false;
    }
    else {
        response["login"] = token_data->login;
        response["status"] = true;
    } 

    return response;    
}

Json::Value DataBaseController::TokenRequestPost(const Json::Value& request) {
    Json::Value response;
    if (!redis_database_.get()) {
        response["DatabaseIsOpen"] = false;
        response["status"] = false;
        return response;
    }

    if (!redis_database_->IsOpen()) {  
        response["DatabaseIsOpen"] = false;
        response["status"] = false;
        return response;
    }

    TokenData token_data;
    token_data.login = request["login"].asString();
    token_data.token = request["password"].asString();
    try {
        token_data.time_live = std::stoi(request["email"].asString());

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        response["status"] = false;
        return response;
    }

    response["status"] = token_rep_->Insert(token_data);
    return response;
}

Json::Value DataBaseController::TokenRequestDelete(const std::string& key) {
    Json::Value response;
    if (!redis_database_.get()) {
        response["DatabaseIsOpen"] = false;
        response["status"] = false;
        return response;
    }

    if (!redis_database_->IsOpen()) {  
        response["DatabaseIsOpen"] = false;
        response["status"] = false;
        return response;
    }

    response["status"] = token_rep_->Delete(key);
    return response;
}