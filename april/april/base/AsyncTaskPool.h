//
//  AsyncTaskPool.hpp
//  april3d_libs
//
//  Created by 黄世平 on 2022/4/5.
//

#ifndef AsyncTaskPool_hpp
#define AsyncTaskPool_hpp

#include <stdio.h>
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#include "platform/AprilPlatformMacros.h"

APRIL_BEGIN

class AsyncTaskPool {
public:
    typedef std::function<void(void*)> TaskCallBack;
    
    enum class TaskType {
        TASK_IO,
        TASK_NETWORK,
        TASK_OTHER,
        TASK_MAX_TYPE,
    };

    static AsyncTaskPool* getInstance();

    static void destroyInstance();
    
    void stopTasks(TaskType type);
    
    void enqueue(TaskType type, TaskCallBack callback, void* callbackParam, std::function<void()> task);

    void enqueue(AsyncTaskPool::TaskType type, std::function<void()> task);
    
    AsyncTaskPool();
    ~AsyncTaskPool();
    
protected:
    
    class ThreadTasks {
        struct AsyncTaskCallBack {
            TaskCallBack          callback;
            void*                 callbackParam;
        };
    public:
        ThreadTasks()
        : stop_(false) {
            thread_ = std::thread(
                                  [this] {
                                      for(;;) {
                                          std::function<void()> task;
                                          AsyncTaskCallBack callback;
                                          {
                                              std::unique_lock<std::mutex> lock(this->_queueMutex);
                                              this->condition_.wait(lock,
                                                                    [this]{ return this->stop_ || !this->tasks_.empty(); });
                                              if(this->stop_ && this->tasks_.empty()) {
                                                  return;
                                              }
                                              task = std::move(this->tasks_.front());
                                              callback = std::move(this->task_callbacks.front());
                                              this->tasks_.pop();
                                              this->task_callbacks.pop();
                                          }
                                          
                                          /**
                                           *执行当前任务
                                           */
                                          task();
                                          
                                          /**
                                           *回调到任务线程
                                           */
//                                          Director::getInstance()->getScheduler()->performFunctionInCocosThread(std::bind(callback.callback, callback.callbackParam));
                                      }
                                  }
                                  );
        }
        ~ThreadTasks() {
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                stop_ = true;
                
                while(tasks_.size()) {
                    tasks_.pop();
                }
                while (task_callbacks.size()) {
                    task_call_backs.pop();
                }
            }
            condition_.notify_all();
            thread_.join();
        }
        void clear() {
            std::unique_lock<std::mutex> lock(queue_mutex);
            while(tasks_.size()) {
                tasks_.pop();
            }
            while (task_callbacks.size()) {
                task_callbacks.pop();
            }
        }

        void enqueue(TaskCallBack callback, void* callbackParam, std::function<void()> task) {
            AsyncTaskCallBack taskCallBack;
            taskCallBack.callback = std::move(callback);
            taskCallBack.callbackParam = callbackParam;

            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                if(stop_){
                    return;
                }
                tasks_.push(std::move(task));
                task_callbacks.push(std::move(taskCallBack));
            }
            _condition.notify_one();
        }
    private:
        std::thread thread_;
        std::queue< std::function<void()> > tasks_;
        std::queue<AsyncTaskCallBack> task_callbacks;
        
        std::mutex queue_mutex;
        std::condition_variable condition_;
        bool stop_;
    };
    
    ThreadTasks thread_tasks[int(TaskType::TASK_MAX_TYPE)];
    
    static AsyncTaskPool* s_asyncTaskPool;
};

inline void AsyncTaskPool::stopTasks(TaskType type) {
    auto& threadTask = thread_tasks[(int)type];
    threadTask.clear();
}

inline void AsyncTaskPool::enqueue(AsyncTaskPool::TaskType type, TaskCallBack callback, void* callbackParam, std::function<void()> task) {
    auto& threadTask = thread_tasks[(int)type];
    threadTask.enqueue(std::move(callback), callbackParam, std::move(task));
}

inline void AsyncTaskPool::enqueue(AsyncTaskPool::TaskType type, std::function<void()> task) {
    enqueue(type, [](void*) {}, nullptr, std::move(task));
}

APRIL_END

#endif /* AsyncTaskPool_hpp */
