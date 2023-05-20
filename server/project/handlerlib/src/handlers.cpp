#include "handlers.h"
#include "handler_exception.h"

#include <vector>
#include <sstream>
#include <iostream>

const std::string HANDLERS_PREDICT = "predict";
const std::string HANDLERS_PLOT = "plot";
const std::string HANDLERS_REGISTRATION = "registration";
const std::string HANDLERS_AUTHORIZATION = "authorization";

const std::string ERROR_MESSAGE = "Error-Message";
const std::string PREDICT_DATA = "Predict-Data";
const std::string PLOT_DATA = "Plot-Data";
const std::string REGISTRATION_STATUS = "Registration-Status";
const std::string AUTHORIZATION_STATUS = "Authorization-Status";


const std::string METHOD = "method";
const std::string ACTIONS = "actions";


const std::string HEADER_JSON_ERROR = "error";
const std::string HEADER_JSON_NAME_STOCK = "name_stock";
const std::string HEADER_JSON_LEN_LAGS = "len_lags";
const std::string HEADER_JSON_WINDOW_SIZE = "window_size";
const std::string HEADER_JSON_DATA = "data";
const std::string HEADER_JSON_STATUS = "status";

const std::string HEADER_JSON_PASSWORD = "password";
const std::string HEADER_JSON_EMAIL = "email";
const std::string HEADER_JSON_LOGIN = "login";



const int OK = 200;
const int BAD_REQUEST = 400;
const int NOT_FOUND = 404;
const int UNAUTHORIZED = 401;
const int INTERNAL_SERVER_ERROR = 500;

const char SEPARATOR_URL = '&';
const char SEPARATOR_BODY = '/';
const std::string SEPARATOR_TOKEN_URL = "=";

const uint SIZE_URL_GET_PREDICT_PLOT = 4;
const uint SIZE_BODY_POST_REGISTRATION = 3;
const uint SIZE_BODY_POST_AUTHORIZATION = 2;
const uint SIZE_BODY_POST_USER_SETTINGS = 4;

const uint NAME_STOCK_ORDER = 0;
const uint LEN_LAGS_ORDER = 2;
const uint WINDOW_SIZE_ORDER = 3;

const uint LOGIN_ORDER = 0;
const uint PASSWORD_ORDER = 1;
const uint EMAIL_ORDER = 2;


