#include "utils_gtest.h"
#include "gmock/gmock.h"
#include "handlers.h"
#include "controllers.h"
#include "handler_exception.h"
#include "dbcontroller.hpp"
#include <jsoncpp/json/value.h>
#include "api_model.h"
#include "http_protocol.h"


TEST(HandlersTests, NullHTTPReqInHandlerPredict) {
    dbcontroller::DataBaseController db_ctrl;
    api::APIModelRequest api_model_request;

    controllers::ModelController model_ctrl(&api_model_request);
    
    controllers::PredictController predict_ctrl(&db_ctrl, &model_ctrl);

    handlers::PredictHandler predict_handler(&predict_ctrl);
    EXPECT_THROW(predict_handler.handle(nullptr, nullptr), market_mentor::NullInHTTPError);
    
}

TEST(HandlersTests, NullHTTPReqInHandlerShowPlot) {
    dbcontroller::DataBaseController db_controller;
   
    controllers::ShowPlotController mock_show_plot_ctrl(&db_controller);

    handlers::ShowPlotHandler show_plot_handler(&mock_show_plot_ctrl);
    EXPECT_THROW(show_plot_handler.handle(nullptr, nullptr), market_mentor::NullInHTTPError);
}

TEST(HandlersTests, NullHTTPReqInHandlerRegister) {
    dbcontroller::DataBaseController db_controller;

    controllers::RegisterController reg_controller(&db_controller);

    handlers::RegisterHandler reg_handler(&reg_controller);
    EXPECT_THROW(reg_handler.handle(nullptr, nullptr), market_mentor::NullInHTTPError);
}


TEST(HandlersTests, NullHTTPReqInHandlerAuthorize) {
    dbcontroller::DataBaseController db_controller;

    controllers::AuthorizeController auth_controller(&db_controller);

    handlers::AuthorizeHandler auth_handler(&auth_controller);
    EXPECT_THROW(auth_handler.handle(nullptr, nullptr), market_mentor::NullInHTTPError);
}


TEST(PredictHandlerTest, ConstructorThrowsOnNullController) {
    EXPECT_THROW(handlers::PredictHandler{nullptr}, market_mentor::NullControllerInHandlerError);
}

TEST(ShowPlotHandlerTest, ConstructorThrowsOnNullController) {
    EXPECT_THROW(handlers::ShowPlotHandler{nullptr}, market_mentor::NullControllerInHandlerError);
}

TEST(RegisterHandlerTest, ConstructorThrowsOnNullController) {
    EXPECT_THROW(handlers::RegisterHandler{nullptr}, market_mentor::NullControllerInHandlerError);
}

TEST(AuthorizeHandlerTest, ConstructorThrowsOnNullController) {
    EXPECT_THROW(handlers::AuthorizeHandler{nullptr}, market_mentor::NullControllerInHandlerError);
}
