#include "controllers_test.h"

class ShowPlotControllerTest: public ::testing::Test {
public:
    void SetUp() { 
        ptr_db_controller = new MockDataBaseController;
    }
    void TearDown() { 
        delete ptr_db_controller;
    };
protected:
        MockDataBaseController* ptr_db_controller;
};


// TESTS
TEST_F(ShowPlotControllerTest, CheckCorrectPositiveResponse) {
    controllers::ShowPlotController showplot_controller(ptr_db_controller);

    Json::Value expected_json_to_db;
    expected_json_to_db[HEADER_JSON_TYPE] = TypeRequest::GET_REQUEST;
    expected_json_to_db[HEADER_JSON_TYPEDATA] = TypeData::TIMESERIES_REQUEST;
    expected_json_to_db[HEADER_JSON_NAME_STOCK] = "test";
    expected_json_to_db[HEADER_JSON_LEN_LAGS] = 4;


    Json::Value data_db_response;
    data_db_response[0] = 1;
    data_db_response[1] = 2;
    data_db_response[2] = 3;
    data_db_response[3] = 4;

    Json::Value expected_db_return;
    expected_db_return[HEADER_JSON_STATUS] = true;
    expected_db_return[HEADER_JSON_DB_STATUS_OPEN] = true;
    expected_db_return[HEADER_JSON_DATA] = data_db_response;

    EXPECT_CALL(*ptr_db_controller, DataRequest(expected_json_to_db)).WillOnce(Return(expected_db_return));
    
    Json::Value request;
    request[HEADER_JSON_NAME_STOCK] = "test";
    request[HEADER_JSON_LEN_LAGS] = 4;


    EXPECT_EQ(showplot_controller.createPlotData(request), expected_db_return);
}


TEST_F(ShowPlotControllerTest, CheckCorrectNegativeResponseDBIsClosed) {
    controllers::ShowPlotController showplot_controller(ptr_db_controller);

    Json::Value expected_json_to_db;
    expected_json_to_db[HEADER_JSON_TYPE] = TypeRequest::GET_REQUEST;
    expected_json_to_db[HEADER_JSON_TYPEDATA] = TypeData::TIMESERIES_REQUEST;
    expected_json_to_db[HEADER_JSON_NAME_STOCK] = "test";
    expected_json_to_db[HEADER_JSON_LEN_LAGS] = 4;

    Json::Value expected_db_return;
    expected_db_return[HEADER_JSON_STATUS] = false;
    expected_db_return[HEADER_JSON_DB_STATUS_OPEN] = false;


    EXPECT_CALL(*ptr_db_controller, DataRequest(expected_json_to_db)).WillOnce(Return(expected_db_return));
    
    Json::Value request;
    request[HEADER_JSON_NAME_STOCK] = "test";
    request[HEADER_JSON_LEN_LAGS] = 4;


    EXPECT_EQ(showplot_controller.createPlotData(request), expected_db_return);
}


TEST_F(ShowPlotControllerTest, CheckCorrectNegativeResponseDataNotFound) {
    controllers::ShowPlotController showplot_controller(ptr_db_controller);

    Json::Value expected_json_to_db;
    expected_json_to_db[HEADER_JSON_TYPE] = TypeRequest::GET_REQUEST;
    expected_json_to_db[HEADER_JSON_TYPEDATA] = TypeData::TIMESERIES_REQUEST;
    expected_json_to_db[HEADER_JSON_NAME_STOCK] = "test";
    expected_json_to_db[HEADER_JSON_LEN_LAGS] = 4;

    Json::Value expected_db_return;
    expected_db_return[HEADER_JSON_STATUS] = false;
    expected_db_return[HEADER_JSON_DB_STATUS_OPEN] = true;


    EXPECT_CALL(*ptr_db_controller, DataRequest(expected_json_to_db)).WillOnce(Return(expected_db_return));
    
    Json::Value request;
    request[HEADER_JSON_NAME_STOCK] = "test";
    request[HEADER_JSON_LEN_LAGS] = 4;


    EXPECT_EQ(showplot_controller.createPlotData(request), expected_db_return);
}