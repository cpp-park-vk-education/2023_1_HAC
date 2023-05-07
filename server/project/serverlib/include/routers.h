#pragma once //NO_LINT
#include <map>
#include <boost/beast/http.hpp>

#include "ihandler.h"
#include "handlers.h"

class IRouterAdapter {
 public:
    virtual void handle(httpRequest& req, httpResponse& resp) = 0; 
};

class RouterAdapter : public IRouterAdapter {
 public: 
    RouterAdapter();
    void handle(httpRequest& req, httpResponse& resp) override;

 private:

   handlers::IHandler* router;
};