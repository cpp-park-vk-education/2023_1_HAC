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
    auto router_adapter = new MockRouterAdapter();
    auto session = new Session(std::move(socket), router_adapter);

    EXPECT_THROW(session->run(), std::bad_weak_ptr);
    delete session;
}