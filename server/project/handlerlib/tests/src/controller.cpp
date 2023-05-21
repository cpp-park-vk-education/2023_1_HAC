#include "dbcontroller.hpp"
// class MockDataBaseController : public dbcontroller::IDataBaseController {
// public:
//     MOCK_METHOD(Json::Value, DataRequest, (const Json::Value& request), (override));
//     MOCK_METHOD(bool, ConnectToDatabase, (), (override));
//     MOCK_METHOD(void, SetDatabaseConfig, (const std::string&  addr, const std::string&  port,
//         const std::string& db_name, const std::string&  user, const std::string&  pass), (override));

//     MOCK_METHOD( Json::Value, TimeSeriesPost, (const Json::Value& data), (override));
//     MOCK_METHOD(Json::Value, TimeSeriesGet, (const std::string& name_stock, const std::string& len_lags), (override));

//     MOCK_METHOD(Json::Value, ClientRequestPost, (const Json::Value& data), (override));
//     MOCK_METHOD(Json::Value, ClientRequestGet, (const std::string& key), (override));
//     MOCK_METHOD(Json::Value, ClientRequestUpdate, (const Json::Value& data), (override));
// };


// // Мок класса IModelController
// class MockModelController : public controllers::IModelController {
// public:
//     MOCK_METHOD(callModelApi, Json::Value(const TimeSeriesPredicts&));
// };

// // Мок класса IShowPlotController
// class MockShowPlotController : public controllers::IShowPlotController {
// public:
//     MOCK_METHOD(createPlotData, Json::Value(const Json::Value&));
// };

// // Мок класса IRegisterController
// class MockRegisterController : public controllers::IRegisterController {
// public:
//     MOCK_METHOD(registration, Json::Value(Json::Value&));
//     MOCK_METHOD(makeDBProtocol, Json::Value(const Json::Value&));
// };

// // Мок класса IAuthorizeController
// class MockAuthorizeController : public controllers::IAuthorizeController {
// public:
//     MOCK_METHOD(authorization, Json::Value(Json::Value&));
//     MOCK_METHOD(makeDBProtocol, Json::Value(const Json::Value&));
//     MOCK_METHOD(checkPassword, Json::Value(const Json::Value&, const Json::Value&));
// };

// // Мок класса IUpdateDataController
// class MockUpdateDataController : public controllers::IUpdateDataController {
// public:
//     MOCK_METHOD(updateData, bool(const handlers::ProtocolAPI&));
//     MOCK_METHOD(makeDBProtocol, Json::Value(const Json::Value&));
// };