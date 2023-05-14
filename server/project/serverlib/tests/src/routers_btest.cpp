#include <gmock/gmock.h>
#include "routers.h"


class MockHandler : public handlers::IHandler {
public:
    MOCK_METHOD(void, handle, (IHTTPRequest*, IHTTPResponse*), (override));

};

TEST(RouterAdapterTest, BasicRequest) {

    auto mock_router = new ::testing::StrictMock<MockHandler>;
    auto router_adapter = RouterAdapter(mock_router);
    IHTTPRequest* req = new HTTPRequestToBoostAdapter();
    IHTTPResponse* resp = new HTTPResponseToBoostAdapter();

    httpRequest request_;
    httpResponse response_;

    EXPECT_CALL(*mock_router, handle(req, resp)).Times(1);

    router_adapter.handle(request_, response_);

    delete mock_router;
    delete req;
    delete resp;
}

TEST(RouterAdapterTest, WrongHTTPMethod) {

    auto mock_router = new ::testing::StrictMock<MockHandler>;
    auto router_adapter = RouterAdapter(mock_router);
    IHTTPRequest* req = new HTTPRequestToBoostAdapter();
    IHTTPResponse* resp = new HTTPResponseToBoostAdapter();

    httpRequest request_;
    httpResponse response_;

    request_.method(boost::beast::http::verb::put);
    EXPECT_CALL(*mock_router, handle(req, resp)).Times(0);

    router_adapter.handle(request_, response_);

    delete mock_router;
    delete req;
    delete resp;
}


TEST(RouterAdapterTest, WrongHTTPVersion) {

    auto mock_router = new ::testing::StrictMock<MockHandler>;
    auto router_adapter = RouterAdapter(mock_router);
    IHTTPRequest* req = new HTTPRequestToBoostAdapter();
    IHTTPResponse* resp = new HTTPResponseToBoostAdapter();

    httpRequest request_;
    httpResponse response_;

    request_.version(10);
    EXPECT_CALL(*mock_router, handle(req, resp)).Times(0);

    router_adapter.handle(request_, response_);

    delete mock_router;
    delete req;
    delete resp;
}