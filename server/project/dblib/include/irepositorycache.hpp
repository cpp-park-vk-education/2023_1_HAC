#pragma once // NO_LINT
#include "dbexception.hpp"

namespace cache {

    template <typename Key, typename Data>
    class IRepositoryCache {
    public:
        virtual bool Has(const Key& key) = 0;
        virtual bool Insert(const Key& key, const Data& data) = 0;
        virtual bool Delete(const Key& key) = 0;
        virtual Data Get(const Key& key) = 0;
        virtual size_t GetSize() = 0;
        virtual void DeleteAll() = 0;
    };

}