namespace handlers {

std::vector<std::string> splitMessage(const std::string& message, char separator) {
    std::vector<std::string> tokens;
    std::stringstream streamBuffer(message);
    std::string tmpBuffer;
    while (std::getline(streamBuffer, tmpBuffer, separator)) {
        tokens.push_back(tmpBuffer);
    }
    return tokens;
}

void cutUrlTokens(std::vector<std::string>& tokens, const std::string& error_mess) {
    for (size_t i = 0; i < tokens.size(); ++i) {
        size_t equally_lit = tokens[i].find(SEPARATOR_TOKEN_URL);
        if (equally_lit == std::string::npos) {
            throw market_mentor::InvalidHttpRequestError(error_mess);
        }
        tokens[i] = tokens[i].substr(equally_lit + 1);
    }
}

// class PredictHandler
PredictHandler::PredictHandler(ptrToPredictController controller)
    : controller_(controller) {};

void PredictHandler::handle(IHTTPRequest_ request, IHTTPResponse_ response) {   
    // url / getUrl
    try {
        Json::Value request_json = parseInputHttpRequest(request->getURL());
        Json::Value response_json = controller_->makePredict(request_json);
        makeResponse(response, response_json);

    } catch (market_mentor::InvalidHttpRequestError &e) {
        response->setStatus(BAD_REQUEST);
        response->setBody(e.what());
    } catch (market_mentor::MarketMentorException &e) {
        response->setStatus(INTERNAL_SERVER_ERROR);
        response->setBody(e.what());
    } catch (std::exception &e) {
        response->setStatus(INTERNAL_SERVER_ERROR);
        response->setBody(e.what());
    }
}

Json::Value PredictHandler::parseInputHttpRequest(const std::string& message) {
    // /?name=apple&graph=predict&lag=8&window_size=8
    Json::Value result;
    auto tokens = splitMessage(message, SEPARATOR_URL);

    if (tokens.size() != SIZE_URL_GET_PREDICT_PLOT) {
        throw market_mentor::InvalidHttpRequestError(HANDLERS_PREDICT);
    }
    cutUrlTokens(tokens, HANDLERS_PREDICT);
    
    result[HEADER_JSON_NAME_STOCK] = tokens[NAME_STOCK_ORDER];
    result[HEADER_JSON_LEN_LAGS] = std::stoi(tokens[LEN_LAGS_ORDER]);
    result[HEADER_JSON_WINDOW_SIZE] = std::stoi(tokens[WINDOW_SIZE_ORDER]);
    
    return result;
}

void PredictHandler::makeResponse(IHTTPResponse_ response, const Json::Value& response_json) {
    if (!response_json[HEADER_JSON_STATUS].asBool()) {
        response->setStatus(INTERNAL_SERVER_ERROR);
        response->setHeader(ERROR_MESSAGE, ERROR_MESSAGE);
        response->setBody(response_json[HEADER_JSON_ERROR].asString());
        return;
    }
    std::string plot_data = response_json[HEADER_JSON_DATA].toStyledString();
    response->setStatus(OK);
    response->setHeader(PREDICT_DATA, PREDICT_DATA);
    response->setBody(plot_data);

}


// class ShowPlotHandler
ShowPlotHandler::ShowPlotHandler(ptrToShowPlotController controller)
    : controller_(controller) {}

void ShowPlotHandler::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    // get / url
    try {
        Json::Value request_json = parseInputHttpRequest(request->getURL());
        Json::Value response_json = controller_->createPlotData(request_json);
        
        makeResponse(response, response_json);

    } catch (market_mentor::InvalidHttpRequestError &e) {
        response->setStatus(BAD_REQUEST);
        response->setBody(e.what());
    } catch (market_mentor::MarketMentorException &e) {
        response->setStatus(INTERNAL_SERVER_ERROR);
        response->setBody(e.what());
    } catch (std::exception &e) {
        response->setStatus(INTERNAL_SERVER_ERROR);
        response->setBody(e.what());
    }
}

Json::Value ShowPlotHandler::parseInputHttpRequest(const std::string& message) {
    // /?name=apple&graph=predict&lag=8&window_size=8
    Json::Value result;
    std::vector<std::string> tokens = splitMessage(message, SEPARATOR_URL);

    if (tokens.size() != SIZE_URL_GET_PREDICT_PLOT) {
        throw market_mentor::InvalidHttpRequestError(HANDLERS_PLOT);
    }
    cutUrlTokens(tokens, HANDLERS_PLOT);
    
    result[HEADER_JSON_NAME_STOCK] = tokens[NAME_STOCK_ORDER];
    result[HEADER_JSON_LEN_LAGS] = std::stoi(tokens[LEN_LAGS_ORDER]);
    
    return result;
}

void ShowPlotHandler::makeResponse(IHTTPResponse_ response, const Json::Value& response_json) {
    if (!response_json[HEADER_JSON_STATUS].asBool()) {
        response->setStatus(INTERNAL_SERVER_ERROR);
        response->setHeader(ERROR_MESSAGE, ERROR_MESSAGE);
        response->setBody(response_json[HEADER_JSON_ERROR].asString());
        return;
    }
    std::string plot_data_ = response_json[HEADER_JSON_DATA].toStyledString();

    response->setStatus(OK);
    response->setHeader(PLOT_DATA, PLOT_DATA);
    response->setBody(plot_data_);
}

// class RegisterHandler
RegisterHandler::RegisterHandler(ptrToRegisterController controller)
    : controller_(controller) {}
