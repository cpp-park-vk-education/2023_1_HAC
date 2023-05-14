#pragma once // NO_LINT
#include "exception.h"

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




} // namespace market_mentor