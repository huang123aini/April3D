//
//  Data.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/6.
//

#ifndef Data_hpp
#define Data_hpp

#include <stdio.h>

#include "platform/AprilPlatformMacros.h"
#include <stdint.h>
#include <string>

APRIL_BEGIN

class  Data {
    friend class Properties;

public:

    static const Data Null;

    Data();

    Data(const Data& other);

    Data(Data&& other);

    ~Data();

    Data& operator= (const Data& other);

    Data& operator= (Data&& other);

    unsigned char* getBytes() const;

    ssize_t getSize() const;

    ssize_t copy(const unsigned char* bytes, const ssize_t size);

    void fastSet(unsigned char* bytes, const ssize_t size);

    void clear();

    bool isNull() const;

    unsigned char* takeBuffer(ssize_t* size);
private:
    void move(Data& other);

private:
    unsigned char* bytes_;
    ssize_t size_;
};


APRIL_END

#endif /* Data_hpp */
