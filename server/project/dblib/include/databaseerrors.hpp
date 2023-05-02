#pragma once
#include <string>
#include <exception>

class BasePostgresError: public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class ConnectError: public BasePostgresError {
public:
    ConnectError(const std::string& what): BasePostgresError{what}{} 
};

class SqlError: public BasePostgresError {
public:
    SqlError(const std::string& what): BasePostgresError{what}{} 
};

class TransactionError: public BasePostgresError {
public:
    TransactionError(const std::string& what): BasePostgresError{what}{} 
};

