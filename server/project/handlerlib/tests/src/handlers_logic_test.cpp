#include "utils_gtest.h"
#include "gmock/gmock.h"
#include "handlers.h"
#include "controllers.h"
#include "handler_exception.h"
#include "dbcontroller.hpp"
#include <jsoncpp/json/value.h>
#include "api_model.h"
#include "http_protocol.h"


TEST(PredictHandlerTest, HandleThrowsOnInvalidHttpRequest) {
    dbcontroller::DataBaseController db_ctrl;
    api::APIModelRequest api_model_request;

    controllers::ModelController model_ctrl(&api_model_request);

    controllers::PredictController controller(&db_ctrl, &model_ctrl);
    handlers::PredictHandler handler(&controller);

    HTTPRequestToBoostAdapter request;
    request.setUrl("/predict");
    request.setHeader("Content-Type", "application/json");
    request.setBody(std::vector<char>());

    HTTPResponseToBoostAdapter response;

    EXPECT_THROW(handler.handle(&request, &response), market_mentor::InvalidHttpRequestError);
}

// ShowPlotHandler test
TEST(ShowPlotHandlerTest, HandleThrowsOnInvalidHttpRequest) {
    dbcontroller::DataBaseController db_controller;

    controllers::ShowPlotController controller(&db_controller);
    handlers::ShowPlotHandler handler(&controller);

    HTTPRequestToBoostAdapter request;
    request.setUrl("/showplot");
    request.setHeader("Content-Type", "application/json");
    request.setBody(std::vector<char>());

    HTTPResponseToBoostAdapter response;

    // Act, Assert
    EXPECT_THROW(handler.handle(&request, &response), market_mentor::InvalidHttpRequestError);
}

// RegisterHandler test
TEST(RegisterHandlerTest, HandleThrowsOnInvalidHttpRequest) {
    dbcontroller::DataBaseController db_controller;

    controllers::RegisterController controller(&db_controller);
    handlers::RegisterHandler handler(&controller);

    HTTPRequestToBoostAdapter request;
    request.setUrl("/register");
    request.setHeader("Content-Type", "application/json");
    request.setBody(std::vector<char>());

    HTTPResponseToBoostAdapter response;

    // Act, Assert
    EXPECT_THROW(handler.handle(&request, &response), market_mentor::InvalidHttpRequestError);
}

// AuthorizeHandler test
TEST(AuthorizeHandlerTest, HandleThrowsOnInvalidHttpRequest) {
    dbcontroller::DataBaseController db_controller;
    controllers::AuthorizeController controller(&db_controller);
    handlers::AuthorizeHandler handler(&controller);

    HTTPRequestToBoostAdapter request;
    request.setUrl("/authorize");
    request.setHeader("Content-Type", "application/json");
    request.setBody(std::vector<char>());

    HTTPResponseToBoostAdapter response;

    // Act, Assert
    EXPECT_THROW(handler.handle(&request, &response), market_mentor::InvalidHttpRequestError);
}



