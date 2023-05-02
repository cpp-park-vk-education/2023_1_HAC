#pragma once // NO_LINT

#include <boost/beast/http.hpp>


using httpRequest = boost::beast::http::request<http::string_body>;
using httpResponse = boost::beast::http::response<http::string_body>;


namespace handlers {

class IHandler {
 public:
    virtual httpResponse hasndle(httpRequest request) = 0;
 private:
};


} // namespace handlers
