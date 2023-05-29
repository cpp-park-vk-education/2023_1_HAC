#include "controllers.h"
#include "handler_exception.h"
#include "logger.h"


#include <openssl/sha.h>
#include <openssl/rand.h>

#include <iomanip>
#include <sstream>
#include <iostream>
#include <vector>

const size_t WINDOW_SIZE = 300;
const size_t TIME_LIFE = 86400;

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
const std::string HEADER_JSON_DATA = "param";
const std::string HEADER_JSON_TOKEN = "token";
const std::string HEADER_JSON_TIME_LIFE = "time_live";

const std::string HEADER_JSON_STATUS = "status";
const std::string HEADER_JSON_SERVER_ERROR = "server_error";

const std::string HEADER_JSON_PASSWORD = "password";
const std::string HEADER_JSON_OLD_PASSWORD = "old_password";
const std::string HEADER_JSON_EMAIL = "email";
const std::string HEADER_JSON_LOGIN = "login";
const std::string HEADER_JSON_DATE = "date";



namespace controllers {

FileLogger& logger = FileLogger::getInstance();

Json::Value makeJsonError(const std::string& error_mes, bool server_error) {
  Json::Value response;
  response[HEADER_JSON_STATUS] = false;
  response[HEADER_JSON_SERVER_ERROR] = server_error;
  response[HEADER_JSON_ERROR] = error_mes;
  logger.log("Json error completed successfully: " + error_mes);
  return response;
}

std::string makeCookie() {
  
    unsigned char buffer[2048];

    if (RAND_bytes(buffer, sizeof(buffer)) != 1) {
        throw market_mentor::CreateCookieError();
    }

    unsigned int randomNum = 0;
    for (int i = 0; i < sizeof(buffer); ++i) {
        randomNum <<= 8;
        randomNum |= buffer[i];
    }

    return hashing(std::to_string(randomNum)).substr(0, 100);

}

Json::Value makeProtocolSendCookie(const std::string& cookie, const std::string& login) {
    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::POST_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::SESSION_REQUEST;
    db_protocol[HEADER_JSON_LOGIN] = login;
    db_protocol[HEADER_JSON_TOKEN] = cookie;
    db_protocol[HEADER_JSON_TIME_LIFE] = TIME_LIFE;
    logger.log("Json DP prtocol cookie completed successfully");
    return db_protocol;
}

hash_ hashing(const std::string& buffer) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, buffer.c_str(), buffer.size());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;

    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
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
    request[HEADER_JSON_PASSWORD] = hashing(request[HEADER_JSON_PASSWORD].asString());
    Json::Value request_to_db = makeDBProtocol(request);
    logger.log("Request to DB... : registration controller");
    Json::Value response_db = db_controller_->DataRequest(request_to_db);

    if (response_db[HEADER_JSON_STATUS].asBool()) {
        std::string cookie = makeCookie();
        logger.log("Make protocol for cookie... : registration controller");
        Json::Value coockei_to_db = makeProtocolSendCookie(cookie, request[HEADER_JSON_LOGIN].asString());
        logger.log("DB request for cookie... : registration controller");
        Json::Value result_send_coockie = db_controller_->DataRequest(coockei_to_db);
        if (!result_send_coockie[HEADER_JSON_STATUS].asBool()) {
            logger.log("Bad response from DB request for cookie... : registration controller");
            throw market_mentor::CreateCookieError();
        }
        logger.log("Add cookie to response... : registration controller");
        result_send_coockie[HEADER_JSON_TOKEN] = cookie;
        return result_send_coockie;
    }

    logger.log("There is already such a user : registration controller");
    return response_db;
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
    logger.log("Request to DB for auth data... : authorization controller");
    Json::Value response_db = db_controller_->DataRequest(request_to_db);
    if (!response_db[HEADER_JSON_STATUS].asBool()) {
        return response_db;
    }
    logger.log("Hashing... : authorization controller");
    request[HEADER_JSON_PASSWORD] = hashing(request[HEADER_JSON_PASSWORD].asString());
    
    Json::Value result_password_check = checkPassword(response_db, request);
    if (result_password_check[HEADER_JSON_STATUS].asBool()) {
        logger.log("Make cookie... : authorization controller");
        std::string cookie = makeCookie();
        logger.log("Make protocol for cookie... : authorization controller");
        Json::Value coockei_to_db = makeProtocolSendCookie(cookie, request[HEADER_JSON_LOGIN].asString());
        logger.log("DB request for cookie... : authorization controller");
        Json::Value result_send_coockie = db_controller_->DataRequest(coockei_to_db);
        if (!result_send_coockie[HEADER_JSON_STATUS].asBool()) {
            logger.log("Bad response from DB request for cookie... : authorization controller");
            throw market_mentor::CreateCookieError();
        }
        logger.log("Add cookie to response... : authorization controller");
        result_send_coockie[HEADER_JSON_TOKEN] = cookie;
        return result_send_coockie;
    }
    logger.log("Bad password or login : authorization controller");
    return result_password_check;
}


