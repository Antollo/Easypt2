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
    (*object::arrayPrototype)[name::prototype] = object::objectPrototype;

    object::stringPrototype = object::makeEmptyObject();
    (*object::stringPrototype)[name::prototype] = object::objectPrototype;

    object::numberPrototype = object::makeEmptyObject();
    (*object::numberPrototype)[name::prototype] = object::objectPrototype;

    object::booleanPrototype = object::makeEmptyObject();
    (*object::booleanPrototype)[name::prototype] = object::objectPrototype;

    object::functionPrototype = object::makeEmptyObject();
    (*object::functionPrototype)[name::prototype] = object::objectPrototype;

    object::promisePrototype = object::makeEmptyObject();
    (*object::promisePrototype)[name::prototype] = object::objectPrototype;

    addFunctionL(object::functionPrototype, "callOperator"_n, {
        return (*thisObj)(thisObj, std::move(args), st);
    });

    addFunctionL(object::functionPrototype, "call"_n, {
        argsConvertibleGuard<nullptr_t>(args);
        auto thisArg = args[0];
        args.erase(args.begin());
        return (*thisObj)(thisArg, std::move(args), st);
    });

    insertObject("true"_n, true)->setConst();
    insertObject("false"_n, false)->setConst();

    insertObject("import"_n, import);
    insertObject("parse"_n, ez_parse);
    insertObject("execute"_n, execute);
    insertObject("getStack"_n, getStack);

    object::objectPtr Function = insertObject("Function"_n, nullptr);
    (*Function)["classPrototype"_n] = object::functionPrototype;

    operators::init(st);
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
}

void runtime::fini(stack *st)
{
    while(coroutine<object::objectPtr>::stepAll());
    object::setGlobalStack(nullptr);
    st->clear();
}