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
    InvalidHttpRequestError() :
        MarketMentorException("Invalid HTTP request") {}
};

class InvalidJsonFormatError : public MarketMentorException {
 public:
    explicit InvalidJsonFormatError(const std::string& message) :
        MarketMentorException("Invalid JSON format: " + message) {}
};


class ErrorInGetDataFromDB : public MarketMentorException {
 public:
    explicit ErrorInGetDataFromDB(const std::string& message) :
        MarketMentorException("Get false status, when try get " + message + ", time: ") {}
};




} // namespace market_mentor