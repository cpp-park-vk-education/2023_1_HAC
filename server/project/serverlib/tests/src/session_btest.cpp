#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "session.h"

using boost::asio::ip::tcp;

class MockRouterAdapter : public IRouterAdapter {
 public:
    MOCK_METHOD(void, handle, (httpRequest& req, httpResponse& resp), (override));
};

TEST(SessionTest, StartSession){
    auto io_context = net::io_context();
    auto socket = tcp::socket(io_context);
    auto session = new Session(std::move(socket));

    EXPECT_NO_THROW(session->run());
    delete session;
}

TEST(SessionTest, SetRouter){
    auto router_adapter = new MockRouterAdapter();
    auto io_context = net::io_context();
    auto socket = tcp::socket(io_context);
    auto session = new Session(std::move(socket));

    session->run();
    EXPECT_NO_THROW(session->setRouterAdapter(router_adapter));
    delete session;
}