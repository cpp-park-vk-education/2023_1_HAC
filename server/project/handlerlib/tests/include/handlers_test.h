#pragma once // NO_LINT

// external dependencies
#include "gmock/gmock.h"

// internal dependencies
#include "utils_gtest_handlers.h"
#include "http_protocol.h"
#include "icontrollers.h"
#include "handler_exception.h"
#include "handlers.h"
#include "controllers.h"


// CONST
const std::string INVALID_HTTP_PREDICT = "Invalid HTTP request (predict)";
const std::string INVALID_HTTP_PLOT = "Invalid HTTP request (plot)";

const std::string ERROR_MESSAGE = "Error-Message";
const std::string PREDICT_DATA = "Predict-Data";
const std::string PLOT_DATA = "Plot-Data";
const std::string HEADER_JSON_ERROR = "error";
const std::string HEADER_JSON_NAME_STOCK = "name_stock";
const std::string HEADER_JSON_LEN_LAGS = "len_lags";
const std::string HEADER_JSON_WINDOW_SIZE = "window_size";
const std::string HEADER_JSON_DATA = "data";
const std::string HEADER_JSON_STATUS = "status";

const int OK = 200;
const int BAD_REQUEST = 400;
const int NOT_FOUND = 404;
const int UNAUTHORIZED = 401;
const int INTERNAL_SERVER_ERROR = 500;


using ::testing::Return;
using ::testing::Throw;
using ::testing::_;


// GLOBAL MOCKS
class MockHTTPRequest: public IHTTPRequest {
public:
    MOCK_METHOD(void, setStatus, (int status_code), (override));
    MOCK_METHOD(void, setUrl, (const std::string& url), (override));
    MOCK_METHOD(void, setHeader, (const std::string &header, const std::string &value), (override));
    MOCK_METHOD(void, setBody, (const std::string &bytes), (override));
    MOCK_METHOD(std::string, getURL, (), (override));
    MOCK_METHOD((std::map<std::string, std::string>), getHeaders, (), (override));
    MOCK_METHOD(std::string, getBoby, (), (override));
};

class MockHTTPResponse: public IHTTPResponse {
public:
    MOCK_METHOD(void, setStatus, (int status_code), (override));
    MOCK_METHOD(void, setHeader, (const std::string &header, const std::string &value), (override));
    MOCK_METHOD(void, setBody, (const std::string &bytes), (override));
    MOCK_METHOD(std::string, getURL, (), (override));
    MOCK_METHOD(int, getStatus, (), (override));
    MOCK_METHOD((std::map<std::string, std::string>), getHeaders, (), (override));
    MOCK_METHOD(std::string, getBoby, (), (override));
    
};
