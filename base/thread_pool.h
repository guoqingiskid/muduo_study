//
// Created by root on 19-10-20.
//

#ifndef REPOS_MY_MUDUO_THREAD_POOL_H
#define REPOS_MY_MUDUO_THREAD_POOL_H

#include "mutex.h"
#include "condition.h"
#include "thread.h"
#include <vector>
#include <memory>
#include <deque>

namespace my_muduo
{
    class ThreadPool
    {
    public:
        typedef std::function<void(void)> Task;
        ThreadPool(const ThreadPool &other) = delete;
        ThreadPool& operator=(const ThreadPool &other) = delete;

        explicit ThreadPool(const std::string &name = "ThreadPool");
        void setMaxQueueSize(size_t size){_maxThreadQueueSize = size;}
        void start(int numThreads = 0);     //start this thread pool
        void stop();                        //stop this thread pool
        const std::string& getName()const {return _name;}
        size_t getTaskSize()const;
        void run(Task task);                //run this task, this will push back to queue!!!
        void runInThreadPool();             //all threds in pool will exec this function, and to exec task by taking take of queue
        ~ThreadPool();

    private:
        Task takeTask();
        bool isFull();
    private:
        std::string _name;
        std::vector<std::unique_ptr<Thread> > _threadQueue;
        std::deque<Task>    _taskQueue;
        MutexLock           _mutex;
        Condition           _notEmpty;
        Condition           _notFull;
        size_t              _maxThreadQueueSize;
        bool                _running;
    };
}


#endif //REPOS_MY_MUDUO_THREAD_POOL_H
