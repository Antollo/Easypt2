#include "coroutine.h"
#include "nobject.h"

std::function<void()> coroutineBase::coYield = coroutineBase::_coYieldMain;
contextType coroutineBase::mainContext = initContext;
std::list<std::shared_ptr<coroutineBase>> coroutineBase::coroutines;

void coroutineBase::handleException(std::exception_ptr ptr)
{
    try
    {
        std::rethrow_exception(ptr);
    }
    catch (objectException &e)
    {
        auto eObj = e.getPtr();
        console::stackTrace();
        if (eObj->isConvertible<std::string>())
            console::error(eObj.getConverted<object::type::String>());
        else
            console::error((std::string)e.what());
    }
    catch (std::exception &e)
    {
        console::stackTrace();
        console::error((std::string)e.what());
    }
    catch (...)
    {
        console::stackTrace();
        console::error("unknown error in destructor");
    }
}