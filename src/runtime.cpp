#include "Node.h"
#include "nobject.h"
#include "runtime.h"
#include "common.h"
#include "console.h"
#include "defines.h"
#include "core.h"

#include "library.h"

#pragma warning(disable : 4100)

void runtime::init(stack *st)
{
    object::setGlobalStack(st);
    object::objectPrototype = object::makeEmptyObject();
    object::classPrototype = object::makeEmptyObject();

    object::arrayPrototype = object::makeEmptyObject();
    (*object::arrayPrototype)[n::prototype] = object::objectPrototype;

    object::stringPrototype = object::makeEmptyObject();
    (*object::stringPrototype)[n::prototype] = object::objectPrototype;

    object::numberPrototype = object::makeEmptyObject();
    (*object::numberPrototype)[n::prototype] = object::objectPrototype;

    object::booleanPrototype = object::makeEmptyObject();
    (*object::booleanPrototype)[n::prototype] = object::objectPrototype;

    object::functionPrototype = object::makeEmptyObject();
    (*object::functionPrototype)[n::prototype] = object::objectPrototype;

    object::promisePrototype = object::makeEmptyObject();
    (*object::promisePrototype)[n::prototype] = object::objectPrototype;

    object::iteratorPrototype = object::makeEmptyObject();
    (*object::iteratorPrototype)[n::prototype] = object::objectPrototype;

    object::rangePrototype = object::makeEmptyObject();
    (*object::rangePrototype)[n::prototype] = object::objectPrototype;

    object::functionPrototype->addFunctionL("call"_n, {
        argsConvertibleGuard<nullptr_t>(args);
        auto thisArg = args[0];
        args.erase(args.begin());
        return (*thisObj)(thisArg, std::move(args), st);
    });

    object::functionPrototype->addFunctionL("__explain"_n, {
        return object::makeObject(thisObj->get<object::type::Function>()->node.explain());
    });

    object::functionPrototype->addFunctionL("__jit"_n, {
        return object::makeObject(thisObj->get<object::type::Function>()->node.toIntermediateLanguage());
    });

    object::objectPtr Function = insertObject("Function"_n, constructorCaller);
    (*Function)[n::classPrototype] = object::functionPrototype;

    insertObject("true"_n, true)->setConst();
    insertObject("false"_n, false)->setConst();

    auto import = insertObject(n::import, &modules::import);
    import->addProperty("getImportPaths"_n, object::makeObject(static_cast<object::objectPtr(*)(object::objectPtr, object::type::Array&&, stack*)>(&modules::getImportPaths)));

    insertObject("parse"_n, interpreter::parse);
    insertObject(n::execute, interpreter::execute);
    insertObject("getStack"_n, interpreter::getStack);

    Object::init(st);
    Array::init(st);
    String::init(st);
    Boolean::init(st);
    Number::init(st);
    consoleObj::init(st);
    File::init(st);
    Class::init(st);
    Promise::init(st);
    Time::init(st);
    Tcp::init(st);
    Accessible::init(st);
    ChildProcess::init(st);
    Buffer::init(st);
    Iterator::init(st);
    Range::init(st);

    auto modulesPtr = st->insert("modules"_n, object::makeEmptyObject());
    modules::init(modulesPtr);
}

void runtime::fini(stack *st)
{
    while (coroutine<object::objectPtr>::stepAll())
        ;
    modules::fini();
    object::setGlobalStack(nullptr);
    st->clear();
    object::numberPrototype = nullptr;
    object::stringPrototype = nullptr;
    object::booleanPrototype = nullptr;
    object::arrayPrototype = nullptr;
    object::functionPrototype = nullptr;
    object::promisePrototype = nullptr;
    object::classPrototype = nullptr;
    object::toNumber = nullptr;
    object::toString = nullptr;
    object::toArray = nullptr;
    object::toBoolean = nullptr;
    (*object::objectPrototype)[n::prototype] = nullptr;
    object::objectPrototype = nullptr;
}