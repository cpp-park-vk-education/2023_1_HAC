#pragma once
#include <string>
#include <exception>

class BaseRepositoryError: public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class IncorrectData: public BaseRepositoryError {
public:
    IncorrectData(const std::string& what): BaseRepositoryError{what}{} 
};

class UnavailableDataBase: public BaseRepositoryError {
public:
    UnavailableDataBase(const std::string& what): BaseRepositoryError{what}{} 
};