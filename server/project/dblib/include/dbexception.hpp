#pragma once // NO_LINT
#include <string>
#include "exception.h"

// Postgres

class ConnectError: public market_mentor::MarketMentorException {
public:
    ConnectError(const std::string& what): market_mentor::MarketMentorException{what}{} 
};

class SqlError: public market_mentor::MarketMentorException {
public:
    SqlError(const std::string& what): market_mentor::MarketMentorException{what}{} 
};

class TransactionError: public market_mentor::MarketMentorException {
public:
    TransactionError(const std::string& what): market_mentor::MarketMentorException{what}{} 
};

// Database and Cache

class ElementNotExist: public market_mentor::MarketMentorException {
public:
    ElementNotExist(const std::string& what): market_mentor::MarketMentorException{what}{} 
};


// Repository

// class IncorrectData: public market_mentor::MarketMentorException {
// public:
//     IncorrectData(const std::string& what): market_mentor::MarketMentorException{what}{} 
// };

// class UnavailableDataBase: public market_mentor::MarketMentorException {
// public:
//     UnavailableDataBase(const std::string& what): market_mentor::MarketMentorException{what}{} 
// };
