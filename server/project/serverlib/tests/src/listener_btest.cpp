#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "server.h"
#include "listener.h"

using boost::asio::ip::tcp;

class MockRouter : public IRouterAdapter {
 public:
    MOCK_METHOD(void, handle, (httpRequest& req, httpResponse& resp), (override));
};

TEST(ListenerTest, StartListening){
    auto endpoint = tcp::endpoint{boost::asio::ip::make_address("127.0.0.1"), 8080};
    auto ioc = net::io_context{1};
    auto router = new MockRouter;
    auto listener = new Listener(ioc, endpoint, router);

    EXPECT_THROW(listener->run(), std::bad_weak_ptr);

    delete router;
    delete listener;
}
