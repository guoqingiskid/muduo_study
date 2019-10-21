//
// Created by root on 19-10-13.
//

#ifndef REPOS_MY_MUDUO_COUNT_DOWN_LATCH_H
#define REPOS_MY_MUDUO_COUNT_DOWN_LATCH_H

#include "mutex.h"
#include "condition.h"
namespace my_muduo
{
    class CountDownLatch
    {
    public:
        CountDownLatch(const CountDownLatch &other) = delete;
        CountDownLatch& operator=(const CountDownLatch &other) = delete;
        CountDownLatch(int count): _count(count), _cond(_mutex){}

        void wait()
        {
            LockGuard lk(_mutex);
            while(_count > 0)
                _cond.wait();
        }

        void countDown()
        {
            LockGuard lk(_mutex);
            --_count;
            if(_count == 0)
                _cond.notify();
        }

        int count()const
        {
            LockGuard lk(_mutex);
            return _count;
        }

    private:
        mutable MutexLock _mutex;
        Condition _cond;
        int _count;
    };
}


#endif //REPOS_MY_MUDUO_COUNT_DOWN_LATCH_H
