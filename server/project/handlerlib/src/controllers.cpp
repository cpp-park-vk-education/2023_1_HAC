#include "controllers.h"
#include "handler_exception.h"


#include <iostream>
#include <vector>


namespace controllers {


Json::Value makeJsonError(const std::string& error_mes) {
  Json::Value response;
  response["status"] = false;
  response["error"] = error_mes;
  return response;
}

Json::Value makeDBProtocolGetTS(const Json::Value& request) {
    Json::Value db_protocol;
    db_protocol["Type"] = TypeRequest::GET_REQUEST;
    db_protocol["TypeData"] = TypeData::TIMESERIES_REQUEST;
    db_protocol["name_stock"] = request["name_stock"];
    db_protocol["len_lags"] = request["lags"];
    return db_protocol;
}

hash_ hashPassword(const std::string& password) {
    return password;
}


// class PredictController
PredictController::PredictController
    (const ptrToDBController db_controller, const ptrToModelController model_controller) :
            db_controller_(db_controller), model_controller_(model_controller) {}

Json::Value PredictController::makePredict(const Json::Value& request) {
    // получает джейсон
    // получает из него название акции 
    Json::Value request_to_db = makeDBProtocolGetTS(request);
    // отправляет название ДБ контроллеру
    Json::Value response_db = db_controller_->DataRequest(request_to_db);
    try {
        std::vector<double> timeseries_vector = parseDBProtocol(response_db);        
        auto time_series = makeTimeSeries(timeseries_vector, std::stol(request["window_size"].asString()));

        return model_controller_->callModelApi(time_series);

    } catch (market_mentor::MarketMentorException &e) {
        return makeJsonError(e.what());
    } catch (std::exception &e) {
        return makeJsonError(e.what());
    }
}

std::vector<double> PredictController::parseDBProtocol(const Json::Value& response) {
    if (!response["status"]) {
        throw market_mentor::ErrorInGetDataFromDB("timeseries");
    }
    std::vector<double> timeseries_vector;
    for (int i = 0; i < response["data"].size(); ++i) {
        timeseries_vector.push_back(std::stod(response["data"][i].asString()));
    }
    return timeseries_vector;
}


TimeSeriesPredicts PredictController::makeTimeSeries(const std::vector<double>& samples_data, size_t window_size) {
    TimeSeriesPredicts ts;
    ts.window_size = window_size;
    if (samples_data.size() % ts.window_size != 0) {
        throw market_mentor::ErrorInGetDataFromDB("timeseries (size)");
    }
    ts.matrix_samples = samples_data;
    return ts;
}

// class ModelController
ModelController::ModelController(api::IAPIModelRequest* api_model) 
    : api_model_(api_model) {}

Json::Value ModelController::callModelApi(const TimeSeriesPredicts& samples_data) {
    return api_model_->getData(samples_data);  
}



// class ShowPlotController
ShowPlotController::ShowPlotController(const ptrToDBController db_controller) 
    : db_controller_(db_controller) {}

Json::Value ShowPlotController::createPlotData(const Json::Value& request) {
    // получает из джейсона название акции
    Json::Value request_to_db = makeDBProtocolGetTS(request);
    return db_controller_->DataRequest(request_to_db);
}

// class RegisterController
RegisterController::RegisterController(const ptrToDBController db_controller)
    : db_controller_(db_controller) {}

Json::Value RegisterController::registration(Json::Value& request) {
    request["password"] = hashPassword(request["password"].asString());
    Json::Value request_to_db = makeDBProtocol(request);
    return db_controller_->DataRequest(request_to_db);
}


Json::Value RegisterController::makeDBProtocol(const Json::Value& request) {
    Json::Value db_protocol;
    db_protocol["Type"] = TypeRequest::POST_REQUEST;
    db_protocol["TypeData"] = TypeData::REGISTRATION;
    db_protocol["login"] = request["login"];
    db_protocol["email"] = request["email"];
    db_protocol["password"] = request["password"];
    return db_protocol;
}


// class AuthorizeController
AuthorizeController::AuthorizeController(const ptrToDBController db_controller)
    : db_controller_(db_controller) {}

Json::Value AuthorizeController::authorization(Json::Value& request) {
    Json::Value request_to_db = makeDBProtocol(request);
    Json::Value response_db = db_controller_->DataRequest(request_to_db);
    request["password"] = hashPassword(request["password"].asString());
    return checkPassword(response_db, request);
    
} 

Json::Value AuthorizeController::makeDBProtocol(const Json::Value& request) {
    Json::Value db_protocol;
    db_protocol["Type"] = TypeRequest::POST_REQUEST;
    db_protocol["TypeData"] = TypeData::AUTHORIZATION;
    db_protocol["login"] = request["login"];
    return db_protocol;
}

Json::Value AuthorizeController::checkPassword(const Json::Value& db_response, const Json::Value& request) {
    Json::Value result_response;
    result_response["status"] = (db_response["password"] == request["password"]);
    return result_response;
}   

// class UpdateDataController
UpdateDataController::UpdateDataController(const ptrToDBController db_controller, const ptrToAPIStock api_stock)
    : db_controller_(db_controller), api_stock_(api_stock) {}

bool UpdateDataController::updateData(const handlers::ProtocolAPI& protocol) {
    // создаём из структуры стрингу
    auto response_model = api_stock_->getData(protocol); // json
    Json::Value db_protocol = makeDBProtocol(response_model);
    Json::Value json_response_db = db_controller_->DataRequest(db_protocol);
    return json_response_db["status"].asBool();
}

Json::Value UpdateDataController::makeDBProtocol(const Json::Value& request) {
    Json::Value db_protocol;
    db_protocol["Type"] = TypeRequest::POST_REQUEST;
    db_protocol["TypeData"] = TypeData::TIMESERIES_REQUEST;
    db_protocol["name_stock"] = request["name_stock"];
    db_protocol["date"] = request["date"];
    db_protocol["data"] = request["data"];
    return db_protocol;
}


} // namespace controllers 
