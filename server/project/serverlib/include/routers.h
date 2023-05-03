#pragma once //NO_LINT
#include <map>
#include <boost/beast/http.hpp>

#include "ihandler.h"
#include "icontrollers.h"
#include "handlers.h"
#include "controllers.h"
#include "config_handler.h"

using httpRequest = boost::beast::http::request<http::string_body>;
using httpResponse = boost::beast::http::response<http::string_body>;

class IRouterAdapter {
 public:
    virtual void handle(httpRequest& req, httpResponse& resp) = 0; 
};

class RouterAdapter : public IRouterAdapter {
 public: 
    RouterAdapter();
    void handle(httpRequest& req, httpResponse& resp) override;

 private:

   IHendler* router;
}

  std::map<string, *IHandler> handlers;