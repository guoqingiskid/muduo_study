//
// Created by root on 19-10-8.
//

#ifndef REPOS_MY_MUDUO_MUTEX_H
#define REPOS_MY_MUDUO_MUTEX_H

#include "macro.h"
#include "current_thread.h"
#include <pthread.h>

namespace my_muduo
{
    class Condition;
    class MutexLock
    {
    public:
        MutexLock(const MutexLock &other) = delete;
        MutexLock&operator=(const MutexLock &other) = delete;

        pthread_mutex_t& getPosixMutex()
        {
            return _mutex;
        }

        MutexLock() : _holder(0)
        {
            PTHREAD_RETURE_MCHECK(pthread_mutex_init(&_mutex, nullptr));
        }

        ~MutexLock()
        {
            PTHREAD_RETURE_MCHECK(pthread_mutex_destroy(&_mutex));
        }

        void lock()
        {
            PTHREAD_RETURE_MCHECK(pthread_mutex_lock(&_mutex));
            setHolder(current_thread::tid());
        }

        void unlock()
        {
            PTHREAD_RETURE_MCHECK(pthread_mutex_unlock(&_mutex));
            setHolder(0);
        }

        bool isLockedByThisThread() const
        {
            return _holder == current_thread::tid();
        }

        friend class Condition;
        class CondWaitGuard
        {
        public:
            CondWaitGuard(MutexLock &mutex) : _mutex(mutex)
            {
                _mutex.setHolder(0);
            }

            ~CondWaitGuard()
            {
                _mutex.setHolder(current_thread::tid());
            }


        private:
            MutexLock& _mutex;
        };

    private:
        void setHolder(int32_t tid)
        {
            _holder = tid;
        }

    private:
        int32_t _holder;
        pthread_mutex_t _mutex;
    };

    class LockGuard
    {
    public:
        LockGuard(MutexLock &mutex) : _mutex(mutex)
        {
            _mutex.lock();
        }

        ~LockGuard()
        {
            _mutex.unlock();
        }

    private:
        MutexLock& _mutex;
    };
}

#endif //REPOS_MY_MUDUO_MUTEX_H
