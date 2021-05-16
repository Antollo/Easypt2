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

#include "coroutineEvent.h"
#include "console.h"
#include "objectPtrImpl.h"

// Common interface for fiber api and ucontext api

// 1 MB
constexpr std::size_t coroutineStackSize = 1048576;

#ifdef _WIN32

#include <windows.h>
// Initializer for contextType
#define initContext nullptr
// Convert the current thread to fiber, creates new fiber
// c - new fiber, m - main context
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
// Switch fibers
#define swapContext(a, b) SwitchToFiber(b)
// Delete fiber
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
// Initializer for contextType
#define initContext \
    {               \
    }
// Initialize new context
// c -new context, m - main context, s - stack
#define initCurrentContext(c, m, s)                                               \
    {                                                                             \
        getcontext(&c);                                                           \
        c.uc_stack.ss_sp = s.data();                                              \
        c.uc_stack.ss_size = s.size();                                            \
        c.uc_link = &m;                                                           \
        makecontext(&c, reinterpret_cast<void (*)()>(&coroutine::call), 1, this); \
    }
// Switch contexts
#define swapContext(a, b) swapcontext(&a, &b)
// Deletes context, noop
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
    coroutineBase()
    {
        coroutineEvent::sem.release();
    }
    virtual ~coroutineBase()
    {
        coroutineEvent::sem.acquire();
    }
    // Process all coroutines
    static bool stepAll()
    {
        if (coroutines.empty())
            return false;

        bool event = false;
        {
            {
                coroutineEvent wait; // Wait for something to happen
            }
            auto j = coroutines.begin();
            for (auto i = coroutines.begin(); i != coroutines.end();)
            {
                j = i++;
                // If coroutine doesn't have ready predicate or ready predicate returns true
                if (!(*j)->ready || (*j)->ready())
                {
                    if (!(*j)->running())
                        coroutines.erase(j);
                    else
                        (*j)->step();
                    event = true;
                }
            }
        }

        if (!event)
            std::this_thread::yield();

        return true;
    }
    static void handleException(std::exception_ptr ptr);
    static std::function<void()> coYield;

    template <class T>
    static T coAwait(std::future<T> &&f)
    {
        // If ready return immediately
        if (f.valid() && f.wait_for(std::chrono::milliseconds::zero()) == std::future_status::ready)
            return f.get();

        // Set ready predicate
        if (currentCoroutine)
            currentCoroutine->ready = [&f]() {
                return f.valid() && f.wait_for(std::chrono::milliseconds::zero()) == std::future_status::ready;
            };
        // Swap to main context
        coYield();
        while (!f.valid() || f.wait_for(std::chrono::milliseconds::zero()) != std::future_status::ready)
            coYield();
        if (currentCoroutine)
            currentCoroutine->ready = nullptr;
        return f.get();
    }

protected:
    std::function<bool()> ready;
    static coroutineBase *currentCoroutine;
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
        return ptr;
    }
    // Free context of coroutine
    ~coroutine()
    {
        isRunning = false;
        // If not consumed (no one called getFuture)
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
