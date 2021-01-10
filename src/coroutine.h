#ifndef COROUTINE_H_
#define COROUTINE_H_

#include <functional>
#include <future>
#include <chrono>
#include <memory>
#include <array>
#include <list>
#include <stdexcept>
#include <type_traits>
#include <random>
#include <iterator>
#include <system_error>

#include "console.h"
#include "objectPtrImpl.h"

// Common interface for fiber api and ucontext api

constexpr std::size_t coroutineStackSize = 1048576;

#ifdef _WIN32

#include <windows.h>
#define initContext nullptr
#define initCurrentContext(c, m, s)                                                               \
    {                                                                                             \
        if (!m)                                                                                   \
        {                                                                                         \
            m = ConvertThreadToFiber(nullptr);                                                    \
        }                                                                                         \
        c = CreateFiberEx(coroutineStackSize / 4, coroutineStackSize, 0, &coroutine::call, this); \
        if (c == nullptr)                                                                         \
        {                                                                                         \
            auto code = GetLastError();                                                           \
            console::error("coroutine creation error ", code);                                    \
            throw std::runtime_error("coroutine creation error");                                 \
        }                                                                                         \
    }
#define swapContext(a, b) SwitchToFiber(b)
#define deleteContext(c) \
    if (c)               \
    DeleteFiber(c)
#define API WINAPI
using contextType = void *;
struct emptyType
{
};
using stackType = emptyType;

#else

#include <ucontext.h>
#define initContext \
    {               \
    }
#define initCurrentContext(c, m, s)                                               \
    {                                                                             \
        getcontext(&c);                                                           \
        c.uc_stack.ss_sp = s.data();                                              \
        c.uc_stack.ss_size = s.size();                                            \
        c.uc_link = &m;                                                           \
        makecontext(&c, reinterpret_cast<void (*)()>(&coroutine::call), 1, this); \
    }
#define swapContext(a, b) swapcontext(&a, &b)
#define deleteContext(c)
#define API
using contextType = ucontext_t;
using stackType = std::array<char, coroutineStackSize>;

#endif

// Common for all coroutines

class coroutineBase
{

public:
    // Check if coroutine finished
    virtual bool running() = 0;
    // Step into coroutine
    virtual void step() = 0;
    virtual ~coroutineBase() {}
    // Process all coroutines
    static bool stepAll()
    {
        //console::debug("Main: ", GetCurrentFiber(), " ", int(coYield.target<void (*)()>()));

        if (coroutines.empty())
            return false;

        auto j = coroutines.begin();
        for (auto i = coroutines.begin(); i != coroutines.end();)
        {
            j = i++;
            if (!(*j)->ready || (*j)->ready())
            {
                if (!(*j)->running())
                    coroutines.erase(j);
                else
                    (*j)->step();
            }
        }

        std::this_thread::yield();

        //console::debug("Back in main: ", GetCurrentFiber());
        //console::warn("End");

        return true;
    }
    static void handleException(std::exception_ptr ptr);
    static std::function<void()> coYield;

    template <class T>
    static T coAwait(std::future<T> &&f)
    {
        if (f.valid() && f.wait_for(std::chrono::milliseconds::zero()) == std::future_status::ready)
            return f.get();
        if (currentCoroutine)
            currentCoroutine->ready = [&f]() {
                return f.valid() && f.wait_for(std::chrono::milliseconds::zero()) == std::future_status::ready;
            };
        coYield();
        while (!f.valid() || f.wait_for(std::chrono::milliseconds::zero()) != std::future_status::ready)
            coYield();
        if (currentCoroutine)
            currentCoroutine->ready = nullptr;
        return f.get();
    }

protected:
    static inline coroutineBase *currentCoroutine = nullptr;
    std::function<bool()> ready;

    static std::list<std::shared_ptr<coroutineBase>> coroutines;
    static contextType mainContext;
    // "yield" of main context
    static void _coYieldMain()
    {
        stepAll();
    }
};

template <class T>
class coroutine : public coroutineBase
{
public:
    // Construct coroutine and add to coroutines queue
    template <class F>
    static std::shared_ptr<coroutine> makeCoroutine(F &&callback)
    {
        auto ptr = std::shared_ptr<coroutine>(new coroutine(std::forward<F>(callback)));
        coroutines.push_back(std::static_pointer_cast<coroutineBase>(ptr));

        //console::log("cr with: ", _coAwaitMain);
        return ptr;
    }
    // Free context of coroutine
    ~coroutine()
    {
        isRunning = false;
        if (!consumed)
        {
            try
            {
                auto f = getFuture();
                if (f.valid() && f.wait_for(std::chrono::milliseconds::zero()) == std::future_status::ready)
                    f.get();
            }
            catch (...)
            {
                console::error("unhandled exception in Promise");
                handleException(std::current_exception());
            }
        }
        //console::log("d: ", this);
        deleteContext(currentContext);
    }
    // Swap context to coroutine and provide it with yield and await
    void step() override
    {
        coYield = [this]() { _yield(); };
        currentCoroutine = this;
        swapContext(mainContext, currentContext);
        currentCoroutine = nullptr;
        coYield = _coYieldMain;
    }

    bool running() override
    {
        return isRunning;
    }

    std::future<T> getFuture()
    {
        consumed = true;
        return result.get_future();
    }

    auto then(std::function<T(T)> callback)
    {
        return makeCoroutine([this, callback]() {
            return callback(coAwait(getFuture()));
        });
    }

private:
    // Construct coroutine context
    template <class F>
    coroutine(F &&f)
        : isRunning(true), function(std::forward<F>(f)), consumed(false)
    {
        initCurrentContext(currentContext, mainContext, stack);
        //console::debug("create ", this);
    }
    // "yield" of this coroutine
    void _yield()
    {
        coYield = _coYieldMain;
        //console::warn("Main: ", mainContext);
        swapContext(currentContext, mainContext);
    }
    // "call" of this coroutine
    void _call()
    {
        try
        {
            result.set_value(function());
        }
        catch (...)
        {
            //console::error("bad !!!");
            result.set_exception(std::current_exception());
        }
        isRunning = false;
        // Cannot exit context (at least in fiber api)
        while (true)
            _yield();
    }
    // Call as normal function pointer
    static void API call(void *data)
    {
        //console::warn("started ", data);
        reinterpret_cast<coroutine *>(data)->_call();
    }

    contextType currentContext;
    bool isRunning, consumed;
    std::function<T()> function;
    std::promise<T> result;
    stackType stack;
};

// Syntactic sugar for await and yield

struct awaitType
{
};

template <class T>
T operator*(awaitType &&, std::future<T> &&f)
{
    return coroutineBase::coAwait(std::move(f));
}

template <class T>
T operator*(awaitType &&, std::future<T> &f)
{
    return coroutineBase::coAwait(std::move(f));
}

template <class T>
T operator*(awaitType &&, std::shared_ptr<coroutine<T>> c)
{
    return coroutineBase::coAwait(c->getFuture());
}

#define await awaitType() *

#define yield coroutineBase::coYield()

template <class F>
auto makeCoroutine(F &&f)
{
    return coroutine<objectPtrImpl>::makeCoroutine(std::forward<F>(f));
}

#endif /* !COROUTINE_H_ */
