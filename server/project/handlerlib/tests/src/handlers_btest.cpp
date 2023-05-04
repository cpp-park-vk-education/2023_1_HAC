#include "utils_gtest.h"
#include "gmock/gmock.h"
#include "handlers.h"
#include "handler_exception.h"
#include "dbcontroller.hpp"
#include <jsoncpp/json/value.h>
#include "api_model.h"
#include "http_protocol.h"

// TEST(HandlersTests, NullHTTPReqInHandlePredict) {

//     class MockDBController : public dbcontroller::IDataBaseController {
//         MOCK_METHOD(Json::Value, DataRequest, (const Json::Value& request), (override));
//         MOCK_METHOD(bool, TimeSeriesPost, (const std::shared_ptr<TimeSeriesData>& data));
//         MOCK_METHOD(std::shared_ptr<TimeSeriesData>, TimeSeriesGet, (const std::string& name_stock, const std::string& len_lags));
//         MOCK_METHOD(bool, ClientRequestPost, (const std::shared_ptr<ClientData>& data));
//         MOCK_METHOD(std::shared_ptr<ClientData>, ClientRequestGet, (const std::string& key));
//         MOCK_METHOD(bool, ClientRequestUpdate, (const std::shared_ptr<ClientData> data));
//     };    
//     MockDBController TestDBController;


//     class MockAPIModelelRequest : public IAPIModelRequest {
//         MOCK_METHOD(IHTTPResponse, getData, (IHTTPRequest req), (override));
//     };    
//     MockAPIModelelRequest TestAPIModelelRequest;

//     controllers::ModelController TestModelController(&TestAPIModelelRequest);
//     controllers::PredictController TestPredictController(&TestModelController, &TestDBController);
//     handlers::PredictHandler TestPredictHandler(&TestPredictController);

//     EXPECT_THROW(TestPredictHandler.handle(nullptr, nullptr), market_mentor::NullInHTTPError);

// }

TEST(HandlersTests, NullHTTPReqInHandleShowPlot) {
    
    // class MockDBController : public IDataBaseController {
    //     MOCK_METHOD0();  
    // };
    
    
    // handlers::PredictHandler()

    // EXPECT_THROW();
    ASSERT_EQ(1, 2);
}

TEST(HandlersTests, NullHTTPReqInHandleRegister) {
    
    // class MockDBController : public IDataBaseController {
    //     MOCK_METHOD0();  
    // };
    
    
    // handlers::PredictHandler()

    // EXPECT_THROW();
    ASSERT_EQ(1, 2);
}


TEST(HandlersTests, NullHTTPReqInHandleAuthorize) {
    
    // class MockDBController : public IDataBaseController {
    //     MOCK_METHOD0();  
    // };
    
    
    // handlers::PredictHandler()

    // EXPECT_THROW();
    ASSERT_EQ(1, 2);
}

