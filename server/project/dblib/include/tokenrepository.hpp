#pragma once // NO_LINT
#include <memory>
#include "redisserver.hpp"
#include "data.hpp"

using namespace database;

namespace repository {
    class  ITokenRepository {
    public:
        virtual bool Insert(const TokenData& data) = 0;
        virtual std::shared_ptr<TokenData> Get(const std::string key) = 0;
    };

    class TokenRepository: public ITokenRepository {
    public:
        TokenRepository();
        TokenRepository(const std::shared_ptr<IMemoryDataBase>& db);

        bool Insert(const TokenData& data) override;
        std::shared_ptr<TokenData> Get(const std::string key) override;
    
    private:
        std::shared_ptr<IMemoryDataBase> database_;
    };
}