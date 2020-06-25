#include "../library.h"

void Promise::init(stack *st)
{
    object::objectPtr Promise = insertObject("Promise"_n, constructorCaller);

    (*Promise)["classPrototype"_n] = object::promisePrototype;

    addFunctionL(object::promisePrototype, "constructor"_n, {
        argsConvertibleGuard<nullptr_t>(args);

        thisObj->setType<object::objectCoroutine>();

        thisObj->get<object::objectCoroutine>() = coroutine<object::objectPtr>::makeCoroutine([args]() {
            object::arrayType argsCopy = args;
            return (*argsCopy[0])(argsCopy[0], {}, nullptr);
        });
        return thisObj;
    });

    addFunctionL(object::promisePrototype, "then"_n, {
        argsConvertibleGuard<nullptr_t>(args);

        return object::makeObject(thisObj->get<object::objectCoroutine>()->then([args](object::objectPtr r) {
            object::arrayType argsCopy = args;
            return (*argsCopy[0])(argsCopy[0], {r, argsCopy[0]}, nullptr);
        }));
    });

    addFunctionL(object::promisePrototype, "await"_n, {
        return await thisObj->get<object::objectCoroutine>()->getFuture();
    });

    addFunctionL(Promise, "tick"_n, {
        yield;
        return thisObj;
    });

    object::objectPtr ConditionalPromise = insertObject("ConditionalPromise"_n, constructorCaller);
    object::objectPtr conditionalPromisePrototype = object::makeEmptyObject();

    (*conditionalPromisePrototype)[name::prototype] = object::promisePrototype;
    (*ConditionalPromise)["classPrototype"_n] = conditionalPromisePrototype;

    addFunctionL(conditionalPromisePrototype, "constructor"_n, {
        argsConvertibleGuard<nullptr_t, nullptr_t>(args);

        thisObj->setType<object::objectCoroutine>();

        thisObj->get<object::objectCoroutine>() = coroutine<object::objectPtr>::makeCoroutine([args]() {
            object::objectPtr f = args[0];
            object::objectPtr c = args[1];
            while ((*c)(c, {}, nullptr)->getConverted<bool>())
                yield;
            return (*f)(f, {}, nullptr);
        });
        return thisObj;
    });

    object::objectPtr Timeout = insertObject("Timeout"_n, constructorCaller);
    object::objectPtr timeoutPrototype = object::makeEmptyObject();

    (*timeoutPrototype)[name::prototype] = object::promisePrototype;
    (*Timeout)["classPrototype"_n] = timeoutPrototype;

    addFunctionL(timeoutPrototype, "constructor"_n, {
        argsConvertibleGuard<number>(args);

        auto begin = std::chrono::steady_clock::now();
        number time = args[0]->get<const number>();

        thisObj->setType<object::objectCoroutine>();

        thisObj->get<object::objectCoroutine>() = coroutine<object::objectPtr>::makeCoroutine(
            [begin, time]() {
                while (!time.visit([begin](auto &&t) {
                    return t <= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count();
                }))
                    yield;

                return object::makeEmptyObject();
            });
        return thisObj;
    });
}