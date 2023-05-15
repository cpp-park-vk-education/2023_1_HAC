#include "utils_gtest.h"
#include "http_protocol.h"
#include "dbcontroller.hpp"
#include "controllers.h"
#include "dbcontroller.hpp"
#include "icontrollers.h"

#include <map>
#include <memory>
#include <vector>
#include <gmock/gmock.h>
#include <jsoncpp/json/value.h>



TEST(PredictControllerTest, MakePredictReturnsCorrectSizeJson) {
    dbcontroller::DataBaseController db_ctrl;
    api::APIModelRequest api_model_request;
    controllers::ModelController model_ctrl(&api_model_request);

    controllers::PredictController controller(&db_ctrl, &model_ctrl);

    const std::string json_str = R"({
        "samples_data": [
            [1.0, 2.0],
            [3.0, 4.0]
        ],
        "window_size": 2
    })";

    Json::Value request;
    Json::Reader reader;
    ASSERT_TRUE(reader.parse(json_str, request));

    Json::Value response = controller.makePredict(request);
    ASSERT_EQ(response["predicted_values"].size(), 2);
}


