//
// Created by root on 19-10-20.
//

#ifndef REPOS_MY_MUDUO_THREAD_H
#define REPOS_MY_MUDUO_THREAD_H

#include "current_thread.h"
#include "mutex.h"
#include "condition.h"
#include <pthread.h>
#include <functional>
#include <string>
#include "count_down_latch.h"
#include "atomic.h"

namespace my_muduo
{
    class Thread
    {
    public:
        Thread(const Thread &other) = delete;
        Thread& operator =(const Thread &other) = delete;
        typedef std::function<void(void)> ThreadFunc;                       //if using args, must be std::bind args
        explicit Thread(ThreadFunc func, const std::string &name = "");
        ~Thread();
        const std::string& getName()const {return _name;}
        bool isStarted()const {return _started;}
        pthread_t pid()const {return _pthreadId;}
        int tid()const {return _tid;}
        int32_t getThreadCreatedCount()const {return _nCreated.get();}
        bool startThread();
        void join();

    private:
        void init();


    private:
        std::string         _name;
        ThreadFunc          _func;
        bool                _started;
        bool                _joined;
        pthread_t           _pthreadId;
        int                 _tid;
        CountDownLatch      _latch;
        static AtomicInt32  _nCreated;
    };
}


#endif //REPOS_MY_MUDUO_THREAD_H
