#include "../library.h"
#include <cstdint>

std::string toId(std::uintptr_t ptr)
{
    constexpr int base = '9' - '0' + 'z' - 'a' + 'Z' - 'A' + 3;
    std::string res;
    do
    {
        int c = ptr % base;
        ptr /= base;
        if (c <= '9' - '0')
            c += '0';
        else if (c <= '9' - '0' + 'z' - 'a' + 1)
            c += 'a' - ('9' - '0' + 1);
        else if (c <= '9' - '0' + 'z' - 'a' + 'Z' - 'A' + 2)
            c += 'A' - ('9' - '0' + 'z' - 'a' + 2);
        res.push_back(c);
    } while (ptr);
    return res;
}

void Object::init(stack *st)
{
    object::objectPtr Object = insertObject("Object"_n, constructorCaller);

    (*Object)["classPrototype"_n] = object::objectPrototype;
    (*object::objectPrototype)[name::prototype] = object::objectPrototype;

    addFunctionL(object::objectPrototype, "hasOwnProperty"_n, {
        argsConvertibleGuard<std::string>(args);
        return object::makeObject(thisObj->hasOwnProperty(static_cast<name>(args[0]->getConverted<std::string>())));
    });

    addFunctionL(object::objectPrototype, "removeProperty"_n, {
        argsConvertibleGuard<std::string>(args);
        thisObj->removeProperty(static_cast<name>(args[0]->getConverted<std::string>()));
        return thisObj;
    });

    addFunctionL(object::objectPrototype, "addProperty"_n, {
        argsConvertibleGuard<std::string, nullptr_t>(args);
        thisObj->addProperty(static_cast<name>(args[0]->getConverted<std::string>()), args[1]);
        return args[1];
    });

    addFunctionL(object::objectPrototype, "getOwnPropertyNames"_n, {
        return object::makeObject(thisObj->getOwnPropertyNames());
    });

    addFunctionL(object::objectPrototype, "readOperator"_n, {
        argsConvertibleGuard<std::string>(args);
        return (*thisObj)[static_cast<name>(args[0]->getConverted<std::string>())];
    });

    addFunctionL(object::objectPrototype, "getId"_n, {
        std::uintptr_t ptr = reinterpret_cast<std::uintptr_t>(thisObj.get());
        return object::makeObject(toId(ptr));
    });
}