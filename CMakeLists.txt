cmake_minimum_required(VERSION 3.6)
project(repos_my_muduo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp base/blocking_queue.h base/current_thread.h base/current_thread.cpp base/atomic.h base/mutex.h base/macro.h base/condition.h base/count_down_latch.h base/date.h base/date.cpp base/thread.h base/thread.cpp base/thread_pool.h base/thread_pool.cpp base/weak_callback.h)
link_libraries(pthread)
add_executable(repos_my_muduo ${SOURCE_FILES})
