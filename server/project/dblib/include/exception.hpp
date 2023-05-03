#pragma once // NO_LINT
#include <string>
#include <exception>

// Postgres

class BaseError: public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class ConnectError: public BaseError {
public:
    ConnectError(const std::string& what): BaseError{what}{} 
};

class SqlError: public BaseError {
public:
    SqlError(const std::string& what): BaseError{what}{} 
};

class TransactionError: public BaseError {
public:
    TransactionError(const std::string& what): BaseError{what}{} 
};

// Repository

class IncorrectData: public BaseError {
public:
    IncorrectData(const std::string& what): BaseError{what}{} 
};

class UnavailableDataBase: public BaseError {
public:
    UnavailableDataBase(const std::string& what): BaseError{what}{} 
};