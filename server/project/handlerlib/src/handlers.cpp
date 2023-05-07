#include "handlers.h"


namespace handlers {

PredictHandler::PredictHandler(ptrToPredictController controller)
    : controller_(controller) {};

void PredictHandler::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    // заглушка
}


ShowPlotHandler::ShowPlotHandler(ptrToShowPlotController controller)
    : controller_(controller) {}

void ShowPlotHandler::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    // заглушка
}


RegisterHandler::RegisterHandler(ptrToRegisterController controller)
    : controller_(controller) {}
void RegisterHandler::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    // заглушка
}


AuthorizeHandler::AuthorizeHandler(ptrToAuthorizeController controller)
    : controller_(controller) {} 
void AuthorizeHandler::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    // заглушка
}

Router::Router(const std::map<std::string, IHandler*>& handlers)
    : handlers_(handlers) {}
void Router::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    // заглушка  
}


} // namespace handlers 