#include "statement.h"
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
    object::objectPrototype = makeUndefined();

    object::arrayPrototype = makeUndefined();
    (*object::arrayPrototype)["prototype"_n] = object::objectPrototype;

    object::stringPrototype = makeUndefined();
    (*object::stringPrototype)["prototype"_n] = object::objectPrototype;

    object::numberPrototype = makeUndefined();
    (*object::numberPrototype)["prototype"_n] = object::objectPrototype;

    object::booleanPrototype = makeUndefined();
    (*object::booleanPrototype)["prototype"_n] = object::objectPrototype;

    object::functionPrototype = makeUndefined();
    (*object::functionPrototype)["prototype"_n] = object::objectPrototype;

    addFunctionL(object::functionPrototype, "callOperator"_n, {
        return (*thisObj)(nullptr, std::move(args), st);
    });

    insertObject("true"_n, true);
    insertObject("false"_n, false);

    insertObject("import"_n, import);
    insertObject("parse"_n, parse);
    insertObject("transpile"_n, transpile);
    insertObject("getStack"_n, getStack);

    object::objectPtr Function = insertObject("Function"_n, nullptr);
    (*Function)["prototype"_n] = object::functionPrototype;
    
    operators::init(st);
    Object::init(st);
    Array::init(st);
    String::init(st);
    Number::init(st);
    consoleObj::init(st);
    File::init(st);
}

void runtime::fini(stack *st)
{
    st->clear();
}