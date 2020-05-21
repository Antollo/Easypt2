#ifndef PROMISE_H_
#define PROMISE_H_

#include <future>
#include <thread>
#include <chrono>
#include <functional>
#include <list>
#include <stdexcept>
#include <memory>
#include <atomic>
#include "console.h"

template <class R>
class promise : public std::enable_shared_from_this<promise<R>>
{
public:
    using promisePtr = std::shared_ptr<promise>;
    enum class executionPolicy
    {
        asynchronous,
        synchronous
    };

    promise(promise &&) = default;

    template <class F>
    static std::shared_ptr<promise> makePromise(F &&callback, executionPolicy ep = executionPolicy::synchronous)
    {
        auto pr = std::shared_ptr<promise>(new promise(std::forward<F>(callback), ep));
        promises.push_back(pr);
        return pr;
    }

    template <class F, class C>
    static std::shared_ptr<promise> makePromise(F &&callback, executionPolicy ep , C &&condition)
    {
        auto pr = std::shared_ptr<promise>(new promise(std::forward<F>(callback), ep, std::forward<C>(condition)));
        promises.push_back(pr);
        return pr;
    }

    ~promise()
    {
        while (!started)
        {
            tick();
            loop();
        }
        while (!isReady())
            loop();
        if (consumable)
        {
            while (!consumed)
                loop();
        }
        if (thread.joinable())
            thread.join();
    }

    std::shared_ptr<promise> then(std::function<R(R)> &&callback, executionPolicy policy = executionPolicy::synchronous)
    {
        consumable = true;
        if (cbf)
            throw std::runtime_error("callback already registered");
        cbf = callback;
        return makePromise([this]() {
                            consumed = true;
                            R temp = future.get();
                            return cbf(temp); },
                           policy,
                           [this]() {
                               return isReady();
                           });
    }
    R await()
    {
        consumable = true;
        if (cbf)
            throw std::runtime_error("cannot await promise, it will be consumed by callback");
        consumed = true;
        while (!isReady())
        {
            loop();
        }
        R temp = future.get();
        return temp;
    }
    static void loop()
    {
        console::debug("loop");
        typename list::iterator i, j;
        for (i = promises.begin(); i != promises.end();)
        {
            j = i++;
            (*j)->tick();
            if ((*j)->isReady())
            {
                console::debug("consumable: ", (*j)->consumable, " consumed: ", (*j)->consumed);
                if ((*j)->consumable && (*j)->consumed)
                {
                    console::debug("released");
                    promises.erase(j);
                }
                else if (!(*j)->consumable && j->use_count() == 1)
                {
                    console::debug("released !consumable");
                    promises.erase(j);
                }
            }
        }
    }
    static bool loopEmpty()
    {
        return promises.empty();
    }

private:
    template <class F>
    promise(F &&callback, executionPolicy policy, std::function<bool(void)> &&condition)
        : pt(std::packaged_task<R(void)>(std::forward<F>(callback))), c(condition),
          started(false), ended(false), consumable(false), consumed(false),
          ep(policy), future(pt.get_future()) {}

    template <class F>
    promise(F &&callback, executionPolicy policy)
        : promise(std::forward<F>(callback), policy, std::function<bool(void)>([]() { return true; })) {}

    void tick()
    {
        if (!started && c())
        {
            started = true;
            if (ep == executionPolicy::asynchronous)
            {
                thread = std::thread([this]() {
                    pt();
                    //ended = true;
                });
            }
            else if (ep == executionPolicy::synchronous)
            {
                pt();
                //ended = true;
            }
        }
    }
    bool isReady()
    {
        if (ended) return true;
        return ended = future.valid() && future.wait_for(std::chrono::milliseconds::zero()) == std::future_status::ready;
    }
    using list = typename std::list<std::shared_ptr<promise>>;
    static list promises;

    std::packaged_task<R(void)> pt;
    std::function<R(R)> cbf;
    std::function<bool(void)> c;
    std::future<R> future;
    bool started, ended, consumable;
    std::atomic<bool> consumed;
    executionPolicy ep;
    std::thread thread;
};

template <class R>
typename promise<R>::list promise<R>::promises;

#endif /* !PROMISE_H_ */
