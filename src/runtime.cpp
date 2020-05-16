//#include "statement.h"
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
    object::objectPrototype = makeEmptyObject();
    object::classPrototype = makeEmptyObject();

    object::arrayPrototype = makeEmptyObject();
    (*object::arrayPrototype)["prototype"_n] = object::objectPrototype;

    object::stringPrototype = makeEmptyObject();
    (*object::stringPrototype)["prototype"_n] = object::objectPrototype;

    object::numberPrototype = makeEmptyObject();
    (*object::numberPrototype)["prototype"_n] = object::objectPrototype;

    object::booleanPrototype = makeEmptyObject();
    (*object::booleanPrototype)["prototype"_n] = object::objectPrototype;

    object::functionPrototype = makeEmptyObject();
    (*object::functionPrototype)["prototype"_n] = object::objectPrototype;

    object::promisePrototype = makeEmptyObject();
    (*object::promisePrototype)["prototype"_n] = object::objectPrototype;

    addFunctionL(object::functionPrototype, "callOperator"_n, {
        return (*thisObj)(nullptr, std::move(args), st);
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
}

void runtime::fini(stack *st)
{
    while (!object::objectPromise::loopEmpty())
        object::objectPromise::loop();
    object::setGlobalStack(nullptr);
    st->clear();
}