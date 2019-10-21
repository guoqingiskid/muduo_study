//
// Created by root on 19-10-21.
//

#ifndef REPOS_MY_MUDUO_WEAK_CALLBACK_H
#define REPOS_MY_MUDUO_WEAK_CALLBACK_H

#include <memory>
#include <functional>

template<typename CLASS, typename ...Args>
class WeakCallBack
{
public:
    WeakCallBack(const std::weak_ptr<CLASS> &weakPtr, const std::function<void(CLASS*, Args...)> &func) :
        _weakPtr(weakPtr),
        _func(func)
    {}

    void operator()(Args&&... args) const
    {
        auto ptr = _weakPtr.lock();
        if(ptr)
        {
            _func(ptr.get(), std::forward<Args>(args)...);
        }
    }

private:
    std::weak_ptr<CLASS> _weakPtr;
    std::function<void(CLASS*, Args...)> _func;
};

template<typename CLASS, typename ...Args>
WeakCallBack<CLASS, Args...> MakeWeakCallBack(const std::shared_ptr<CLASS> &ptr, void (CLASS::*func)(Args...))
{
    return WeakCallBack<CLASS, Args...>(ptr, func);
}
#endif //REPOS_MY_MUDUO_WEAK_CALLBACK_H
