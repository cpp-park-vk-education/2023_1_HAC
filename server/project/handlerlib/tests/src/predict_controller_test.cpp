#include "controllers_test.h"
#include <vector>

// Мок класса IShowPlotController
class MockModelController : public controllers::IModelController {
public:
   MOCK_METHOD(Json::Value, callModelApi, (const controllers::TimeSeriesPredicts& samples_data), (override));
};


class PredictControllerTest: public ::testing::Test {
public:
    void SetUp() { 
        ptr_db_controller = new MockDataBaseController;
        ptr_model_controller = new MockModelController;
    }
    void TearDown() { 
        delete ptr_db_controller;
        delete ptr_model_controller;
    };
protected:
        MockDataBaseController* ptr_db_controller;
        MockModelController* ptr_model_controller;
};

// TESTS
TEST_F(PredictControllerTest, CheckCorrectPositiveResponse) {
    controllers::PredictController predict_controller(ptr_db_controller, ptr_model_controller);

    Json::Value expected_json_to_db;
    expected_json_to_db[HEADER_JSON_TYPE] = TypeRequest::GET_REQUEST;
    expected_json_to_db[HEADER_JSON_TYPEDATA] = TypeData::TIMESERIES_REQUEST;
    expected_json_to_db[HEADER_JSON_NAME_STOCK] = "test";
    expected_json_to_db[HEADER_JSON_LEN_LAGS] = WINDOW_SIZE;


    Json::Value data_db_response;
    data_db_response[0] = 1;
    data_db_response[1] = 2;
    data_db_response[2] = 3;
    data_db_response[3] = 4;
    data_db_response[4] = 5;
    data_db_response[5] = 6;
    data_db_response[6] = 7;
    data_db_response[7] = 8;
    Json::Value expected_db_return;
    expected_db_return[HEADER_JSON_STATUS] = true;
    expected_db_return[HEADER_JSON_DB_STATUS_OPEN] = true;
    expected_db_return[HEADER_JSON_DATA] = data_db_response;

    EXPECT_CALL(*ptr_db_controller, DataRequest(expected_json_to_db)).WillOnce(Return(expected_db_return));
    
    controllers::TimeSeriesPredicts ts;
    ts.lenpredict = 2;
    ts.matrix_samples = std::vector<double>{1, 2, 3, 4, 5, 6, 7, 8};

    Json::Value data_predict;
    data_predict[0] = 1;
    data_predict[1] = 2;
    Json::Value expected_model_return;
    expected_model_return[HEADER_JSON_STATUS] = true;
    expected_model_return[HEADER_JSON_DATA] = data_predict;

    EXPECT_CALL(*ptr_model_controller, callModelApi(ts)).WillOnce(Return(expected_model_return));

    Json::Value request;
    request[HEADER_JSON_NAME_STOCK] = "test";
    request[HEADER_JSON_LENPREDICT] = 2;

    EXPECT_EQ(predict_controller.makePredict(request), expected_model_return);
}

TEST_F(PredictControllerTest, CheckCorrectNegativeResponseDBIsClosed) {
    controllers::PredictController predict_controller(ptr_db_controller, ptr_model_controller);

    Json::Value expected_json_to_db;
    expected_json_to_db[HEADER_JSON_TYPE] = TypeRequest::GET_REQUEST;
    expected_json_to_db[HEADER_JSON_TYPEDATA] = TypeData::TIMESERIES_REQUEST;
    expected_json_to_db[HEADER_JSON_NAME_STOCK] = "test";
    expected_json_to_db[HEADER_JSON_LEN_LAGS] = WINDOW_SIZE;


    Json::Value expected_db_return;
    expected_db_return[HEADER_JSON_STATUS] = false;
    expected_db_return[HEADER_JSON_DB_STATUS_OPEN] = false;

    EXPECT_CALL(*ptr_db_controller, DataRequest(expected_json_to_db)).WillOnce(Return(expected_db_return));
    
    Json::Value request;
    request[HEADER_JSON_NAME_STOCK] = "test";
    request[HEADER_JSON_LENPREDICT] = 2;

    Json::Value expected_return;
    expected_return[HEADER_JSON_STATUS] = false;
    expected_return[HEADER_JSON_SERVER_ERROR] = true;
    expected_return[HEADER_JSON_ERROR] = "Get false status from DB, when try get timeseries. Problem with DB.";

    EXPECT_EQ(predict_controller.makePredict(request), expected_return);
}


TEST_F(PredictControllerTest, CheckCorrectNegativeResponseDataNotFound) {
    controllers::PredictController predict_controller(ptr_db_controller, ptr_model_controller);

    Json::Value expected_json_to_db;
    expected_json_to_db[HEADER_JSON_TYPE] = TypeRequest::GET_REQUEST;
    expected_json_to_db[HEADER_JSON_TYPEDATA] = TypeData::TIMESERIES_REQUEST;
    expected_json_to_db[HEADER_JSON_NAME_STOCK] = "test";
    expected_json_to_db[HEADER_JSON_LEN_LAGS] = WINDOW_SIZE;


    Json::Value expected_db_return;
    expected_db_return[HEADER_JSON_STATUS] = false;
    expected_db_return[HEADER_JSON_DB_STATUS_OPEN] = true;

    EXPECT_CALL(*ptr_db_controller, DataRequest(expected_json_to_db)).WillOnce(Return(expected_db_return));
    
    Json::Value request;
    request[HEADER_JSON_NAME_STOCK] = "test";
    request[HEADER_JSON_LENPREDICT] = 2;

    Json::Value expected_return;
    expected_return[HEADER_JSON_STATUS] = false;
    expected_return[HEADER_JSON_SERVER_ERROR] = false;
    expected_return[HEADER_JSON_ERROR] = "Get false status from DB, when try get timeseries. Not found data request.";

    EXPECT_EQ(predict_controller.makePredict(request), expected_return);
}
