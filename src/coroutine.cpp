#include "coroutine.h"
#include "nobject.h"

std::function<void()> coroutineBase::coYield = coroutineBase::_coYieldMain;
contextType coroutineBase::mainContext = initContext;
std::list<std::shared_ptr<coroutineBase>> coroutineBase::coroutines;
coroutineBase *coroutineBase::currentCoroutine = nullptr;

void coroutineBase::handleException(std::exception_ptr ptr)
{
    try
    {
        std::rethrow_exception(ptr);
    }
    catch (objectException &e)
    {
        auto eObj = e.getPtr();
        console::printStackTrace();
        if (eObj->isConvertible<std::string>())
            console::error(eObj->getConverted<object::type::String>());
        else
            console::error((std::string)e.what());
    }
    catch (std::exception &e)
    {
        console::printStackTrace();
        console::error((std::string)e.what());
    }
    catch (...)
    {
        console::printStackTrace();
        console::error("unknown error in destructor");
    }
}