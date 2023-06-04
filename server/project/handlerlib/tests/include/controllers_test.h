#pragma once // NO_LINT

// external dependencies
#include "gmock/gmock.h"

// internal dependencies
#include "utils_gtest_handlers.h"

#include "dbcontroller.hpp"
#include "controllers.h"
#include "icontrollers.h"
#include "handler_exception.h"

using ::testing::Return;
using ::testing::Throw;
using ::testing::_;

// CONST
const size_t WINDOW_SIZE = 300;

const std::string HEADER_JSON_NAME_STOCK = "name_stock";
const std::string HEADER_JSON_LENPREDICT = "lenpredict";
const std::string HEADER_JSON_TYPE = "Type";
const std::string HEADER_JSON_TYPEDATA = "TypeData";
const std::string HEADER_JSON_LEN_LAGS = "len_lags";
const std::string HEADER_JSON_DATA = "param";
const std::string HEADER_JSON_DATE = "date";
const std::string HEADER_JSON_STATUS = "status";
const std::string HEADER_JSON_ERROR = "error";
const std::string HEADER_JSON_DB_STATUS_OPEN = "DatabaseIsOpen";
const std::string HEADER_JSON_SERVER_ERROR = "server_error";

// MOCKS
class MockDataBaseController : public dbcontroller::IDataBaseController {
public:
    MOCK_METHOD(Json::Value, DataRequest, (const Json::Value& request), (override));
    MOCK_METHOD(bool, ConnectToDatabase, (), (override));
    MOCK_METHOD(void, SetDatabaseConfig, (const std::string&  addr, const std::string&  port,
            const std::string&  db_name, const std::string&  user, const std::string&  pass), (override));
    MOCK_METHOD(void, SetMemoryDatabaseConfig, (const std::string& addr, const std::string&  port, const std::string&  pass), (override));

    MOCK_METHOD(Json::Value, TimeSeriesPost, (const Json::Value& data), (override));
    MOCK_METHOD(Json::Value, TimeSeriesGet, (const Json::Value& data), (override));
    MOCK_METHOD(Json::Value, StocksGet, (), (override));
    MOCK_METHOD(Json::Value, TimeSeriesFill, (const Json::Value& data), (override));

    MOCK_METHOD(Json::Value, ClientRequestPost, (const Json::Value& data), (override));
    MOCK_METHOD(Json::Value, ClientRequestGet, (const TypeData& request_type, const std::string& key), (override));
    MOCK_METHOD(Json::Value, ClientRequestUpdate, (const Json::Value& data), (override));

    MOCK_METHOD(Json::Value, TokenRequestGet, (const Json::Value& request), (override));
    MOCK_METHOD(Json::Value, TokenRequestPost, (const Json::Value& request), (override));
    MOCK_METHOD(Json::Value, TokenRequestDelete, (const std::string& key), (override));
     
};