void RegisterHandler::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    // post / body
    try {
        Json::Value request_json = parseInputHttpRequest(request->getBoby());
        Json::Value response_json = controller_->registration(request_json);
        makeResponse(response, response_json);

    } catch (market_mentor::InvalidHttpRequestError &e) {
        response->setStatus(BAD_REQUEST);
        response->setBody(e.what());
    } catch (market_mentor::MarketMentorException &e) {
        response->setStatus(INTERNAL_SERVER_ERROR);
        response->setBody(e.what());
    } catch (std::exception &e) {
        response->setStatus(INTERNAL_SERVER_ERROR);
        response->setBody(e.what());
    }
}

Json::Value RegisterHandler::parseInputHttpRequest(const std::string& message) {
    // real_login/real_password/real_email/
    Json::Value result;
    std::vector<std::string> tokens = splitMessage(message, SEPARATOR_BODY);

    if (tokens.size() != SIZE_BODY_POST_REGISTRATION) {
        throw market_mentor::InvalidHttpRequestError(HANDLERS_REGISTRATION);
    }

    result[HEADER_JSON_LOGIN] = tokens[LOGIN_ORDER];
    result[HEADER_JSON_EMAIL] = tokens[EMAIL_ORDER];
    result[HEADER_JSON_PASSWORD] = tokens[PASSWORD_ORDER];
 
    return result;
}

void RegisterHandler::makeResponse(IHTTPResponse_ response, const Json::Value& response_json) {
    if (!response_json[HEADER_JSON_STATUS].asBool()) {
        response->setStatus(UNAUTHORIZED);
        return;
    }
    response->setStatus(OK);
}


// class AuthorizeHandler
AuthorizeHandler::AuthorizeHandler(ptrToAuthorizeController controller)
    : controller_(controller) {} 
void AuthorizeHandler::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    // post / body
    try {
        Json::Value request_json = parseInputHttpRequest(request->getBoby());
        Json::Value response_json = controller_->authorization(request_json);
        makeResponse(response, response_json);

    } catch (market_mentor::InvalidHttpRequestError &e) {
        response->setStatus(BAD_REQUEST);
        response->setBody(e.what());
    } catch (market_mentor::MarketMentorException &e) {
        response->setStatus(INTERNAL_SERVER_ERROR);
        response->setBody(e.what());
    } catch (std::exception &e) {
        response->setStatus(INTERNAL_SERVER_ERROR);
        response->setBody(e.what());
    }
}

Json::Value AuthorizeHandler::parseInputHttpRequest(const std::string& message) {
    //  real_login/real_password/
    Json::Value result;
    std::vector<std::string> tokens = splitMessage(message, SEPARATOR_BODY);

    if (tokens.size() != SIZE_BODY_POST_AUTHORIZATION) {
        throw market_mentor::InvalidHttpRequestError(HANDLERS_AUTHORIZATION);
    }

    result[HEADER_JSON_LOGIN] = tokens[LOGIN_ORDER];
    result[HEADER_JSON_PASSWORD] = tokens[PASSWORD_ORDER];
 
    return result;
}

void AuthorizeHandler::makeResponse(IHTTPResponse_ response, const Json::Value& response_json) {
    if (!response_json[HEADER_JSON_STATUS].asBool()) {
        response->setStatus(UNAUTHORIZED);
        return;
    }
    response->setStatus(OK);
}


// class Router
Router::Router(const std::map<std::string, IHandler*>& handlers)
    : handlers_(handlers) {}
void Router::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    auto header = request->getHeaders();
    std::string key = header[METHOD] + ":" + header[ACTIONS];
    if (handlers_.find(key) == handlers_.end()) {
        std::cerr << "\nNot found key: \"" << key << "\"" << std::endl;
        response->setStatus(BAD_REQUEST);
        response->setHeader(ERROR_MESSAGE, ERROR_MESSAGE);
        response->setBody(ERROR_MESSAGE);
        return;
    }
    handlers_[key]->handle(request, response);
}


Json::Value Router::parseInputHttpRequest(const std::string& message) {
}

void Router::makeResponse(IHTTPResponse_ response, const Json::Value& response_json) {

}

} // namespace handlers 