//
// Created by root on 19-10-20.
//
#include "thread.h"
#include <iostream>

namespace my_muduo
{
    AtomicInt32 Thread::_nCreated;
    Thread::Thread(ThreadFunc func, const std::string &name) :
        _name(name),
        _func(std::move(func)),
        _started(false),
        _joined(false),
        _tid(tid()),
        _latch(1)
    {
        init();
    }

    void Thread::init()
    {
        int32_t num = _nCreated.incrementAndGet();
        if(_name.empty())
        {
            char name[32] = {0};
            sprintf(name, "Thread_%d", num);
            _name.assign(name);
        }
    }

    void Thread::join()
    {
        if(_started && _joined)
        {
            pthread_join(_pthreadId, nullptr);
        }
    }

    Thread::~Thread()
    {
        if(_started && _joined)
        {
            pthread_detach(_pthreadId);
        }
    }

    namespace detail
    {
        struct ThreadData
        {
            typedef typename Thread::ThreadFunc ThreadFunc;
            ThreadFunc          _func;
            int32_t             _tid;
            std::string         _name;
            CountDownLatch*     _latch;
            ThreadData(ThreadFunc func, int32_t tid, const std::string &name, CountDownLatch *latch) :
                _func(std::move(func)),
                _tid(tid),
                _name(name),
                _latch(latch)
            {

            }

            void runThread()
            {
                _latch->countDown();
                current_thread::t_threadName = _name.empty() ? "myThread" : _name.c_str();
                try
                {
                    _func();
                }
                catch(const std::exception &ex)
                {
                    current_thread::t_threadName = "crashed";
                    abort();
                }
            }

        };

        static void *startThread(void *arg)
        {
            ThreadData *pThreadInfo = static_cast<ThreadData*>(arg);
            pThreadInfo->runThread();
            delete pThreadInfo;
        }
    }

    bool Thread::startThread()
    {
        if(_started)
            return false;
        _started = true;

        //due to this func can't be using by pthread, so we must use other method!!!
        detail::ThreadData *pThreadInfo = new detail::ThreadData(_func, _tid, _name, &_latch);
        if(pthread_create(&_pthreadId, nullptr, detail::startThread, (void*)pThreadInfo))
        {
            _started = false;
            delete pThreadInfo;
            return false;
        }
        else
        {
            _latch.wait();
            return true;
        }
    }

}
