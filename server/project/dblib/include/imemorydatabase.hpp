#pragma once // NO_LINT
#include <string>
#include <memory>

namespace database {
    
class IMemoryDataBase {
public:
    virtual bool IsOpen() = 0;
    virtual bool Connect() = 0;
    virtual bool Insert(const std::string& key, const std::string& value, const size_t& ttl) = 0;
    virtual bool Delete(const std::string& key) = 0;
    virtual bool Update(const std::string& key, const std::string& value, const size_t& ttl) = 0;
    virtual bool Has(const std::string& key) = 0;
    virtual std::string Get(const std::string& key) = 0;    
};
 
} //namespace database
