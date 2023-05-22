#include <gmock/gmock.h>
#include "routers.h"


class MockHandler : public handlers::IHandler {
public:
    MOCK_METHOD(void, handle, (IHTTPRequest*, IHTTPResponse*), (override));
    Json::Value parseInputHttpRequest(const std::string& message) override {
        
    }
    void makeResponse(IHTTPResponse* response, const Json::Value& response_json) override {
    }
};

TEST(RouterAdapterTest, BasicRequest) {

    auto mock_router = new ::testing::StrictMock<MockHandler>;
    auto router_adapter = RouterAdapter(mock_router);
    httpRequest request_;
    httpResponse response_;

    EXPECT_CALL(*mock_router, handle(testing::_, testing::_)).Times(1);

    router_adapter.handle(request_, response_);

    delete mock_router;
};