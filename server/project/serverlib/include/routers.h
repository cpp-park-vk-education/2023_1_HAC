#pragma once
#include <map>
#include <boost/beast/http.hpp>

using httpRequest = boost::beast::http::request<http::string_body>;
using httpResponse = boost::beast::http::response<http::string_body>;

class IRouter {
 public:
    virtual httpResponse handle(httpRequest) = 0; 
    virtual void setHandlers(std::map<string, *IHandler>) = 0;
};

class Router : public IRouter {
 public: 
    Router();
    httpResponse handle(httpRequest) override;
    void setHandlers(std::map<string, *IHandler>) override;

 private:
    std::map<string, *IHandler> handlers;

    IHendler* getHandler(httpRequest);
}