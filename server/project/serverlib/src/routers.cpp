#include "routers.h"

RouterAdapter::RouterAdapter(handlers::IHandler* router): router_(router){};
void RouterAdapter::handle(httpRequest& req, httpResponse& resp){

    std::unique_ptr<IHTTPRequest> req_ = std::make_unique<HTTPRequestToBoostAdapter>(req);
    std::unique_ptr<IHTTPResponse> resp_ = std::make_unique<HTTPResponseToBoostAdapter>(resp);
    router_->handle(req_.get(), resp_.get());
};