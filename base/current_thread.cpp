//
// Created by root on 19-10-6.
//
#include "current_thread.h"
#include <syscall.h>
#include <unistd.h>
#include <cstdio>
#include <string.h>

using namespace my_muduo;
__thread int32_t current_thread::t_cacheTid = 0;
__thread pthread_t current_thread::t_threadId = 0;
__thread char current_thread::t_tidString[32];
__thread const char* current_thread::t_threadName = nullptr;
__thread int32_t current_thread::t_tidStringLen = 0;

const int global::kMicroSecondPerSecond = 1000 * 1000;

void current_thread::detail::cacheId()
{
    t_cacheTid = syscall(SYS_gettid);
    std::snprintf(t_tidString, sizeof(t_tidString), "%5d", t_cacheTid);
    t_tidStringLen = strlen(t_tidString);
}

bool current_thread::detail::isMainThread()
{
    return ::getpid() == t_cacheTid;
}

/* if intrreput by signal, no handler */
void current_thread::detail::sleepUsec(int64_t usec)
{
    using namespace my_muduo::global;
    struct timespec ts{0, 0};
    ts.tv_sec = usec / kMicroSecondPerSecond;
    ts.tv_nsec = (usec % kMicroSecondPerSecond) * 1000;
    ::nanosleep(&ts, nullptr);
}
