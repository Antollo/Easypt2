#include "../library.h"

void Object::init(stack *st)
{
    auto Object = insertObject("Object"_n, constructorCaller);

    (*Object)["prototype"_n] = object::objectPrototype;
    
    addFunctionL(object::objectPrototype, "hasOwnProperty"_n, {
        argsConvertibleGuard<std::string>(args);
        return makeObject(thisObj->hasOwnProperty(static_cast<name>(args[0]->getConverted<std::string>())));
    });

    addFunctionL(object::objectPrototype, "addProperty"_n, {
        argsConvertibleGuard<std::string, nullptr_t>(args);
        thisObj->addProperty(static_cast<name>(args[0]->getConverted<std::string>()), args[1]);
        return args[1];
    });

    addFunctionL(object::objectPrototype, "getOwnPropertyNames"_n, {
        return makeObject(thisObj->getOwnPropertyNames());
    });

    addFunctionL(object::objectPrototype, "readOperator"_n, {
        argsConvertibleGuard<std::string>(args);
        return (*thisObj)[static_cast<name>(args[0]->getConverted<std::string>())];
    });
}