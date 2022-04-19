//
//  AutoreleasePool.cpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/6.
//

#include "base/AutoreleasePool.h"
#include "base/aprilMacros.h"

APRIL_BEGIN

AutoreleasePool::AutoreleasePool()
: name_("")
#if defined(APRIL3D_DEBUG) && (APRIL3D_DEBUG > 0)
, is_cleaning(false)
#endif
{
    managed_object_array.reserve(150);
    PoolManager::getInstance()->push(this);
}

AutoreleasePool::AutoreleasePool(const std::string &name)
: name_(name)
#if defined(APRIL3D_DEBUG) && (APRIL3D_DEBUG > 0)
, is_cleaning(false)
#endif
{
    managed_object_array.reserve(150);
    PoolManager::getInstance()->push(this);
}

AutoreleasePool::~AutoreleasePool() {
    printf("deallocing AutoreleasePool: %p \n", this);
    clear();
    
    PoolManager::getInstance()->pop();
}

void AutoreleasePool::addObject(Ref* object)
{
    managed_object_array.push_back(object);
}

void AutoreleasePool::clear()
{
#if defined(APRIL3D_DEBUG) && (APRIL3D_DEBUG > 0)
    is_cleaning = true;
#endif
    std::vector<Ref*> releasings;
    releasings.swap(managed_object_array);
    for (const auto &obj : releasings)
    {
        obj->release();
    }
#if defined(APRIL3D_DEBUG) && (APRIL3D_DEBUG > 0)
    is_cleaning = false;
#endif
}

bool AutoreleasePool::contains(Ref* object) const {
    for (const auto& obj : managed_object_array) {
        if (obj == object)
            return true;
    }
    return false;
}

void AutoreleasePool::dump() {
    printf("autorelease pool: %s, number of managed object %d\n", name_.c_str(), static_cast<int>(managed_object_array.size()));
    printf("%20s%20s%20s", "Object pointer", "Object id", "reference count");
    for (const auto &obj : managed_object_array) {
        printf("%20p%20u\n", obj, obj->getReferenceCount());
    }
}


PoolManager* PoolManager::s_singleInstance = nullptr;

PoolManager* PoolManager::getInstance()
{
    if (s_singleInstance == nullptr)
    {
        s_singleInstance = new (std::nothrow) PoolManager();
        // Add the first auto release pool
        new AutoreleasePool("cocos2d autorelease pool");
    }
    return s_singleInstance;
}

void PoolManager::destroyInstance()
{
    delete s_singleInstance;
    s_singleInstance = nullptr;
}

PoolManager::PoolManager() {
    release_pool_stack.reserve(10);
}

PoolManager::~PoolManager() {
    printf("deallocing PoolManager: %p \n", this);
    while (!release_pool_stack.empty()) {
        AutoreleasePool* pool = release_pool_stack.back();
        delete pool;
    }
}


AutoreleasePool* PoolManager::getCurrentPool() const
{
    return release_pool_stack.back();
}

bool PoolManager::isObjectInPools(Ref* obj) const
{
    for (const auto& pool : release_pool_stack)
    {
        if (pool->contains(obj))
            return true;
    }
    return false;
}

void PoolManager::push(AutoreleasePool *pool)
{
    release_pool_stack.push_back(pool);
}

void PoolManager::pop() {
    if(release_pool_stack.empty()) {
        printf("Error: PoolManager is empty. \n");
        return;
    }
    release_pool_stack.pop_back();
}

APRIL_END
