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


// class PredictController
PredictController::PredictController
    (const ptrToDBController db_controller, const ptrToModelController model_controller) :
            db_controller_(db_controller), model_controller_(model_controller) {}

Json::Value PredictController::makePredict(const Json::Value& request) {
    // получает джейсон
    // получает из него название акции 
    Json::Value request_to_db = makeDBProtocol(request, std::stol(request["lags"].asString()));
    // отправляет название ДБ контроллеру
    Json::Value response_db = db_controller_->DataRequest(request_to_db);
    try {
        std::vector<double> timeseries_vector = parseDBProtocol(response_db);        
        auto time_series = makeTimeSeries(timeseries_vector, std::stol(request["window_size"].asString()));

        Json::Value model_response = model_controller_->callModelApi(time_series);
        return model_response;

    } catch (market_mentor::MarketMentorException &e) {
        return makeJsonError(e.what());
    } catch (std::exception &e) {
        return makeJsonError(e.what());
    }

}

Json::Value PredictController::makeDBProtocol(const Json::Value& request, size_t lags) {
    Json::Value db_protocol;
    db_protocol["Type"] = TypeRequest::GET_REQUEST;
    db_protocol["TypeData"] = TypeData::TIMESERIES_REQUEST;
    db_protocol["name_stock"] = request["name_stock"];
    db_protocol["len_lags"] = lags;
    return db_protocol;
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
ModelController::ModelController(const ptrToAPIModel api_model) 
    : api_model_(api_model) {}

Json::Value ModelController::callModelApi(const TimeSeriesPredicts& samples_data) {
    // создаёт реквест для апи модели
    //auto request_to_model = makeHttpForModel(sample_data);
    // // отправляет реквест апи
    // // принимает респонс
    // auto response = api_model_->getData(request_to_model);
    // // парсит респонс
    // Json::Value response_json = parseModelResponse(response);
    // // формирует хороший или плохой джейсон
    // return response_json;
}

IHTTPRequest_ ModelController::makeHttpForModel(const TimeSeriesPredicts& samples_data) {
    // создание реквеста по тайм серии
}

Json::Value ModelController::parseModelResponse(const IHTTPResponse_ request) {
    // // парсинг респоса от модели
    // Json::Value result;
    // result["data"] = "распарсил";
    // return result;
}

// class ShowPlotController
ShowPlotController::ShowPlotController(const ptrToDBController db_controller) 
    : db_controller_(db_controller) {}

Json::Value ShowPlotController::createPlotData(const Json::Value& request) {
    // // получает из джейсона название акции
    // try {
    //     std::string name_stok = request["name_stock"];
    // } catch (...) {
    //     std::cerr << "Invalid name stock" << std::endl;
    //     Json::Value response;
    //      // отдаёт плохой джейсон
    //     response["error"] = "Error";
    //     return response;
    // }
    // // запрашивает у ДБ контроллера 
    // Json::Value response_from_db = db_controller_->DataRequest(request);
    // // проверяет ответ
    // if (name_stock != response_from_db["name_stock"] || !response_from_db["data_stock"]) {
    //     Json::Value response;
    //      // отдаёт плохой джейсон
    //     response["error"] = "Error";
    //     return response;
    // }
    // // отдаёт хороший джейсон
    // return response_from_db;
}

// class RegisterController
RegisterController::RegisterController(const ptrToDBController db_controller)
    : db_controller_(db_controller) {}

Json::Value RegisterController::registration(const Json::Value& request) {
    //  // получаем джейсон
    // Json::Value response;
    // // хешируем пароль
    // try {
    //     request["password"] = hashPassword(request["password"]);
    // } catch {
    //     std::cerr << "Invalid password" << std::endl;
    //      // отдаёт плохой джейсон
    //     response["error"] = "Error";
    //     return response;
    // }
    // // отправляем бд

    // Json::Value response_from_db = db_controller_->DataRequest(request);
    // if (request["status"] != "OK") {
    //      // отдаёт плохой джейсон
    //     response["error"] = "Error";
    //     return response;
    // }
    // response["status"] = "OK";
    // return response;
}

hash_ RegisterController::hashPassword(const std::string& password) {
  //  return password;
}

// class AuthorizeController
AuthorizeController::AuthorizeController(const ptrToDBController db_controller)
    : db_controller_(db_controller) {}

Json::Value AuthorizeController::authorization(const Json::Value& request) {
    // // получаем джейсон
    // Json::Value response;
    // // хешируем пароль
    // try {
    //     request["password"] = hashPassword(request["password"]);
    // } catch {
    //     std::cerr << "Invalid password" << std::endl;
    //      // отдаёт плохой джейсон
    //     response["error"] = "Error";
    //     return response;
    // }
    // // отправляем бд

    // Json::Value response_from_db = db_controller_->DataRequest(request);
    // if (request["status"] != "OK") {
    //      // отдаёт плохой джейсон
    //     response["error"] = "Error";
    //     return response;
    // }
    // response["status"] = "OK";
   // return response;
    
} 

hash_ AuthorizeController::hashPassword(const std::string& password) {
    return password;
}

// class UpdateDataController
UpdateDataController::UpdateDataController(const ptrToDBController db_controller, const ptrToAPIStock api_stock)
    : db_controller_(db_controller), api_stock_(api_stock) {}

bool UpdateDataController::updateData(const handlers::ProtocolAPI& protocol) {
    // создаём из структуры стрингу
    // std::string protocol_string = parseToApiRequest(protocol);
    
    // // отдаём на вход апи стрингу
    // auto response_model = api_stock_->getData(protocol_string);
    // // парсим хттп в json
    // Json::Value json_response_model = parseHTTPToJson(&response);
    // // отдаём json ДБ контроллеру на запись
    // Json::Value json_response_db = db_controller_->DataRequest(json_response_model);
    // // возращаем статус состояния
    // return (json_response_db["status"] == "OK");

}


Json::Value UpdateDataController::parseHTTPToJson(IHTTPResponse_ response) {
    // из поданнной на вход структуры делаем стрингу по протоколу 
    // Json::Value result;
    // result["data"] = "распарсил";
    // return result;
}

std::string UpdateDataController::parseToApiRequest(const handlers::ProtocolAPI& protocol) {
    // из поданнной на вход структуры делаем стрингу по протоколу
    return "распаршено";

}

} // namespace controllers 
