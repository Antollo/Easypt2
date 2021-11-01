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

    (*Object)[n::classPrototype] = object::objectPrototype;
    (*object::objectPrototype)[n::prototype] = object::objectPrototype;

    addFunctionL(object::objectPrototype, "hasOwnProperty"_n, {
        argsConvertibleGuard<std::string>(args);
        return object::makeObject(thisObj->hasOwnProperty(static_cast<name>(args[0]->getConverted<object::type::String>())));
    });

    addFunctionL(object::objectPrototype, "removeProperty"_n, {
        argsConvertibleGuard<std::string>(args);
        thisObj->removeProperty(static_cast<name>(args[0]->getConverted<object::type::String>()));
        return thisObj;
    });

    addFunctionL(object::objectPrototype, "addProperty"_n, {
        argsConvertibleGuard<std::string, nullptr_t>(args);
        thisObj->addProperty(static_cast<name>(args[0]->getConverted<object::type::String>()), args[1]);
        return args[1];
    });

    addFunctionL(object::objectPrototype, "getOwnPropertyNames"_n, {
        return object::makeObject(thisObj->getOwnPropertyNames());
    });

    addFunctionL(object::objectPrototype, n::readOperator, {
        argsConvertibleGuard<std::string>(args);
        return (*thisObj)[static_cast<name>(args[0]->getConverted<object::type::String>())];
    });

    addFunctionL(object::objectPrototype, "getId"_n, {
        std::uintptr_t ptr = reinterpret_cast<std::uintptr_t>(thisObj.get());
        return object::makeObject(toId(ptr));
    });

    addFunctionL(object::objectPrototype, n::toNumber, {
        return object::makeObject(thisObj->getConverted<object::type::Number>());
    });

    addFunctionL(object::objectPrototype, n::toString, {
        return object::makeObject(thisObj->getConverted<object::type::String>());
    });

    addFunctionL(object::objectPrototype, n::toArray, {
        return object::makeObject(thisObj->getConverted<object::type::Array>());
    });

    addFunctionL(object::objectPrototype, n::toBoolean, {
        return object::makeObject(thisObj->getConverted<object::type::Boolean>());
    });

    addFunctionL(object::objectPrototype, n::equal, {
        argsGuard<std::nullptr_t>(args);
        return object::makeObject(*thisObj == *args[0]);
    });

    object::toNumber = (*object::objectPrototype)[n::toNumber];
    object::toString = (*object::objectPrototype)[n::toString];
    object::toArray = (*object::objectPrototype)[n::toArray];
    object::toBoolean = (*object::objectPrototype)[n::toBoolean];
}