Json::Value AuthorizeController::makeDBProtocol(const Json::Value& request) {
    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::POST_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::AUTHORIZATION;
    db_protocol[HEADER_JSON_LOGIN] = request[HEADER_JSON_LOGIN].asString();
    logger.log("Json DP prtocol auth completed successfully: authorization controller");
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
    Json::Value response_model = api_stock_->getOneStockPrise(protocol); // json
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


// class MiddleWare
MiddleWare::MiddleWare(const ptrToDBController db_controller)
    : db_controller_(db_controller) {}


cookie_map MiddleWare::checkCookieFile(const std::string& cookie) {
    
    Json::Value request_to_db = makeDBProtocol(cookie);
    Json::Value response_db = db_controller_->DataRequest(request_to_db);
    return paseDBResponse(response_db, cookie);
}

Json::Value MiddleWare::makeDBProtocol(const std::string& cookie) {
    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::GET_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::SESSION_REQUEST;
    db_protocol[HEADER_JSON_TOKEN] = cookie;
    logger.log("Json DP prtocol completed successfully: middleware");
    return db_protocol;
}

cookie_map MiddleWare::paseDBResponse(const Json::Value& response, const std::string& cookie) {
    if (!response[HEADER_JSON_STATUS].asBool()) {
        throw market_mentor::InvalidCookieError();
    }
    cookie_map result;
    result[response[HEADER_JSON_LOGIN].asString()] = cookie;
    return result;
}

// class ExitController
ExitController::ExitController(const ptrToDBController db_controller)
    : db_controller_(db_controller) {}

Json::Value ExitController::deleteCookie(const Json::Value& request) {
    Json::Value request_to_db = makeDBProtocol(request);

    logger.log("Request to DB... : exit controller");
    return db_controller_->DataRequest(request_to_db);
}

Json::Value ExitController::makeDBProtocol(const Json::Value& request) {
    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::GET_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::SESSION_DELETE;
    db_protocol[HEADER_JSON_TOKEN] = request[HEADER_JSON_TOKEN].asString();
    logger.log("Json DP prtocol completed successfully: exit controller");
    return db_protocol;
}

// class ChangeEmailController
ChangeEmailController::ChangeEmailController(const ptrToDBController db_controller)
    : db_controller_(db_controller) {}

Json::Value ChangeEmailController::changeEmail(const Json::Value& request) {
    Json::Value request_to_db_author = makeProtocolAuthor(request);
    logger.log("Request to DB auth. : change email controller");
    
    Json::Value response_db_auth = db_controller_->DataRequest(request_to_db_author);
    if (!response_db_auth[HEADER_JSON_STATUS].asBool()) {
        return response_db_auth;
    }
    response_db_auth[HEADER_JSON_STATUS] = (response_db_auth[HEADER_JSON_PASSWORD].asString() == hashing(request[HEADER_JSON_PASSWORD].asString()));
    if (!response_db_auth[HEADER_JSON_STATUS].asBool()) {
        return response_db_auth;
    }
    Json::Value request_to_db = makeDBProtocol(request);
    logger.log("Request to DB... : change email controller");
    return db_controller_->DataRequest(request_to_db);
}

Json::Value ChangeEmailController::makeDBProtocol(const Json::Value& request) {
    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::POST_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::CHANGE_USER_EMAIL_SETTINGS;
    db_protocol[HEADER_JSON_LOGIN] = request[HEADER_JSON_LOGIN].asString();
    db_protocol[HEADER_JSON_EMAIL] = request[HEADER_JSON_EMAIL].asString();
    logger.log("Json DP prtocol completed successfully: change email controller");
    return db_protocol;
}

Json::Value ChangeEmailController::makeProtocolAuthor(const Json::Value& request) {
    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::POST_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::AUTHORIZATION;
    db_protocol[HEADER_JSON_LOGIN] = request[HEADER_JSON_LOGIN].asString();
    logger.log("Json DP prtocol auth completed successfully: change email controller");
    return db_protocol;
}

// class ChangePasswordController
ChangePasswordController::ChangePasswordController(const ptrToDBController db_controller)
    : db_controller_(db_controller) {}

Json::Value ChangePasswordController::changePassword(const Json::Value& request) {
    Json::Value request_to_db_author = makeProtocolAuthor(request);
    logger.log("Request to DB auth. : change email controller");
    
    Json::Value response_db_auth = db_controller_->DataRequest(request_to_db_author);
    if (!response_db_auth[HEADER_JSON_STATUS].asBool()) {
        return response_db_auth;
    }
    response_db_auth[HEADER_JSON_STATUS] = (response_db_auth[HEADER_JSON_PASSWORD].asString() == hashing(request[HEADER_JSON_OLD_PASSWORD].asString()));
    if (!response_db_auth[HEADER_JSON_STATUS].asBool()) {
        return response_db_auth;
    }

    Json::Value request_to_db = makeDBProtocol(request);
    logger.log("Request to DB... : change passowrd controller");
    return db_controller_->DataRequest(request_to_db);
}

Json::Value ChangePasswordController::makeDBProtocol(const Json::Value& request) {
    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::POST_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::CHANGE_USER_PASSWORD_SETTINGS;
    db_protocol[HEADER_JSON_LOGIN] = request[HEADER_JSON_LOGIN].asString();
    db_protocol[HEADER_JSON_PASSWORD] = hashing(request[HEADER_JSON_PASSWORD].asString());
    logger.log("Json DP prtocol completed successfully: change passowrd controller");
    return db_protocol;
}

Json::Value ChangePasswordController::makeProtocolAuthor(const Json::Value& request) {
    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::POST_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::AUTHORIZATION;
    db_protocol[HEADER_JSON_LOGIN] = request[HEADER_JSON_LOGIN].asString();
    logger.log("Json DP prtocol auth completed successfully: change passowrd controller");
    return db_protocol;
}


// class GetStocksController
GetStocksController::GetStocksController(const ptrToDBController db_controller)
    : db_controller_(db_controller) {}

Json::Value GetStocksController::getNameStocks() {
    Json::Value request_to_db = makeDBProtocol();
    logger.log("Request to DB... : getStocks controller");
    return db_controller_->DataRequest(request_to_db);
}

Json::Value GetStocksController::makeDBProtocol() {
    Json::Value db_protocol;
    db_protocol[HEADER_JSON_TYPE] = TypeRequest::GET_REQUEST;
    db_protocol[HEADER_JSON_TYPEDATA] = TypeData::STOCKS_REQUEST;
    logger.log("Json DP prtocol completed successfully: getStocks controller");
    return db_protocol;
}


} // namespace controllers 
