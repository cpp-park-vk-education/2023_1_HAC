#include "controllers.h"

namespace controllers {

PredictController::PredictController
    (const ptrToDBController db_controller, const ptrToModelController model_controller) :
            db_controller_(db_controller), model_controller_(model_controller) {}

void PredictController::makePredict(IHTTPRequest_ request, IHTTPResponse_ response) {
    // заглушка
}

Json::Value PredictController::parseInputHttpRequest(const IHTTPRequest_ request) {
    // заглушка
}
Json::Value PredictController::parsePlotDataFromDB(const Json::Value& data_protocol) {
    // заглушка
}

TimeSeriesPredicts& PredictController::makeTimeSeries(const Json::Value& samples_data, size_t window_size) {
    // заглушка
}

IHTTPResponse_ PredictController::parseModelResponse(const IHTTPResponse_ request) {
    // заглушка
}  

ModelController::ModelController(const ptrToAPIModel api_model) 
    : api_model_(api_model) {}

IHTTPResponse_ ModelController::callModelApi(const TimeSeriesPredicts& samples_data) {
    // заглушка
}

IHTTPRequest_ ModelController::makeHttpForModel(const TimeSeriesPredicts& samples_data) {
    // заглушка
}


ShowPlotController::ShowPlotController(const ptrToDBController db_controller) 
    : db_controller_(db_controller) {}

IHTTPResponse_ ShowPlotController::createPlotData(IHTTPRequest_ request) {
    // заглушка
}

Json::Value ShowPlotController::parseInputHttpRequest(const IHTTPRequest_ request) {
    // заглушка
}


RegisterController::RegisterController(const ptrToDBController db_controller)
    : db_controller_(db_controller) {}

IHTTPResponse_ RegisterController::registration(const IHTTPRequest_ request) {
    // заглушка
}
 
Json::Value RegisterController::parseInputHttpRequest(const IHTTPRequest_ request) {
    // заглушка
}

hash_ RegisterController::hashPassword(const std::string& password) {
    // заглушка
}
   
bool RegisterController::postDataRegistrDB(const Json::Value& data_protocol) {
    // заглушка
}


AuthorizeController::AuthorizeController(const ptrToDBController db_controller)
    : db_controller_(db_controller) {}

IHTTPResponse_ AuthorizeController::authorization(const IHTTPRequest_ request) {
    // заглушка
} 

Json::Value AuthorizeController::parseInputHttpRequest(const IHTTPRequest_ request) {
    // заглушка
}
hash_ AuthorizeController::hashPassword(const std::string& password) {
    // заглушка
}
bool AuthorizeController::getCheckAuthorData(const Json::Value& data_protocol) {
    // заглушка
} 

UpdateDataController::UpdateDataController(const ptrToDBController db_controller, const ptrToAPIStock api_stock)
    : db_controller_(db_controller), api_stock_(api_stock) {}

bool UpdateDataController::udateData(const handlers::ProtocolAPI& protocol) {
    // заглушка
}

std::string UpdateDataController::parseToApiRequest(const handlers::ProtocolAPI& protocol) {
    // заглушка
}


} // namespace controllers 
