#pragma once // NO_LINT

#include "exception.h"
#include <chrono>


namespace market_mentor {

class NullInHTTPError : public MarketMentorException {
 public:
    NullInHTTPError() :
        MarketMentorException("Passed nullptr in HTTP") {}
};

class NullControllerInHandlerError : public MarketMentorException {
 public:
    NullControllerInHandlerError() :
        MarketMentorException("Passed nullptr in Controller") {}
};


class InvalidHttpRequestError : public MarketMentorException {
 public:
    InvalidHttpRequestError(const std::string& message) :
        MarketMentorException("Invalid HTTP request (" + message + ")") {}
};

class InvalidJsonFormatError : public MarketMentorException {
 public:
    explicit InvalidJsonFormatError(const std::string& message) :
        MarketMentorException("Invalid JSON format: " + message) {}
};


class ErrorInGetDataFromDBInternal : public MarketMentorException {
 public:
    explicit ErrorInGetDataFromDBInternal(const std::string& message) :
        MarketMentorException("Get false status from DB, when try get " + message + 
        ". Problem with DB.") {}
};

class ErrorInGetDataFromDBNotFound : public MarketMentorException {
 public:
    explicit ErrorInGetDataFromDBNotFound(const std::string& message) :
        MarketMentorException("Get false status from DB, when try get " + message + 
        ". Not found data request.") {}
};

class InvalidCookieError : public MarketMentorException {
 public:
    InvalidCookieError() :
        MarketMentorException("Invalid cookie. Refused.") {}
};

class CreateCookieError : public MarketMentorException {
 public:
    CreateCookieError() :
        MarketMentorException("Error when try create cookie.") {}
};

} // namespace market_mentor