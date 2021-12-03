#include "../library.h"

void Promise::init(stack *st)
{
    object::objectPtr Promise = insertObject("Promise"_n, constructorCaller);

    (*Promise)[n::classPrototype] = object::promisePrototype;

    object::promisePrototype->addFunctionL(n::constructor, {
        argsConvertibleGuard<nullptr_t>(args);

        thisObj->setType<object::type::Promise>();

        thisObj->get<object::type::Promise>() = coroutine<object::objectPtr>::makeCoroutine([args]() {
            object::type::Array argsCopy = args;
            return (*argsCopy[0])(argsCopy[0], {}, nullptr);
        });
        return thisObj;
    });

    object::promisePrototype->addFunctionL("then"_n, {
        argsConvertibleGuard<nullptr_t>(args);

        return object::makeObject(thisObj->get<object::type::Promise>()->then([args](object::objectPtr r) {
            object::type::Array argsCopy = args;
            return (*argsCopy[0])(argsCopy[0], {r}, nullptr);
        }));
    });

    object::promisePrototype->addFunctionL(n::await_, {
        return await thisObj->get<object::type::Promise>()->getFuture();
    });

    Promise->addFunctionL("tick"_n, {
        yield;
        return thisObj;
    });

    object::objectPtr ConditionalPromise = insertObject("ConditionalPromise"_n, constructorCaller);
    object::objectPtr conditionalPromisePrototype = object::makeEmptyObject();

    (*conditionalPromisePrototype)[n::prototype] = object::promisePrototype;
    (*ConditionalPromise)[n::classPrototype] = conditionalPromisePrototype;

    conditionalPromisePrototype->addFunctionL(n::constructor, {
        argsConvertibleGuard<nullptr_t, nullptr_t>(args);

        thisObj->setType<object::type::Promise>();

        thisObj->get<object::type::Promise>() = coroutine<object::objectPtr>::makeCoroutine([args]() {
            object::objectPtr f = args[0];
            object::objectPtr c = args[1];
            while ((*c)(c, {}, nullptr)->getConverted<object::type::Boolean>())
                yield;
            return (*f)(f, {}, nullptr);
        });
        return thisObj;
    });

    object::objectPtr Timeout = insertObject("Timeout"_n, constructorCaller);
    object::objectPtr timeoutPrototype = object::makeEmptyObject();

    (*timeoutPrototype)[n::prototype] = object::promisePrototype;
    (*Timeout)[n::classPrototype] = timeoutPrototype;

    timeoutPrototype->addFunctionL(n::constructor, {
        argsConvertibleGuard<number>(args);

        auto begin = std::chrono::steady_clock::now();
        number time = args[0]->get<const number>();

        thisObj->setType<object::type::Promise>();

        thisObj->get<object::type::Promise>() = coroutine<object::objectPtr>::makeCoroutine(
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