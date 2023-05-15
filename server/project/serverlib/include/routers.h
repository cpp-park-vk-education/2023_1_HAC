#pragma once //NO_LINT
#include <map>
#include <boost/beast/http.hpp>

#include "ihandler.h"
#include "http_protocol.h"

class IRouterAdapter {
 public:
    virtual void handle(httpRequest& req, httpResponse& resp) = 0; 
};

class RouterAdapter : public IRouterAdapter {
 public: 
    RouterAdapter(handlers::IHandler* router);
    void handle(httpRequest& req, httpResponse& resp) override;

 private:

   handlers::IHandler* router_;
};