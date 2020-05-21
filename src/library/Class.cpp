#include "../library.h"

void Class::init(stack *st)
{
    object::objectPtr Class = insertObject("Class"_n, constructorCaller);
    (*object::classPrototype)[name::prototype] = object::functionPrototype;

    (*Class)["classPrototype"_n] = object::classPrototype;

    addFunctionL(object::classPrototype, "constructor"_n, {
        argsConvertibleGuard<nullptr_t>(args);
        thisObj->setType<object::nativeFunctionType>();
        thisObj->get<object::nativeFunctionType&>() = constructorCaller;
        thisObj->addProperty("classPrototype"_n, args[0]);
        return thisObj;
    });
}