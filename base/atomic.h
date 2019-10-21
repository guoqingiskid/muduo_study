//
// Created by root on 19-10-6.
//

#ifndef REPOS_MY_MUDUO_ATOMIC_H
#define REPOS_MY_MUDUO_ATOMIC_H

#include <stdint.h>
namespace my_muduo
{
    namespace detail
    {
        /* only int type */
        template<typename T>
        class AtomicIntegerT
        {
        public:
            AtomicIntegerT(const AtomicIntegerT &other) = delete;
            AtomicIntegerT& operator=(const AtomicIntegerT &other) = delete;

            AtomicIntegerT(): _value(0){}
            T get()
            {
                return __sync_val_compare_and_swap(&_value, 0, 0); //if _value == 0
            }

            T getAndAdd(T val)
            {
                return __sync_fetch_and_add(&_value, val);
            }

            T addAndGet(T val)
            {
                return getAndAdd(val) + val;
            }

            T incrementAndGet()
            {
                return addAndGet(1);
            }

            void add(T val)
            {
                getAndAdd(val);
            }

            void increment()
            {
                incrementAndGet();
            }

            T decrementAndGet()
            {
                return addAndGet(-1);
            }

            void decrement()
            {
                decrementAndGet();
            }

            T getAndSet(T newValue)
            {
                return __sync_lock_test_and_set(&_value, newValue);
            }

            T operator++()
            {
                return incrementAndGet();
            }

            T operator++(int)
            {
                return getAndAdd(1);
            }

            T operator--()
            {
                return decrementAndGet();
            }

            T operator--(int)
            {
                return getAndAdd(-1);
            }

        private:
            volatile T _value;
        };
    }

    typedef detail::AtomicIntegerT<int32_t > AtomicInt32;
    typedef detail::AtomicIntegerT<int64_t > AtomicInt64;
}


#endif //REPOS_MY_MUDUO_ATOMIC_H
