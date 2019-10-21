//
// Created by root on 19-10-20.
//

#include "thread_pool.h"
#include <iostream>

namespace my_muduo
{
    ThreadPool::ThreadPool(const std::string &name):
        _mutex(),
        _notEmpty(_mutex),
        _notFull(_mutex),
        _maxThreadQueueSize(0),
        _running(false)
    {

    }

    ThreadPool::~ThreadPool()
    {
        if(_running)
            this->stop();
    }

    void ThreadPool::start(int numThreads)
    {
        assert(_threadQueue.empty());
        _running = true;
        _threadQueue.reserve(numThreads);
        if(_maxThreadQueueSize == 0)
            _maxThreadQueueSize = numThreads;

        for(int idx = 0; idx < numThreads; ++idx)
        {
            char id[32] = {0};
            sprintf(id, "%d", idx + 1);
            _threadQueue.emplace_back(new Thread(std::bind(&ThreadPool::runInThreadPool, this), _name + std::string(id)));
            _threadQueue[idx]->startThread();
        }
    }

    ThreadPool::Task ThreadPool::takeTask()
    {
        LockGuard lk(_mutex);
        Task task;
        while(_running && _taskQueue.empty())
        {
            _notEmpty.wait();
        }
        if(!_taskQueue.empty())
        {
            task = std::move(_taskQueue.front());
            _taskQueue.pop_front();
            if(_maxThreadQueueSize > 0)
                _notFull.notify();
        }

        return task;
    }

    void ThreadPool::runInThreadPool()
    {
        std::cout << "this thread id: " << current_thread::tid() << std::endl;
        while(_running)
        {
            Task task(takeTask());
            if(task)
                task();
        }
    }

    bool ThreadPool::isFull()
    {
        return _taskQueue.size() >= _maxThreadQueueSize;
    }

    void ThreadPool::run(Task task)
    {
        if(_threadQueue.empty())
            task();
        else
        {
            LockGuard lk(_mutex);
            while(isFull())
            {
                _notFull.wait();
            }

            _taskQueue.push_back(std::move(task));
            _notEmpty.notify();

        }
    }

    void ThreadPool::stop()
    {
        LockGuard lk(_mutex);
        _running = false;
        _notEmpty.notifyAll();
        for(int idx = 0; idx < _threadQueue.size(); ++idx)
            _threadQueue[idx]->join();
    }

}