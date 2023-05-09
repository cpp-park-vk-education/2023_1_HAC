#pragma once // NO_LINT
#include <iostream>
#include <memory>
#include <queue>
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
        std::queue<Key> keys_queue_;
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
        keys_queue_.push(key);
        return true;
    }


    template <typename Key, typename Data>
    bool RepositoryCache<Key, Data>::Delete(const Key& key) {
        if (!Has(key)) {
            return false;
        }        

        cache_.erase(key);
        std::queue<Key> new_queue;
        Key front_key;
        for (size_t i = 0; i < size_ ; i++) {
            front_key = keys_queue_.front();
            if (front_key != key) {
                new_queue.push(front_key);
                keys_queue_.pop();                
            }
        }

        keys_queue_ = std::move(new_queue);
        size_--;
        return true;
    }

    template <typename Key, typename Data>
    void RepositoryCache<Key, Data>::DeleteFirstElem() {
        cache_.erase(keys_queue_.front());
        keys_queue_.pop();
        size_--;
    }

    template <typename Key, typename Data>
    Data RepositoryCache<Key, Data>::Get(const Key& key) {
        if (!Has(key)) {
            throw ElementNotExist("[ERROR] Element with this key doesn't exists ");
        }  

        Data result = cache_[key];
        std::queue<Key> new_queue;
        Key front_key;
        for (size_t i = 0; i < size_ ; i++) {
            front_key = keys_queue_.front();
            if (front_key != key) {
                new_queue.push(front_key);
                keys_queue_.pop();                
            }
        }

        keys_queue_ = std::move(new_queue);
        keys_queue_.push(key);
        return result;
    }

    template <typename Key, typename Data>
    size_t RepositoryCache<Key, Data>::GetSize() {
        return size_;
    }

    template <typename Key, typename Data>
    void RepositoryCache<Key, Data>::DeleteAll() {
        std::string front_key;
        while (size_ != 0) {
            front_key = keys_queue_.front();
            cache_.erase(front_key);
            keys_queue_.pop();      
            size_--;          
        }
    }
}