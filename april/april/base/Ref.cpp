//
//  Ref.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/2.
//
//c++知识点: typeid(p)表示的是p的类型
//

#include "base/Ref.h"
#include "base/AutoreleasePool.h"
#include "base/aprilMacros.h"

#if CC_REF_LEAK_DETECTION
#include <algorithm>
#include <thread>
#include <mutex>
#include <vector>
#endif

APRIL_BEGIN

#if REF_LEAK_DETECTION
static void trackRef(Ref* ref);
static void untrackRef(Ref* ref);
#endif

Ref::Ref()
: reference_count(1) {
#if REF_LEAK_DETECTION
    trackRef(this);
#endif
}

Ref::~Ref() {
#if REF_LEAK_DETECTION
    if (reference_count != 0) {
        untrackRef(this);
    }
#endif
}

void Ref::retain() {
    if (reference_count <= 0) {
        printf("Ref::release reference count should be greater than 0. \n");
        return;
    }
    ++reference_count;
}

void Ref::release() {
    if (reference_count <= 0) {
        printf("Ref::release reference count should be greater than 0. \n");
        return;
    }
    --reference_count;

    if (reference_count == 0) {
#if defined(APRIL3D_DEBUG) && (APRIL3D_DEBUG > 0)
        auto poolManager = PoolManager::getInstance();
        if (!poolManager->getCurrentPool()->isClearing() && poolManager->isObjectInPools(this)) {
            printf("The reference shouldn't be 0 because it is still in autorelease pool. \n");
        }
#endif

#if REF_LEAK_DETECTION
        untrackRef(this);
#endif
        delete this;
    }
}

Ref* Ref::autorelease() {
    PoolManager::getInstance()->getCurrentPool()->addObject(this);
    return this;
}

unsigned int Ref::getReferenceCount() const {
    return reference_count;
}

#if REF_LEAK_DETECTION

static std::vector<Ref*> ref_allocation_list;
static std::mutex ref_mutex;

void Ref::printLeaks() {
    std::lock_guard<std::mutex> refLockGuard(ref_mutex);
    // Dump Ref object memory leaks
    if (ref_allocation_list.empty()) {
        printf("[memory] All Ref objects successfully cleaned up (no leaks detected).\n");
    } else {
        printf("[memory] WARNING: %d Ref objects still active in memory.\n", (int)ref_allocation_list.size());
        for (const auto& ref : ref_allocation_list) {
            const char* type = typeid(*ref).name();
            printf("[memory] LEAK: Ref object '%s' still active with reference count %d.\n", (type ? type : ""), ref->getReferenceCount());
        }
    }
}

static void trackRef(Ref* ref) {
    std::lock_guard<std::mutex> refLockGuard(ref_mutex);
    if(!ref) {
        printf("trackRef Invalid parameter, ref should not be null! \n");
        return;
    }
    ref_allocation_list.push_back(ref);
}

static void untrackRef(Ref* ref) {
    std::lock_guard<std::mutex> refLockGuard(ref_mutex);
    auto iter = std::find(ref_allocation_list.begin(), ref_allocation_list.end(), ref);
    if (iter == ref_allocation_list.end()) {
        printf("[memory] CORRUPTION: Attempting to free (%s) with invalid ref tracking record.\n", typeid(*ref).name());
        return;
    }
    ref_allocation_list.erase(iter);
}
#endif

APRIL_END
