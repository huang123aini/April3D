//
//  Vector.h
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/6.
//

#ifndef Vector_h
#define Vector_h

#include "base/aprilMacros.h"
#include "base/Ref.h"
#include <vector>
#include <functional>
#include <algorithm>

APRIL_BEGIN

template<class T>
class Vector {
public:
    using iterator = typename std::vector<T>::iterator;

    using const_iterator = typename std::vector<T>::const_iterator;

    using reverse_iterator = typename std::vector<T>::reverse_iterator;

    using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;
    
    iterator begin() { return data_.begin(); }

    const_iterator begin() const { return data_.begin(); }
    
    iterator end() { return data_.end(); }

    const_iterator end() const { return data_.end(); }
    
    const_iterator cbegin() const { return data_.cbegin(); }

    const_iterator cend() const { return data_.cend(); }
    
    reverse_iterator rbegin() { return data_.rbegin(); }

    const_reverse_iterator rbegin() const { return data_.rbegin(); }
    
    reverse_iterator rend() { return data_.rend(); }
  
    const_reverse_iterator rend() const { return data_.rend(); }
    
    const_reverse_iterator crbegin() const { return data_.crbegin(); }
   
    const_reverse_iterator crend() const { return data_.crend(); }
    
    Vector<T>()
    : data_() {
        static_assert(std::is_convertible<T, Ref*>::value, "Invalid Type for april3d::Vector<T>!");
    }
    
    explicit Vector<T>(ssize_t capacity)
    : data_() {
        static_assert(std::is_convertible<T, Ref*>::value, "Invalid Type for april3d::Vector<T>!");
        printf("In the default constructor with capacity of Vector. \n");
        reserve(capacity);
    }

    Vector<T>(std::initializer_list<T> list) {
        for (auto& element : list) {
           pushBack(element);
        }
    }

    ~Vector<T>() {
        printf("In the destructor of Vector. \n");
        clear();
    }

    Vector<T>(const Vector<T>& other) {
        static_assert(std::is_convertible<T, Ref*>::value, "Invalid Type for cocos2d::Vector<T>!");
        printf("In the copy constructor! \n");
        data_ = other.data_;
        addRefForAllObjects();
    }
    
    Vector<T>(Vector<T>&& other) {
        static_assert(std::is_convertible<T, Ref*>::value, "Invalid Type for cocos2d::Vector<T>!");
        printf("In the move constructor of Vector! \n");
        data_ = std::move(other.data_);
    }
    
    Vector<T>& operator=(const Vector<T>& other) {
        if (this != &other) {
            printf("In the copy assignment operator! \n");
            clear();
            data_ = other.data_;
            addRefForAllObjects();
        }
        return *this;
    }
    

    Vector<T>& operator=(Vector<T>&& other) {
        if (this != &other) {
            printf("In the move assignment operator! \n");
            clear();
            data_ = std::move(other.data_);
        }
        return *this;
    }
    
    void reserve(ssize_t n) {
        data_.reserve(n);
    }
 
    ssize_t capacity() const {
        return data_.capacity();
    }
    
    ssize_t size() const {
        return  data_.size();
    }
    
    bool empty() const {
        return data_.empty();
    }
     
    ssize_t max_size() const {
        return data_.max_size();
    }
    
    ssize_t getIndex(T object) const {
        auto iter = std::find(data_.begin(), data_.end(), object);
        if (iter != data_.end())
            return iter - data_.begin();

        return -1;
    }
 
    const_iterator find(T object) const {
        return std::find(data_.begin(), data_.end(), object);
    }
    
    iterator find(T object) {
        return std::find(data_.begin(), data_.end(), object);
    }
    

    T at(ssize_t index) const {
        if(index < 0 || index >= size()) {
            printf("");
        }
        CCASSERT( index >= 0 && index < size(), "index out of range in getObjectAtIndex()");
        return data_[index];
    }

    T front() const {
        return data_.front();
    }

    T back() const {
        return data_.back();
    }

