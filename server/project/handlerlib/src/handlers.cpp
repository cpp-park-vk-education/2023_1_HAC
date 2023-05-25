#include "handlers.h"
#include "handler_exception.h"

#include "logger.h"
#include <vector>
#include <sstream>
#include <iostream>

const std::string HANDLERS_PREDICT = "predict";
const std::string HANDLERS_PLOT = "plot";
const std::string HANDLERS_REGISTRATION = "registration";
const std::string HANDLERS_AUTHORIZATION = "authorization";
const std::string HANDLERS_EXIT = "exit";
const std::string HANDLERS_CHECK_COOKIE = "check cookie";
const std::string HANDLERS_CHANGE_EMAIL = "change email";
const std::string HANDLERS_CHANGE_PASSWORD = "change password";

const std::string ERROR_MESSAGE = "Error-Message";
const std::string PREDICT_DATA = "Predict-Data";
const std::string PLOT_DATA = "Plot-Data";
const std::string REGISTRATION_STATUS = "Registration-Status";
const std::string AUTHORIZATION_STATUS = "Authorization-Status";

const std::string HTTP_URL_NAME = "name";
const std::string HTTP_URL_LENPREDICT = "lenpredict";
const std::string HTTP_URL_LAG = "lag";

const std::string METHOD = "method";
const std::string ACTIONS = "actions";
const std::string COOKIE = "cookie";

const std::string HEADER_JSON_DB_STATUS_OPEN = "DatabaseIsOpen";
const std::string HEADER_JSON_SERVER_ERROR = "server_error";
const std::string HEADER_JSON_ERROR = "error";
const std::string HEADER_JSON_NAME_STOCK = "name_stock";
const std::string HEADER_JSON_LEN_LAGS = "len_lags";
const std::string HEADER_JSON_LENPREDICT = "lenpredict";
const std::string HEADER_JSON_DATA = "data";
const std::string HEADER_JSON_DATE = "date";
const std::string HEADER_JSON_STATUS = "status";
const std::string HEADER_JSON_TOKEN = "token";

const std::string HEADER_JSON_PASSWORD = "password";
const std::string HEADER_JSON_OLD_PASSWORD = "old_password";
const std::string HEADER_JSON_EMAIL = "email";
const std::string HEADER_JSON_LOGIN = "login";
const std::string HEADER_JSON_PARAM = "param";

const std::string POST_AUTHORIZATION = "POST:AUTHORIZATION";
const std::string POST_REGISTRATION = "POST:REGISTRATION";
const std::string POST_CHECKCOOKIE = "POST:CHECKCOOKIE";

const int OK = 200;
const int BAD_REQUEST = 400;
const int NOT_FOUND = 404;
const int UNAUTHORIZED = 401;
const int FORBIDDEN = 403;
const int INTERNAL_SERVER_ERROR = 500;

const char SEPARATOR_URL = '&';
const char SEPARATOR_BODY = '/';
const std::string SEPARATOR_TOKEN_URL = "=";

const uint SIZE_BODY_CHECK_EXIT = 1;
const uint SIZE_BODY_EXIT = 1;
const uint SIZE_URL_GET_PREDICT = 2;
const uint SIZE_URL_GET_PLOT = 2;
const uint SIZE_BODY_POST_REGISTRATION = 3;
const uint SIZE_BODY_POST_AUTHORIZATION = 2;
const uint SIZE_BODY_POST_USER_SETTINGS = 4;
const uint SIZE_BODY_CHANGE_EMAIL = 3;
const uint SIZE_BODY_CHANGE_PASSWORD = 3;

const uint URL_SPECIAL_SYMBOL = 2;

const uint NAME_STOCK_ORDER = 0;
const uint LEN_LAGS_ORDER = 1;
const uint LENPREDICT_ORDER = 1;

const uint LOGIN_ORDER = 0;
const uint PASSWORD_ORDER = 1;
const uint EMAIL_ORDER = 2;

const uint CHANGE_ORDER_EMAIL = 1;
const uint CHANGE_ORDER_PASSWORD = 2;

