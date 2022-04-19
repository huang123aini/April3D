//
//  RenderCommandPool.h
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/4.
//

#ifndef RenderCommandPool_h
#define RenderCommandPool_h

#include <list>

#include "platform/AprilPlatformMacros.h"

APRIL_BEGIN

template <class T>
class RenderCommandPool {
public:
    RenderCommandPool() {}
    ~RenderCommandPool() {

        free_pool.clear();
        for (auto& allocatedPoolBlock : allocated_pool_blocks) {
            delete[] allocatedPoolBlock;
            allocatedPoolBlock = nullptr;
        }
        allocated_pool_blocks.clear();
    }

    T* generateCommand() {
        T* result = nullptr;
        if(free_pool.empty()) {
            AllocateCommands();
        }
        result = free_pool.front();
        free_pool.pop_front();
        return result;
    }
    
    void pushBackCommand(T* ptr) {
        free_pool.push_back(ptr);
    }
private:
    void AllocateCommands() {
        static const int COMMANDS_ALLOCATE_BLOCK_SIZE = 32;
        T* commands = new (std::nothrow) T[COMMANDS_ALLOCATE_BLOCK_SIZE];
        allocated_pool_blocks.push_back(commands);
        for(int index = 0; index < COMMANDS_ALLOCATE_BLOCK_SIZE; ++index) {
            free_pool.push_back(commands+index);
        }
    }

    std::list<T*> allocated_pool_blocks;
    std::list<T*> free_pool;
};

APRIL_END

#endif /* RenderCommandPool_h */
