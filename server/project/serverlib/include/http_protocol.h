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
   virtual void setHeader(const std::string &header, const std::string &value) = 0;
   virtual void setBody(const std::string &bytes) = 0;
  
   virtual int getStatus() = 0;
   virtual std::map<std::string, std::string> getHeaders() = 0;
   virtual std::string getBody() = 0;
};

class IHTTPRequest{
 public:
    virtual void setStatus(int status_code) = 0;
    virtual void setUrl(const std::string& url) = 0;
    virtual void setHeader(const std::string &header, const std::string &value) = 0;
    virtual void setBody(const std::string &bytes) = 0;

    // virtual int getStatus() = 0;
    virtual std::string getURL() = 0;  
    virtual std::map<std::string, std::string> getHeaders() = 0;
    virtual std::string getBoby() = 0;
};

class HTTPResponseToBoostAdapter : public IHTTPResponse{
 public:
    HTTPResponseToBoostAdapter(httpResponse &response);

    void setStatus(int status_code) override;
    void setHeader(const std::string &header, const std::string &value) override;
    void setBody(const std::string &bytes) override;

    int getStatus() override;
    std::map<std::string, std::string> getHeaders() override;
    std::string getBody() override;

 private:
    httpResponse* response_;
};

class HTTPRequestToBoostAdapter : public IHTTPRequest{
 public:
   HTTPRequestToBoostAdapter(httpRequest &request);
   HTTPRequestToBoostAdapter(const std::string &url, const std::string &headers, const std::string &body);
   HTTPRequestToBoostAdapter(const std::string &url, const std::string &headers, std::string body);
     
   void setStatus(int status_code) override;
   void setUrl(const std::string& url) override;
   void setHeader(const std::string &header, const std::string &value) override;
   void setBody(const std::string &bytes) override;
   
   std::string getURL() override;
   std::map<std::string, std::string> getHeaders() override;
   std::string getBoby() override;

 private:
   httpRequest* request_;
};