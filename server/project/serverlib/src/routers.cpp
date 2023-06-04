#include "routers.h"
#include <iostream>

RouterAdapter::RouterAdapter(handlers::IHandler* router): router_(router){};
void RouterAdapter::handle(httpRequest& req, httpResponse& resp){

    std::cerr << req.body();
    auto req_ = std::make_unique<HTTPRequestToBoostAdapter>(req);
    auto resp_ = std::make_unique<HTTPResponseToBoostAdapter>(resp);
    router_->handle(req_.get(), resp_.get());
};