const uint CHANGE_ORDER_OLD_PASSWORD = 1;
const uint CHANGE_ORDER_NEW_PASSWORD = 2;

namespace handlers {

FileLogger& logger = FileLogger::getInstance();

std::vector<std::string> splitMessage(const std::string& message, char separator) {
    std::vector<std::string> tokens;
    std::stringstream streamBuffer(message);
    std::string tmpBuffer;
    while (std::getline(streamBuffer, tmpBuffer, separator)) {
        tokens.push_back(tmpBuffer);
    }
    return tokens;
}

std::vector<std::string> cutUrlTokens(std::vector<std::string>& tokens, const std::string& error_mess) {
    std::vector<std::string> names_tokens;
    for (size_t i = 0; i < tokens.size(); ++i) {
        size_t equally_lit = tokens[i].find(SEPARATOR_TOKEN_URL);
        if (equally_lit == std::string::npos) {
            throw market_mentor::InvalidHttpRequestError(error_mess);
        }
        names_tokens.push_back(tokens[i].substr(0, equally_lit));
        tokens[i] = tokens[i].substr(equally_lit + 1);
    }
    names_tokens[NAME_STOCK_ORDER] = names_tokens[NAME_STOCK_ORDER].substr(URL_SPECIAL_SYMBOL);
    return names_tokens;
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
    // /?name=apple&lenpredict=8
    Json::Value result;
    auto tokens = splitMessage(message, SEPARATOR_URL);

    if (tokens.size() != SIZE_URL_GET_PREDICT) {
        throw market_mentor::InvalidHttpRequestError(HANDLERS_PREDICT);
    }
    auto names_tokens = cutUrlTokens(tokens, HANDLERS_PREDICT);

    if (names_tokens[NAME_STOCK_ORDER] != HTTP_URL_NAME || 
        names_tokens[LENPREDICT_ORDER] != HTTP_URL_LENPREDICT) {
        throw market_mentor::InvalidHttpRequestError(HANDLERS_PREDICT);
    }
    
    result[HEADER_JSON_NAME_STOCK] = tokens[NAME_STOCK_ORDER];
    result[HEADER_JSON_LENPREDICT] = std::stoi(tokens[LENPREDICT_ORDER]);
    
    return result;
}

void PredictHandler::makeResponse(IHTTPResponse_ response, const Json::Value& response_json) {
    if (!response_json[HEADER_JSON_STATUS].asBool()) {
        if (response_json[HEADER_JSON_SERVER_ERROR].asBool()) {
            response->setStatus(INTERNAL_SERVER_ERROR);
        } else {
            response->setStatus(NOT_FOUND);
        }
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
    // /?name=apple&lag=8
    Json::Value result;
    std::vector<std::string> tokens = splitMessage(message, SEPARATOR_URL);

    if (tokens.size() != SIZE_URL_GET_PLOT) {
        throw market_mentor::InvalidHttpRequestError(HANDLERS_PLOT);
    }
    auto names_tokens = cutUrlTokens(tokens, HANDLERS_PLOT);

    if (names_tokens[NAME_STOCK_ORDER] != HTTP_URL_NAME || 
        names_tokens[LEN_LAGS_ORDER] != HTTP_URL_LAG) {
        throw market_mentor::InvalidHttpRequestError(HANDLERS_PLOT);
    }

    result[HEADER_JSON_NAME_STOCK] = tokens[NAME_STOCK_ORDER];
    result[HEADER_JSON_LEN_LAGS] = std::stoi(tokens[LEN_LAGS_ORDER]);
    
    return result;
}

void ShowPlotHandler::makeResponse(IHTTPResponse_ response, const Json::Value& response_json) {
    if (!response_json[HEADER_JSON_STATUS].asBool()) {
        if (response_json[HEADER_JSON_DB_STATUS_OPEN].asBool()) {
            response->setStatus(NOT_FOUND);
        } else {
            response->setStatus(INTERNAL_SERVER_ERROR);
        }
        response->setHeader(ERROR_MESSAGE, ERROR_MESSAGE);
        response->setBody(response_json[HEADER_JSON_ERROR].asString());
        return;
    }
    std::string plot_data_ = response_json[HEADER_JSON_DATA].toStyledString();
    std::string date_ = response_json[HEADER_JSON_DATE].toStyledString();
    response->setStatus(OK);
    response->setHeader(PLOT_DATA, PLOT_DATA);
    response->setBody(plot_data_ + "\n" + date_);
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
    response->setHeader(COOKIE, response_json[HEADER_JSON_TOKEN].asString());
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
    response->setHeader(COOKIE, response_json[HEADER_JSON_TOKEN].asString());
}


// class Router
Router::Router(const std::map<std::string, IHandler*>& handlers, ptrToMiddleWare middleware)
    : handlers_(handlers), middleware_(middleware) {}
void Router::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    auto header = request->getHeaders();
    
    std::string key = header[METHOD] + ":" + header[ACTIONS];

    if (handlers_.find(key) == handlers_.end()) {
        logger.log("\nNot found key: \"" + key + "\"");
        response->setStatus(BAD_REQUEST);
        response->setBody("Not found key METHOD:ACTIONS");
        return;
    }
    cookie_map cookies;
    if (key != POST_AUTHORIZATION && key != POST_REGISTRATION) {
        try {
            cookies = middleware_->checkCookieFile(header[COOKIE]);
            for (auto it : cookies) {
                request->setHeader(it.first, it.second);
            }
        } catch(market_mentor::InvalidCookieError &e) {
            response->setStatus(FORBIDDEN);
            response->setBody(e.what());
            return;
        } catch(market_mentor::MarketMentorException &e) {
            response->setStatus(INTERNAL_SERVER_ERROR);
            response->setBody(e.what());
            return;
        } catch(std::exception &e) {
            response->setStatus(INTERNAL_SERVER_ERROR);
            response->setBody(e.what());
            return;
        }
    }
    if (key == POST_CHECKCOOKIE) {
        response->setStatus(OK);
        response->setBody(cookies.begin()->first);
        return;         
    }
    handlers_[key]->handle(request, response);
}


// class ExitHandler

ExitHandler::ExitHandler(ptrToExitController controller)
    : controller_(controller) {}

void ExitHandler::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    // post / body
    try {
        Json::Value request_json = parseInputHttpRequest(request->getBoby());
        if (request->getHeaders().find(request_json[HEADER_JSON_LOGIN].asString()) == request->getHeaders().end()) {
            throw market_mentor::InvalidCookieError();
        }
        request_json[HEADER_JSON_TOKEN] = request->getHeaders()[request_json[HEADER_JSON_LOGIN].asString()];
        Json::Value response_json = controller_->deleteCookie(request_json);
        makeResponse(response, response_json);

    } catch (market_mentor::InvalidCookieError &e) {
        response->setStatus(FORBIDDEN);
        response->setBody(e.what());
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

Json::Value ExitHandler::parseInputHttpRequest(const std::string& message) {
    //  login
    Json::Value result;
    std::vector<std::string> tokens = splitMessage(message, SEPARATOR_BODY);

    if (tokens.size() != SIZE_BODY_EXIT) {
        throw market_mentor::InvalidHttpRequestError(HANDLERS_EXIT);
    }
    result[HEADER_JSON_LOGIN] = tokens[LOGIN_ORDER];
    return result;
}

void ExitHandler::makeResponse(IHTTPResponse_ response, const Json::Value& response_json) {
    if (!response_json[HEADER_JSON_STATUS].asBool()) {
        response->setStatus(NOT_FOUND);
        return;
    }
    response->setStatus(OK);
}


// class ChangeEmailHandler
ChangeEmailHandler::ChangeEmailHandler(ptrToChangeEmailController controller)
    : controller_(controller) {}

void ChangeEmailHandler::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    // post / body
    try {
        Json::Value request_json = parseInputHttpRequest(request->getBoby());
        if (request->getHeaders().find(request_json[HEADER_JSON_LOGIN].asString()) == request->getHeaders().end()) {
            throw market_mentor::InvalidCookieError();
        }
        Json::Value response_json = controller_->changeEmail(request_json);
        makeResponse(response, response_json);

    } catch (market_mentor::InvalidCookieError &e) {
        response->setStatus(FORBIDDEN);
        response->setBody(e.what());
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

Json::Value ChangeEmailHandler::parseInputHttpRequest(const std::string& message) {
    // Username/newemail/password
    Json::Value result;
    std::vector<std::string> tokens = splitMessage(message, SEPARATOR_BODY);

    if (tokens.size() != SIZE_BODY_CHANGE_EMAIL) {
        throw market_mentor::InvalidHttpRequestError(HANDLERS_CHANGE_EMAIL);
    }
    result[HEADER_JSON_LOGIN] = tokens[LOGIN_ORDER];
    result[HEADER_JSON_EMAIL] = tokens[CHANGE_ORDER_EMAIL];
    result[HEADER_JSON_PASSWORD] = tokens[CHANGE_ORDER_PASSWORD];
    return result;
}

void ChangeEmailHandler::makeResponse(IHTTPResponse_ response, const Json::Value& response_json) {
    if (!response_json[HEADER_JSON_STATUS].asBool()) {
        response->setStatus(UNAUTHORIZED);
        return;
    }
    response->setStatus(OK);
}

// class ChangePasswordHandlers
ChangePasswordHandler::ChangePasswordHandler(ptrToChangePasswordController controller)
    : controller_(controller) {}

void ChangePasswordHandler::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    // post / body
    try {
        Json::Value request_json = parseInputHttpRequest(request->getBoby());
        if (request->getHeaders().find(request_json[HEADER_JSON_LOGIN].asString()) == request->getHeaders().end()) {
            throw market_mentor::InvalidCookieError();
        }
        Json::Value response_json = controller_->changePassword(request_json);
        makeResponse(response, response_json);

    } catch (market_mentor::InvalidCookieError &e) {
        response->setStatus(FORBIDDEN);
        response->setBody(e.what());
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

Json::Value ChangePasswordHandler::parseInputHttpRequest(const std::string& message) {
    // Username/oldpassword/newpassword
    Json::Value result;
    std::vector<std::string> tokens = splitMessage(message, SEPARATOR_BODY);

    if (tokens.size() != SIZE_BODY_CHANGE_PASSWORD) {
        throw market_mentor::InvalidHttpRequestError(HANDLERS_CHANGE_PASSWORD);
    }
    result[HEADER_JSON_LOGIN] = tokens[LOGIN_ORDER];
    result[HEADER_JSON_OLD_PASSWORD] = tokens[CHANGE_ORDER_OLD_PASSWORD];
    result[HEADER_JSON_PASSWORD] = tokens[CHANGE_ORDER_NEW_PASSWORD];
    return result;
}

void ChangePasswordHandler::makeResponse(IHTTPResponse_ response, const Json::Value& response_json) {
    if (!response_json[HEADER_JSON_STATUS].asBool()) {
        response->setStatus(UNAUTHORIZED);
        return;
    }
    response->setStatus(OK);
}

// class GetStocksHandler

GetStocksHandler::GetStocksHandler(ptrToGetStocksController controller)
    : controller_(controller) {}

void GetStocksHandler::handle(IHTTPRequest_ request, IHTTPResponse_ response) {
    // post / body
    try {
        Json::Value response_json = controller_->getNameStocks();
        makeResponse(response, response_json);

    } catch (market_mentor::MarketMentorException &e) {
        response->setStatus(INTERNAL_SERVER_ERROR);
        response->setBody(e.what());
    } catch (std::exception &e) {
        response->setStatus(INTERNAL_SERVER_ERROR);
        response->setBody(e.what());
    }
}

Json::Value GetStocksHandler::parseInputHttpRequest(const std::string& message) {}

void GetStocksHandler::makeResponse(IHTTPResponse_ response, const Json::Value& response_json) {
    if (!response_json[HEADER_JSON_STATUS].asBool()) {
        response->setStatus(NOT_FOUND);
        return;
    }
    response->setStatus(OK);
    std::string names_stock = response_json[HEADER_JSON_PARAM].toStyledString();
    response->setBody(names_stock);
}

} // namespace handlers 