    T getRandomObject() const {
        if (!data_.empty()) {
            ssize_t randIdx = RandomHelper::random_int<int>(0, static_cast<int>(data_.size()) - 1);
            return *(data_.begin() + randIdx);
        }
        return nullptr;
    }

    bool contains(T object) const {
        return( std::find(data_.begin(), data_.end(), object) != data_.end() );
    }

    bool equals(const Vector<T> &other) const {
        ssize_t s = this->size();
        if (s != other.size())
            return false;
        
        for (ssize_t i = 0; i < s; i++) {
            if (this->at(i) != other.at(i)) {
                return false;
            }
        }
        return true;
    }

    void pushBack(T object) {
        if(object == nullptr) {
            printf("The object should not be nullptr \n");
        }
        data_.push_back( object );
        object->retain();
    }
    
    void pushBack(const Vector<T>& other) {
        for(const auto &obj : other) {
            data_.push_back(obj);
            obj->retain();
        }
    }

    void insert(ssize_t index, T object) {
        if(index < 0 || index > size()) {
            printf("Failed: Invalid index! \n");
            return;
        }
        if(object == nullptr) {
            printf("Failed: The object should not be nullptr \n");
        }
        data_.insert((std::begin(data_) + index), object);
        object->retain();
    }
    
    void popBack() {
        if(data.empty()) {
            printf("no objects added \n");
            return;
        }
        auto last = data_.back();
        data_.pop_back();
        last->release();
    }
    
    void eraseObject(T object, bool removeAll = false) {
        if(object == nullptr) {
            printf("The object should not be nullptr \n");
            return;
        }
        if (removeAll) {
            for (auto iter = data_.begin(); iter != data_.end();) {
                if ((*iter) == object) {
                    iter = data_.erase(iter);
                    object->release();
                } else {
                    ++iter;
                }
            }
        } else {
            auto iter = std::find(data_.begin(), data_.end(), object);
            if (iter != data_.end()) {
                data_.erase(iter);
                object->release();
            }
        }
    }

    iterator erase(iterator position) {
        if(position < data_.begin() || position >= data_.end()) {
            printf("Invalid position! \n");
        }
        (*position)->release();
        return data_.erase(position);
    }
    
    iterator erase(iterator first, iterator last) {
        for (auto iter = first; iter != last; ++iter) {
            (*iter)->release();
        }
        
        return data_.erase(first, last);
    }
    
    iterator erase(ssize_t index) {
        if(data_.empty() || index < 0 || index >= size()) {
            printf("Failed: Invalid index! \n");
        }
        auto it = std::next( begin(), index );
        (*it)->release();
        return data_.erase(it);
    }
 
    void clear() {
        for( auto& it : data_) {
            it->release();
        }
        data_.clear();
    }

    void swap(T object1, T object2) {
        ssize_t idx1 = getIndex(object1);
        ssize_t idx2 = getIndex(object2);

        if(idx1 < 0 || idx2 < 0) {
            printf("invalid object index \n");
        }
        std::swap( data_[idx1], data_[idx2] );
    }
    
    void swap(ssize_t index1, ssize_t index2) {
        if(index1 < 0 || index1 >= size() || index2 < 0 || index2 >= size()) {
            printf("Failed: Invalid indices \n");
        }
        std::swap( data_[index1], data_[index2] );
    }

    void replace(ssize_t index, T object) {
        if(index < 0 || index > size()) {
            printf("Failed: Invalid Index. \n");
            return;
        }
        if(object == nullptr) {
            printf("Failed: The object should not be nullptr \n");
            return;
        }
        data_[index]->release();
        data_[index] = object;
        object->retain();
    }

    void reverse() {
        std::reverse( std::begin(data_), std::end(data_) );
    }
    
    void shrinkToFit() {
        data_.shrink_to_fit();
    }
    
protected:

    void addRefForAllObjects() {
        for(const auto &obj : data_) {
            obj->retain();
        }
    }
    
    std::vector<T> data_;
};

APRIL_END

#endif /* Vector_h */
