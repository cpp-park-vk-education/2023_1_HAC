#pragma once // NO_LINT
#include <chrono>
#include <sw/redis++/redis++.h>
#include "imemorydatabase.hpp"

namespace database {
    
    class RedisServer: public IMemoryDataBase {
    public:
        RedisServer();
        RedisServer(const std::string& host,  int port, const std::string& password);

        bool IsOpen() override;
        bool Connect() override;
        bool Insert(const std::string& key, const std::string& value, const size_t& ttl) override;
        bool Delete(const std::string& key) override;
        bool Update(const std::string& key, const std::string& value, const size_t& ttl) override;
        bool Has(const std::string& key) override;
        std::string Get(const std::string& key) override;

    private:
        std::shared_ptr<sw::redis::Redis> redis_;
        std::string host_;
        int port_;
        std::string password_;

    };    
}
