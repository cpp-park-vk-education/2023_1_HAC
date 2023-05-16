#pragma once // NO_LINT
#include <string>
#include <memory>
#include <list>
#include <unordered_map>
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

    template <typename Key, typename Data>
    class RepositoryCache: public IRepositoryCache<Key,Data> {
    public:
        RepositoryCache();
        RepositoryCache(size_t max_size);

        bool Has(const Key& key) override;
        bool Insert(const Key& key, const Data& data) override;
        bool Delete(const Key& key) override;
        Data Get(const Key& key) override; 
        size_t GetSize() override;
        void DeleteAll() override;

    private:
        void DeleteFirstElem();

        size_t size_;
        size_t max_size_;
        std::list<Key> keys_list_;
        std::unordered_map<Key, Data> cache_;
    };

    template <typename Key, typename Data>
    RepositoryCache<Key, Data>::RepositoryCache(): size_(0), max_size_ (10) {
    }

    template <typename Key, typename Data>
    RepositoryCache<Key, Data>::RepositoryCache(size_t max_size): size_(0), max_size_ (max_size) {
    }

    template <typename Key, typename Data>
    bool RepositoryCache<Key, Data>::Has(const Key& key) {
        if (cache_.find(key) == cache_.end()) {
            return false;
        }
        else {
            return true;
        }
    }

    template <typename Key, typename Data>
    bool RepositoryCache<Key, Data>::Insert(const Key& key, const Data& data) {
        if (Has(key)) {
            return false;
        }

        if (size_ == max_size_) {
            DeleteFirstElem();
        }

        cache_[key] = data;
        size_++;
        keys_list_.push_back(key);
        return true;
    }


    template <typename Key, typename Data>
    bool RepositoryCache<Key, Data>::Delete(const Key& key) {
        if (!Has(key)) {
            return false;
        }        

        cache_.erase(key);
        keys_list_.remove(key);
        return true;
    }

    template <typename Key, typename Data>
    void RepositoryCache<Key, Data>::DeleteFirstElem() {
        cache_.erase(keys_list_.front());
        keys_list_.pop_front();
        size_--;
    }

    template <typename Key, typename Data>
    Data RepositoryCache<Key, Data>::Get(const Key& key) {
        if (!Has(key)) {
            throw ElementNotExist("[ERROR] Element with this key doesn't exists ");
        }  

        Data result = cache_[key];
        keys_list_.remove(key);
        keys_list_.push_back(key);
        return result;
    }

    template <typename Key, typename Data>
    size_t RepositoryCache<Key, Data>::GetSize() {
        return size_;
    }

    template <typename Key, typename Data>
    void RepositoryCache<Key, Data>::DeleteAll() {
        std::string front_key;
        keys_list_.clear();
        size_ = 0;
        cache_.clear();
    }
    
}