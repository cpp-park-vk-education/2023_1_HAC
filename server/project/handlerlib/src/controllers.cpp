#include "controllers.h"
#include "handler_exception.h"
#include "logger.h"

#include <iostream>
#include <vector>

const size_t WINDOW_SIZE = 8;

const bool SERVER_ERROR = true;
const bool NOT_SERVER_ERROR = false;

const std::string TIME_SERIES = "timeseries";

const std::string HEADER_JSON_DB_STATUS_OPEN = "DatabaseIsOpen";
const std::string HEADER_JSON_ERROR = "error";
const std::string HEADER_JSON_TYPE = "Type";
const std::string HEADER_JSON_TYPEDATA = "TypeData";
const std::string HEADER_JSON_NAME_STOCK = "name_stock";
const std::string HEADER_JSON_LEN_LAGS = "len_lags";
const std::string HEADER_JSON_LENPREDICT = "lenpredict";
const std::string HEADER_JSON_DATA = "data";

const std::string HEADER_JSON_STATUS = "status";
const std::string HEADER_JSON_SERVER_ERROR = "server_error";

const std::string HEADER_JSON_PASSWORD = "password";
const std::string HEADER_JSON_EMAIL = "email";
const std::string HEADER_JSON_LOGIN = "login";
const std::string HEADER_JSON_DATE = "date";

FileLogger& logger = FileLogger::getInstance();

