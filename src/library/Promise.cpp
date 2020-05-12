#include "../library.h"

void Promise::init(stack *st)
{
    object::objectPtr Promise = insertObject("Promise"_n, constructorCaller);

    (*Promise)["classPrototype"_n] = object::promisePrototype;

    addFunctionL(object::promisePrototype, "constructor"_n, {
        argsConvertibleGuard<nullptr_t>(args);

        thisObj->setType<object::objectPromise::promisePtr>();

        thisObj->get<object::objectPromise::promisePtr>() = object::objectPromise::makePromise([args]() {
            // TODO release callback after call to free thisObj
            object::arrayType argsCopy = args;
            return (*argsCopy[0])(nullptr, {}, nullptr);
        });
        return thisObj;
    });

    addFunctionL(object::promisePrototype, "then"_n, {
        argsConvertibleGuard<nullptr_t>(args);
        
        return makeObject(thisObj->get<object::objectPromise::promisePtr>()->then([args](auto r) {
            // TODO release callback after call to free thisObj
            object::arrayType argsCopy = args;
            return (*argsCopy[0])(nullptr, {r}, nullptr);
        }));
    });

    addFunctionL(object::promisePrototype, "await"_n, {
        return thisObj->get<object::objectPromise::promisePtr&>()->await();
    });

    object::objectPtr ConditionalPromise = insertObject("ConditionalPromise"_n, constructorCaller);
    object::objectPtr conditionalPromisePrototype = makeEmptyObject();

    (*conditionalPromisePrototype)["prototype"_n] = object::promisePrototype;
    (*ConditionalPromise)["classPrototype"_n] = conditionalPromisePrototype;

    addFunctionL(conditionalPromisePrototype, "constructor"_n, {
        argsConvertibleGuard<nullptr_t, nullptr_t>(args);

        thisObj->setType<object::objectPromise::promisePtr>();

        thisObj->get<object::objectPromise::promisePtr>() = object::objectPromise::makePromise([args]() {
            // TODO release callback after call to free thisObj
            object::objectPtr f = args[0];
            return (*f)(nullptr, {}, nullptr);
        }, object::objectPromise::executionPolicy::synchronous, [args](){
            object::objectPtr f = args[1];
            return (*f)(nullptr, {}, nullptr)->getConverted<bool>();
        });
        return thisObj;
    });

    object::objectPtr Timeout = insertObject("Timeout"_n, constructorCaller);
    object::objectPtr timeoutPrototype = makeEmptyObject();

    (*timeoutPrototype)["prototype"_n] = object::promisePrototype;
    (*Timeout)["classPrototype"_n] = timeoutPrototype;

    addFunctionL(timeoutPrototype, "constructor"_n, {
        argsConvertibleGuard<number>(args);

        auto begin = std::chrono::steady_clock::now();
        number time = args[0]->get<const number>();

        thisObj->setType<object::objectPromise::promisePtr>();

        thisObj->get<object::objectPromise::promisePtr&>() = object::objectPromise::makePromise([]() {
            return makeEmptyObject();
        }, object::objectPromise::executionPolicy::synchronous, [begin, time](){
            return time.visit([begin](auto&& t){ return t <= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count(); });
        });
        return thisObj;
    });
}