//
//  Map.h
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/6.
//

#ifndef Map_h
#define Map_h

#define USE_STD_UNORDERED_MAP 1

#include "base/aprilMacros.h"
#include "base/Ref.h"
#include <vector>

#if USE_STD_UNORDERED_MAP
#include <unordered_map>
#else
#include <map>
#endif

APRIL_BEGIN

template <class K, class V>
class Map {
public:
#if USE_STD_UNORDERED_MAP
    typedef std::unordered_map<K, V> RefMap;
#else
    typedef std::map<K, V> RefMap;
#endif
    
    typedef typename RefMap::iterator iterator;

    typedef typename RefMap::const_iterator const_iterator;
    
    iterator begin() { return data_.begin(); }

    const_iterator begin() const { return data_.begin(); }
    
    iterator end() { return data_.end(); }

    const_iterator end() const { return data_.end(); }
    
    const_iterator cbegin() const { return data_.cbegin(); }

    const_iterator cend() const { return data_.cend(); }
    
    Map<K, V>()
    : data_() {
        static_assert(std::is_convertible<V, Ref*>::value, "Invalid Type for april3d::Map<K, V>!");
        printf("In the default constructor of Map! \n");
    }
    
    explicit Map<K, V>(ssize_t capacity)
    : data_() {
        static_assert(std::is_convertible<V, Ref*>::value, "Invalid Type for april3d::Map<K, V>!");
        printf("In the constructor with capacity of Map! \n");
        data_.reserve(capacity);
    }

    Map<K, V>(const Map<K, V>& other) {
        static_assert(std::is_convertible<V, Ref*>::value, "Invalid Type for april3d::Map<K, V>!");
        printf("In the copy constructor of Map! \n");
        data_ = other.data_;
        addRefForAllObjects();
    }

    Map<K, V>(Map<K, V>&& other) {
        static_assert(std::is_convertible<V, Ref*>::value, "Invalid Type for cocos2d::Map<K, V>!");
        printf("In the move constructor of Map! \n");
        data_ = std::move(other.data_);
    }
    
    ~Map<K, V>() {
        printf("In the destructor of Map! \n");
        clear();
    }
    
    void reserve(ssize_t capacity)
    {
#if USE_STD_UNORDERED_MAP
        data_.reserve(capacity);
#endif
    }
    
    ssize_t bucketCount() const
    {
#if USE_STD_UNORDERED_MAP
        return data_.bucket_count();
#else
        return 0;
#endif
    }
    
    ssize_t bucketSize(ssize_t n) const
    {
#if USE_STD_UNORDERED_MAP
        return data_.bucket_size(n);
#else
        return 0;
#endif
    }
    
    ssize_t bucket(const K& k) const
    {
#if USE_STD_UNORDERED_MAP
        return data_.bucket(k);
#else
        return 0;
#endif
    }
    
    ssize_t size() const {
        return data_.size();
    }

    bool empty() const {
        return data_.empty();
    }
    
    std::vector<K> keys() const {
        std::vector<K> keys;

        if (!data_.empty()) {
            keys.reserve(data_.size());
            
            for (const auto& iter : data_) {
                keys.push_back(iter.first);
            }
        }
        return keys;
    }
    
    std::vector<K> keys(V object) const {
        std::vector<K> keys;
        
        if (!data_.empty()) {
            keys.reserve(data_.size() / 10);
            
            for (const auto& iter : data_) {
                if (iter.second == object) {
                    keys.push_back(iter.first);
                }
            }
        }
        keys.shrink_to_fit();
        
        return keys;
    }
    
    const V at(const K& key) const {
        auto iter = data_.find(key);
        if (iter != data_.end())
            return iter->second;
        return nullptr;
    }
    
    V at(const K& key) {
        auto iter = data_.find(key);
        if (iter != data_.end())
            return iter->second;
        return nullptr;
    }

    const_iterator find(const K& key) const {
        return data_.find(key);
    }
    
    iterator find(const K& key) {
        return data_.find(key);
    }
    
    void insert(const K& key, V object) {
        if(object == nullptr) {
            printf("Object is nullptr! \n");
            return;
        }
        object->retain();
        erase(key);
        data_.emplace(key, object);
    }
    
    iterator erase(const_iterator position) {
        if(position == data_.cend()) {
            printf("Failed: Invalid iterator! \n");
        }
        position->second->release();
        return data_.erase(position);
    }
    
    size_t erase(const K& k)
    {
        auto iter = data_.find(k);
        if (iter != data_.end())
        {
            iter->second->release();
            data_.erase(iter);
            return 1;
        }
        return 0;
    }
    
    void erase(const std::vector<K>& keys) {
        for(const auto &key : keys) {
            this->erase(key);
        }
    }
    
    void clear() {
        for (const auto& iter : data_) {
            iter.second->release();
        }
        
        data_.clear();
    }
    
    V getRandomObject() const {
        if (!data_.empty()) {
            ssize_t randIdx = RandomHelper::random_int<int>(0, static_cast<int>(data_.size()) - 1);
            const_iterator randIter = data_.begin();
            std::advance(randIter , randIdx);
            return randIter->second;
        }
        return nullptr;
    }
    
    Map<K, V>& operator= ( const Map<K, V>& other ) {
        if (this != &other) {
            printf("In the copy assignment operator of Map! \n");
            clear();
            data_ = other.data_;
            addRefForAllObjects();
        }
        return *this;
    }
    
    Map<K, V>& operator= ( Map<K, V>&& other ) {
        if (this != &other) {
            printf("In the move assignment operator of Map! \n");
            clear();
            data_ = std::move(other.data_);
        }
        return *this;
    }
    
protected:
    void addRefForAllObjects() {
        for (auto& iter : data_) {
            iter.second->retain();
        }
    }
    RefMap data_;
};


APRIL_END

#endif /* Map_h */
