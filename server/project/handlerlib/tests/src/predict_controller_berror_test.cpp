#include "utils_gtest.h"
#include "http_protocol.h"
#include "dbcontroller.hpp"
#include "controllers.h"
#include "dbcontroller.hpp"
#include "icontrollers.h"
#include "handler_exception.h"

#include <vector>
#include <gmock/gmock.h>
#include <jsoncpp/json/value.h>



TEST(PredictControllerTest, InvalidRequestFormat) {
    dbcontroller::DataBaseController db_ctrl;
    api::APIModelRequest api_model_request;
    controllers::ModelController model_ctrl(&api_model_request);

    controllers::PredictController controller(&db_ctrl, &model_ctrl);

    Json::Value request;
    request["samples_data"] = "invalid_format";
    request["window_size"] = 2;

    EXPECT_THROW(controller.makePredict(request), market_mentor::InvalidJsonFormatError);
}


TEST(PredictControllerTest, InvalidWindowSize) {
    dbcontroller::DataBaseController db_ctrl;
    api::APIModelRequest api_model_request;
    controllers::ModelController model_ctrl(&api_model_request);

    controllers::PredictController controller(&db_ctrl, &model_ctrl);

    Json::Value request;
    request["samples_data"] = Json::arrayValue;
    request["window_size"] = -2;

    EXPECT_THROW(controller.makePredict(request), market_mentor::InvalidJsonFormatError);
}


TEST(PredictControllerTest, MissingSamplesData) {
    dbcontroller::DataBaseController db_ctrl;
    api::APIModelRequest api_model_request;
    controllers::ModelController model_ctrl(&api_model_request);
    controllers::PredictController controller(&db_ctrl, &model_ctrl);

    Json::Value request;
    request["window_size"] = 2;

    EXPECT_THROW(controller.makePredict(request), market_mentor::InvalidJsonFormatError);
}

TEST(PredictControllerTest, EmptySamplesDataArray) {
    dbcontroller::DataBaseController db_ctrl;
    api::APIModelRequest api_model_request;
    controllers::ModelController model_ctrl(&api_model_request);
    controllers::PredictController controller(&db_ctrl, &model_ctrl);

    Json::Value request;
    request["samples_data"] = Json::arrayValue;
    request["window_size"] = 2;

    EXPECT_THROW(controller.makePredict(request), std::invalid_argument);
}

TEST(PredictControllerTest, MissingWindowSize) {
    dbcontroller::DataBaseController db_ctrl;
    api::APIModelRequest api_model_request;
    controllers::ModelController model_ctrl(&api_model_request);
    controllers::PredictController controller(&db_ctrl, &model_ctrl);

    Json::Value request;
    request["samples_data"] = Json::arrayValue;

    EXPECT_THROW(controller.makePredict(request), market_mentor::InvalidJsonFormatError);
}

