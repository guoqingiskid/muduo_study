//
// Created by root on 19-10-6.
//

#ifndef REPOS_MY_MUDUO_BLOCKING_QUEUE_H
#define REPOS_MY_MUDUO_BLOCKING_QUEUE_H

#include "mutex.h"
#include "condition.h"
#include "deque"

namespace my_muduo
{
    template<typename T>
    class BlockingQueue
    {
    public:
        BlockingQueue(const BlockingQueue &other) = delete;
        BlockingQueue& operator=(const BlockingQueue &other) = delete;

        BlockingQueue() : _notEmpty(_mutex){}
        void put(const T &ele)
        {
            LockGuard lk(_mutex);
            _queue.push_back(ele);
            _notEmpty.notify();
        }

        void put(T &&ele)
        {
            LockGuard lk(_mutex);
            _queue.push_back(std::move(ele));
            _notEmpty.notify();
        }

        T take()
        {
            LockGuard lk(_mutex);
            while(_queue.empty())
                _notEmpty.wait();
            T res(std::move(_queue.front()));
            _queue.pop_front();
            return std::move(res);
        }

        size_t size()const
        {
            return _queue.size();
        }

    private:
        std::deque<T> _queue;
        MutexLock  _mutex;
        Condition _notEmpty;
    };
}



#endif //REPOS_MY_MUDUO_BLOCKING_QUEUE_H
