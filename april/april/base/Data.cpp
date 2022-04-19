//
//  Data.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/6.
//

#include "base/Data.h"

APRIL_BEGIN

const Data Data::Null;

Data::Data() :
bytes_(nullptr),
size_(0) {}

Data::Data(Data&& other) :
bytes_(nullptr),
size_(0) {
    printf("In the move constructor of Data. \n");
    move(other);
}

Data::Data(const Data& other) :
bytes_(nullptr),
size_(0) {
    printf("In the copy constructor of Data. \n");
    if (other.bytes_ && other.size_) {
        copy(other.bytes_, other.size_);
    }
}

Data::~Data() {
    printf("deallocing Data: %p", this);
    clear();
}

Data& Data::operator= (const Data& other) {
    if (this != &other) {
        printf("In the copy assignment of Data.");
        copy(other.bytes_, other.size_);
    }
    return *this;
}

Data& Data::operator= (Data&& other) {
    if (this != &other) {
        printf("In the move assignment of Data.");
        move(other);
    }
    return *this;
}

void Data::move(Data& other) {
    if(bytes_ != other.bytes_) clear();
    
    bytes_ = other.bytes_;
    size_ = other.size_;
    other.bytes_ = nullptr;
    other.size_ = 0;
}

bool Data::isNull() const {
    return (bytes_ == nullptr || size_ == 0);
}

unsigned char* Data::getBytes() const {
    return bytes_;
}

ssize_t Data::getSize() const {
    return size_;
}

ssize_t Data::copy(const unsigned char* bytes, const ssize_t size) {
    if(size < 0) {
        printf("copy size should be non-negative. \n");
        return -1;
    }
    if(!bytes) {
        printf("bytes should not be nullptr \n");
        return -1;
    }
    if (size <= 0) return 0;

    if (bytes != bytes_) {
        clear();
        bytes_ = (unsigned char*)malloc(sizeof(unsigned char) * size);
        memcpy(bytes_, bytes, size);
    }

    size_ = size;
    return size_;
}

void Data::fastSet(unsigned char* bytes, const ssize_t size) {
    if(size < 0) {
        printf("fastSet size should be non-negative. \n");
    }
    bytes_ = bytes;
    size_ = size;
}

void Data::clear() {
    if(bytes_) free(bytes_);
    bytes_ = nullptr;
    size_ = 0;
}

unsigned char* Data::takeBuffer(ssize_t* size) {
    auto buffer = getBytes();
    if (size) {
        *size = getSize();
    }
    fastSet(nullptr, 0);
    return buffer;
}

APRIL_END
