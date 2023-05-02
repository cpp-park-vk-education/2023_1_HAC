#pragma once 
#include <iostream>
#include <memory>

template <typename Key, typename Data>
class IRepositoryCache {
public:
    virtual bool Has(const Key& key) = 0;
    virtual void Insert(const Key& key, const Data& data) = 0;
    virtual void Delete(const Key& key) = 0;
    virtual Data Get(const Key& key) = 0;
};

template <typename Key, typename Data>
class RepositoryCache: public IRepositoryCache<Key,Data> {
public:
    RepositoryCache();

    bool Has(const Key& key) override;
    void Insert(const Key& key, const Data& data) override;
    void Delete(const Key& key) override;
    Data Get(const Key& key) override;    
    
};



template <typename Key, typename Data>
RepositoryCache<Key, Data>::RepositoryCache() {
}

template <typename Key, typename Data>
bool RepositoryCache<Key, Data>::Has(const Key& key) {
    return true;
}
template <typename Key, typename Data>
void RepositoryCache<Key, Data>::Insert(const Key& key, const Data& data) {
    return;
}

template <typename Key, typename Data>
void RepositoryCache<Key, Data>::Delete(const Key& key) {
    return;
}

template <typename Key, typename Data>
Data RepositoryCache<Key, Data>::Get(const Key& key) {
    Data a;
    return a;
}