namespace controllers {

Json::Value makeJsonError(const std::string& error_mes, bool server_error) {
  Json::Value response;
  response[HEADER_JSON_STATUS] = false;
  response[HEADER_JSON_SERVER_ERROR] = server_error;
  response[HEADER_JSON_ERROR] = error_mes;
  logger.log("Json error completed successfully: " + error_mes);
  return response;
}


hash_ hashPassword(const std::string& password) {
    return password;
}

// class PredictController
PredictController::PredictController(const ptrToDBController db_controller, const ptrToModelController model_controller)
    : db_controller_(db_controller), model_controller_(model_controller) {}

Json::Value PredictController::makePredict(const Json::Value& request) {

    Json::Value request_to_db = makeDBProtocol(request);
    // отправляет название ДБ контроллеру
    logger.log("Request to DB... : predict controller");
    Json::Value response_db = db_controller_->DataRequest(request_to_db);
    try {
        std::vector<double> timeseries_vector = parseDBProtocol(response_db);        
        auto time_series = makeTimeSeries(timeseries_vector, std::stoi(request[HEADER_JSON_LENPREDICT].asString()));
        logger.log("Request to Model... : predict controller");
        return model_controller_->callModelApi(time_series);
    } catch (market_mentor::ErrorInGetDataFromDBInternal &e) {
        logger.log("Catched ErrorInGetDataFromDBInternal : predict controller");
        return makeJsonError(e.what(), SERVER_ERROR);
    } catch (market_mentor::ErrorInGetDataFromDBNotFound &e) {
        logger.log("Catched ErrorInGetDataFromDBNotFound : predict controller");
        return makeJsonError(e.what(), NOT_SERVER_ERROR);
    } catch (market_mentor::MarketMentorException &e) {
        logger.log("Catched MarketMentorException : predict controller");
        return makeJsonError(e.what(), SERVER_ERROR);
    } catch (std::exception &e) {
        logger.log("Catched std::exception : predict controller");
        return makeJsonError(e.what(), SERVER_ERROR);
    }
}

std::vector<double> PredictController::parseDBProtocol(const Json::Value& response) {
    if (!response[HEADER_JSON_STATUS].asBool() && !response[HEADER_JSON_DB_STATUS_OPEN].asBool()) {
        logger.log("Error. DB is closed: predict controller");
        throw market_mentor::ErrorInGetDataFromDBInternal(TIME_SERIES);
    }

    if (!response[HEADER_JSON_STATUS].asBool() && response[HEADER_JSON_DB_STATUS_OPEN].asBool()) {
        logger.log("Error. Not found pass request data: predict controller");
        throw market_mentor::ErrorInGetDataFromDBNotFound(TIME_SERIES);
    }

    if (response[HEADER_JSON_DATA].size() != WINDOW_SIZE) {
        logger.log("Error. Bad size received data from BD: predict controller");
        throw market_mentor::ErrorInGetDataFromDBInternal(TIME_SERIES);
    }
    std::vector<double> timeseries_vector;
    for (int i = 0; i < response[HEADER_JSON_DATA].size(); ++i) {
        timeseries_vector.push_back(std::stod(response[HEADER_JSON_DATA][i].asString()));
    }
    logger.log("Parse DB protocol completed successfully: predict controller");
    return timeseries_vector;
}

TimeSeriesPredicts PredictController::makeTimeSeries(const std::vector<double>& samples_data, size_t lenpredict) {
    TimeSeriesPredicts ts;
    ts.lenpredict = lenpredict;
    ts.matrix_samples = samples_data;
    logger.log("Timeseries completed successfully: predict controller");
    return ts;
}

Json::Value PredictController::makeDBProtocol(const Json::Value& request) {
    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::GET_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::TIMESERIES_REQUEST;
    db_protocol[HEADER_JSON_NAME_STOCK] = request[HEADER_JSON_NAME_STOCK].asString();
    db_protocol[HEADER_JSON_LEN_LAGS] = WINDOW_SIZE;
    logger.log("Json DP prtocol completed successfully: predict controller");
    return db_protocol;
}


// class ModelController
ModelController::ModelController(api::IAPIModelRequest* api_model) 
    : api_model_(api_model) {}

Json::Value ModelController::callModelApi(const TimeSeriesPredicts& samples_data) {
    logger.log("Enter in model controller: model controller");
    return api_model_->getData(samples_data);  
}

// class ShowPlotController
ShowPlotController::ShowPlotController(const ptrToDBController db_controller) 
    : db_controller_(db_controller) {}

Json::Value ShowPlotController::createPlotData(const Json::Value& request) {
    // получает из джейсона название акции
    Json::Value request_to_db = makeDBProtocol(request);
    logger.log("Request to DB... : showplot controller");
    return db_controller_->DataRequest(request_to_db);
}

Json::Value ShowPlotController::makeDBProtocol(const Json::Value& request) {
    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::GET_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::TIMESERIES_REQUEST;
    db_protocol[HEADER_JSON_NAME_STOCK] = request[HEADER_JSON_NAME_STOCK].asString();
    db_protocol[HEADER_JSON_LEN_LAGS] = request[HEADER_JSON_LEN_LAGS].asInt();
    logger.log("Json DP prtocol completed successfully: showplot controller");
    return db_protocol;
}


// class RegisterController
RegisterController::RegisterController(const ptrToDBController db_controller)
    : db_controller_(db_controller) {}

Json::Value RegisterController::registration(Json::Value& request) {
    request[HEADER_JSON_PASSWORD] = hashPassword(request[HEADER_JSON_PASSWORD].asString());
    Json::Value request_to_db = makeDBProtocol(request);
    logger.log("Request to DB... : registration controller");
    return db_controller_->DataRequest(request_to_db);
}

Json::Value RegisterController::makeDBProtocol(const Json::Value& request) {
    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::POST_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::REGISTRATION;
    db_protocol[HEADER_JSON_LOGIN] = request[HEADER_JSON_LOGIN].asString();
    db_protocol[HEADER_JSON_EMAIL] = request[HEADER_JSON_EMAIL].asString();
    db_protocol[HEADER_JSON_PASSWORD] = request[HEADER_JSON_PASSWORD].asString();
    logger.log("Json DP prtocol completed successfully: registration controller");
    return db_protocol;
}


// class AuthorizeController
AuthorizeController::AuthorizeController(const ptrToDBController db_controller)
    : db_controller_(db_controller) {}

Json::Value AuthorizeController::authorization(Json::Value& request) {
    Json::Value request_to_db = makeDBProtocol(request);
    logger.log("Request to DB... : authorization controller");
    Json::Value response_db = db_controller_->DataRequest(request_to_db);
    request[HEADER_JSON_PASSWORD] = hashPassword(request[HEADER_JSON_PASSWORD].asString());
    return checkPassword(response_db, request);
    
} 

Json::Value AuthorizeController::makeDBProtocol(const Json::Value& request) {
    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::POST_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::AUTHORIZATION;
    db_protocol[HEADER_JSON_LOGIN] = request[HEADER_JSON_LOGIN].asString();
    logger.log("Json DP prtocol completed successfully: authorization controller");
    return db_protocol;
}

Json::Value AuthorizeController::checkPassword(const Json::Value& db_response, const Json::Value& request) {
    Json::Value result_response;
    result_response[HEADER_JSON_STATUS] = (db_response[HEADER_JSON_PASSWORD].asString() == request[HEADER_JSON_PASSWORD].asString());
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
    return json_response_db[HEADER_JSON_STATUS].asBool();
}

Json::Value UpdateDataController::makeDBProtocol(const Json::Value& request) {
    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::POST_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::TIMESERIES_REQUEST;
    db_protocol[HEADER_JSON_NAME_STOCK] = request[HEADER_JSON_NAME_STOCK].asString();
    db_protocol[HEADER_JSON_DATE] = request[HEADER_JSON_DATE].asString();
    db_protocol[HEADER_JSON_DATA] = request[HEADER_JSON_DATA].asString();
    return db_protocol;
}


} // namespace controllers 
