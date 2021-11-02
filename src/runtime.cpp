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

    addFunctionL(object::functionPrototype, "call"_n, {
        argsConvertibleGuard<nullptr_t>(args);
        auto thisArg = args[0];
        args.erase(args.begin());
        return (*thisObj)(thisArg, std::move(args), st);
    });

    addFunctionL(object::functionPrototype, "__explain"_n, {
        return object::makeObject(thisObj->get<object::type::Function>().first->explain());
    });

    object::objectPtr Function = insertObject("Function"_n, constructorCaller);
    (*Function)[n::classPrototype] = object::functionPrototype;

    insertObject("true"_n, true)->setConst();
    insertObject("false"_n, false)->setConst();
    
    auto import = insertObject(n::import, &Import::import);
    import->addProperty("getImportPaths"_n, object::makeObject(&Import::getImportPaths));

    insertObject("parse"_n, ez_parse);
    insertObject(n::execute, execute);
    insertObject("getStack"_n, getStack);


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

    auto modules = st->insert("modules"_n, object::makeEmptyObject());
    Import::init(modules);
}

void runtime::fini(stack *st)
{
    while (coroutine<object::objectPtr>::stepAll())
        ;
    Import::fini();
    object::setGlobalStack(nullptr);
    st->clear();
    Array::fini();
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