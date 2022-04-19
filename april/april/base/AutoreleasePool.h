//
//  AutoReleasePool.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/5.
//

#ifndef AutoReleasePool_hpp
#define AutoReleasePool_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include "base/Ref.h"

APRIL_BEGIN

/**
 *自动释放池
 */
 
class AutoreleasePool {
public:
    AutoreleasePool();

    AutoreleasePool(const std::string &name);
    
    ~AutoreleasePool();

    void addObject(Ref *object);

    void clear();
    
#if defined(APRIL3D_DEBUG) && (APRIL3D_DEBUG > 0)
    bool isClearing() const { return is_clearing; };
#endif
    
    bool contains(Ref* object) const;
    
    void dump();
    
private:
    /**
     *添加到当前自动释放池的对象数组
     */
    std::vector<Ref*> managed_object_array;
    std::string name_;
    
#if defined(APRIL_DEBUG) && (APRIL_DEBUG > 0)
    bool is_clearing;
#endif
};

class PoolManager {
public:
    static PoolManager* getInstance();
    static void destroyInstance();
    
    AutoreleasePool* getCurrentPool() const;

    bool isObjectInPools(Ref* obj) const;

    friend class AutoreleasePool;
    
private:
    PoolManager();
    ~PoolManager();
    
    void push(AutoreleasePool *pool);
    void pop();
    
    static PoolManager* s_singleInstance;
    
    std::vector<AutoreleasePool*> release_pool_stack;
};

APRIL_END

#endif /* AutoReleasePool_hpp */
