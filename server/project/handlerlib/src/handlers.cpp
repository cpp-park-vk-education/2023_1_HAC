#include "handlers.h"


namespace handlers {

PredictHandler::PredictHandler(ptrToPredictController controller)
    : controller_(controller) {};

void PredictHandler::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    // парс реквеста
    // вызов конроллера
    // парс ответа от контроллера
    // отдача в респонс
}

Json::Value PredictHandler::parseInputHttpRequest(const IHTTPRequest_ request) {
    // парс реквеста
}

ShowPlotHandler::ShowPlotHandler(ptrToShowPlotController controller)
    : controller_(controller) {}

void ShowPlotHandler::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    // парс реквеста
    // вызов конроллера
    // парс ответа от контроллера
    // отдача в респонс
}

Json::Value ShowPlotHandler::parseInputHttpRequest(const IHTTPRequest_ request) {
    // парс реквеста
}


RegisterHandler::RegisterHandler(ptrToRegisterController controller)
    : controller_(controller) {}
void RegisterHandler::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    // заглушка
}

Json::Value RegisterHandler::parseInputHttpRequest(const IHTTPRequest_ request) {
    // парс реквеста
}

AuthorizeHandler::AuthorizeHandler(ptrToAuthorizeController controller)
    : controller_(controller) {} 
void AuthorizeHandler::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
     // парс реквеста
    // вызов конроллера
    // парс ответа от контроллера
    // отдача в респонс
}

Json::Value AuthorizeHandler::parseInputHttpRequest(const IHTTPRequest_ request) {
    // парос реквеста
}

Router::Router(const std::map<std::string, IHandler*>& handlers)
    : handlers_(handlers) {}
void Router::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    // заглушка  
}


} // namespace handlers 