#pragma once // NO_LINT
#include <string>
#include <jsoncpp/json/json.h>
#include "dbexception.hpp"

namespace database {
    
class IDataBase{
public:
    virtual bool IsOpen() = 0;
    virtual bool SendQuery(const std::string& query) = 0;
    virtual bool Connect() = 0;

    virtual Json::Value GetData(const std::string& query) = 0;
    virtual Json::Value GetRow(const std::string& query) = 0;
};

} // namespace database
