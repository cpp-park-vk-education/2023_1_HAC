#pragma once //NO_LINT
#include <boost/beast/http.hpp>
#include <string>
#include <map>
#include <vector>


using httpRequest = boost::beast::http::request<boost::beast::http::string_body>;
using httpResponse = boost::beast::http::response<boost::beast::http::string_body>;

class IHTTPResponse{
 public:
    virtual void setStatus(int status_code) = 0;
    virtual void setHeader(std::string header, std::string value) = 0;
    virtual void setBody(std::vector<char> bytes) = 0;
};

class IHTTPRequest{
 public:
    virtual void getURL() = 0;  
    virtual std::map<std::string, std::string> getHeaders() = 0;
    virtual std::vector<char> getBoby();
};

class HTTPResponseToBoostAdapter : public IHTTPResponse{
 public:
    HTTPResponseToBoostAdapter();
    httpResponse toBoost();

    void setStatus(int status_code) override;
    void setHeader(std::string header, std::string value) override;
    void setBody(std::vector<char> bytes) override;

 private:
    httpResponse response;
};

class HTTPRequestToBoostAdapter : public IHTTPRequest{
 public:
    HTTPRequestToBoostAdapter();
    HTTPRequestToBoostAdapter(std::string url, std::string headers, std::string body);
    
    void getURL() override;
    std::map<std::string, std::string> getHeaders() override;
    std::vector<char> getBoby() override;

    void toIRequest(httpRequest);

 private:
    httpRequest request;
};