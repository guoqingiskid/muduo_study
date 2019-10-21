//
// Created by root on 19-10-6.
//

#ifndef REPOS_MY_MUDUO_CURRENT_THREAD_H
#define REPOS_MY_MUDUO_CURRENT_THREAD_H

#include <stdint.h>
#include <zconf.h>
#include <pthread.h>

namespace my_muduo
{
    namespace global
    {
        extern const int kMicroSecondPerSecond;
    }

    namespace current_thread
    {
        extern __thread int32_t     t_cacheTid;
        extern __thread char        t_tidString[32];
        extern __thread int32_t     t_tidStringLen;
        extern __thread const char* t_threadName;
        extern __thread pthread_t   t_threadId;

        namespace detail
        {
            void cacheId();
            bool isMainThread();
            void sleepUsec(int64_t usec);
        }

        /* inline: function can definition there*/
        inline int32_t tid()
        {
            if(t_cacheTid == 0)
            {
                detail::cacheId();
            }

            return t_cacheTid;
        }

        inline int32_t tidStringLength()
        {
            return t_tidStringLen;
        }

        inline pthread_t threadId()
        {
            if(pthread_equal(t_threadId, 0))
            {
                t_threadId = pthread_self();
            }

            return t_threadId;
        }

        inline const char* threadName()
        {
            return t_threadName;
        }

        inline const char* tidString()
        {
            return t_tidString;
        }
    }
}

#endif //REPOS_MY_MUDUO_CURRENT_THREAD_H
