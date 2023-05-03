#pragma once // NO_LINT

#include <boost/beast/http.hpp>


using IHTTPRequest = boost::beast::http::request<http::string_body>;
using IHTTPResponse = boost::beast::http::response<http::string_body>;


namespace handlers {

class IHandler {
 public:
    virtual void handle(IHTTPRequest request, IHTTPResponse response) = 0;
 private:
};


} // namespace handlers
