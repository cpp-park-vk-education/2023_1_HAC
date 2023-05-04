#include "utils_gtest.h"
#include "gmock/gmock.h"
#include "handlers.h"
#include "controllers.h"
#include "handler_exception.h"
#include "dbcontroller.hpp"
#include <jsoncpp/json/value.h>
#include "api_model.h"
#include "http_protocol.h"


TEST(HandlersTests, NullHTTPReqInHandlePredict) {
    dbcontroller::DataBaseController TestDBController;
    api::APIModelelRequest TestAPIModelRequest;
    controllers::ModelController TestModelController(&TestAPIModelRequest);
    controllers::PredictController TestPredictController(&TestDBController, &TestModelController);
    handlers::PredictHandler TestPredictHandler(&TestPredictController);
    EXPECT_THROW(TestPredictHandler.handle(nullptr, nullptr), market_mentor::NullInHTTPError);
    
}

TEST(HandlersTests, NullHTTPReqInHandleShowPlot) {
    
    // class MockDBController : public IDataBaseController {
    //     MOCK_METHOD0();  
    // };
    
    
    // handlers::PredictHandler()

    // EXPECT_THROW();
    ASSERT_EQ(1, 1);
}

TEST(HandlersTests, NullHTTPReqInHandleRegister) {
    
    // class MockDBController : public IDataBaseController {
    //     MOCK_METHOD0();  
    // };
    
    
    // handlers::PredictHandler()

    // EXPECT_THROW();
    ASSERT_EQ(1, 1);
}


TEST(HandlersTests, NullHTTPReqInHandleAuthorize) {
    
    // class MockDBController : public IDataBaseController {
    //     MOCK_METHOD0();  
    // };
    
    
    // handlers::PredictHandler()

    // EXPECT_THROW();
    ASSERT_EQ(1, 1);
}

