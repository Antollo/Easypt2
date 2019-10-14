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

    addFunctionL(object::stringPrototype, "readOperator"_n, {
        argsConvertibleGuard<number>(args);
        return makeObject(std::string(1, thisObj->get<const std::string>().at(static_cast<int>(args[0]->getConverted<number>()))));
    });

    addFunctionL(object::functionPrototype, "callOperator"_n, {
        argsConvertibleGuard<std::string>(args);
        return (*thisObj)(nullptr, std::move(args), st);
    });

    insertObject("true"_n, true);
    insertObject("false"_n, false);

    insertObject("import"_n, import);

    operators::init(st);
    Object::init(st);
    Array::init(st);
    consoleObj::init(st);
}

void runtime::fini(stack *st)
{
    st->clear();
}