//
// Created by root on 19-10-9.
//

#ifndef REPOS_MY_MUDUO_CONDITION_H
#define REPOS_MY_MUDUO_CONDITION_H

#include "mutex.h"
#include <pthread.h>
#include <string.h>
#include <errno.h>

namespace my_muduo
{
    class Condition
    {
    public:
        Condition(const Condition &other) = delete;

        Condition &operator=(const Condition &other) = delete;

        Condition(MutexLock &mutex) : _mutex(mutex)
        {
            PTHREAD_RETURE_MCHECK(pthread_cond_init(&_cond, nullptr));
        }

        void notify()
        {
            PTHREAD_RETURE_MCHECK(pthread_cond_signal(&_cond));
        }

        void notifyAll()
        {
            PTHREAD_RETURE_MCHECK(pthread_cond_broadcast(&_cond));
        }

        void wait()
        {
            MutexLock::CondWaitGuard gurad(_mutex);
            PTHREAD_RETURE_MCHECK(pthread_cond_wait(&_cond, &_mutex.getPosixMutex()));
        }

        bool waitForSeconds(int64_t seconds)
        {
            struct timespec abstime;
            memset(&abstime, 0, sizeof(abstime));
            clock_gettime(CLOCK_REALTIME, &abstime);
            const int64_t kNanoSecondsPerSeconds = 1000 * 1000 * 1000;
            int64_t nanosec = seconds  * kNanoSecondsPerSeconds;
            abstime.tv_sec += (nanosec + abstime.tv_nsec) / kNanoSecondsPerSeconds;
            abstime.tv_nsec += (nanosec + abstime.tv_nsec) % kNanoSecondsPerSeconds;
            return ETIMEDOUT == pthread_cond_timedwait(&_cond, &_mutex.getPosixMutex(), &abstime);

        }

        MutexLock& getMutexLock()
        {
            return _mutex;
        }

    private:
        MutexLock& _mutex;
        pthread_cond_t _cond;
    };

}

#endif //REPOS_MY_MUDUO_CONDITION_H
