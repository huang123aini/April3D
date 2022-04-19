//
//  AsyncTaskPool.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/5.
//

#include "base/AsyncTaskPool.h"

APRIL_BEGIN

AsyncTaskPool* AsyncTaskPool::s_asyncTaskPool = nullptr;

AsyncTaskPool* AsyncTaskPool::getInstance() {
    if (s_asyncTaskPool == nullptr) {
        s_asyncTaskPool = new (std::nothrow) AsyncTaskPool();
    }
    return s_asyncTaskPool;
}

void AsyncTaskPool::destroyInstance() {
    delete s_asyncTaskPool;
    s_asyncTaskPool = nullptr;
}

AsyncTaskPool::AsyncTaskPool() {}

AsyncTaskPool::~AsyncTaskPool(){}

APRIL_END
