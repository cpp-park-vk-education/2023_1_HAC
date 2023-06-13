#pragma once // NO LINT
#include "exception.h"

namespace market_mentor {

    class InvalidServerConfig : public MarketMentorException {
     public:
        InvalidServerConfig(const std::string& s) :
            MarketMentorException(s){};
    };

    class ConnectDatabaseException : public MarketMentorException {
     public:
        ConnectDatabaseException() :
            MarketMentorException("Cannot connect to database"){};
    };

    class CreatingNullptr : public MarketMentorException {
     public:
        CreatingNullptr(const std::string& s) :
            MarketMentorException(s){};
    };

    class ConnectionException : public MarketMentorException {
     public:
        ConnectionException(const std::string& s) :
            MarketMentorException(s){};
    };

    class ReedDataException : public MarketMentorException {
        public:
            ReedDataException(const std::string& s) :
            MarketMentorException(s){};
    };

    class ParseDataException : public MarketMentorException {
        public:
            ParseDataException(const std::string& s) :
            MarketMentorException(s){};
    };

    class UpdateDataException : public MarketMentorException {
        public:
            UpdateDataException() :
            MarketMentorException("Cannot create update data job"){};
    };

    class ColdStartException : public MarketMentorException {
        public:
            ColdStartException() :
            MarketMentorException("Cannot start server"){};
    };
}