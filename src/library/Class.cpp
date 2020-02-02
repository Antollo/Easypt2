#include "../library.h"

void Class::init(stack *st)
{
    object::objectPtr Class = insertObject("Class"_n, constructorCaller);
    object::objectPtr classPrototype = makeEmptyObject();

    (*Class)["prototype"_n] = classPrototype;

    addFunctionL(classPrototype, "constructor"_n, {
        argsConvertibleGuard<nullptr_t>(args);
        thisObj->setType<object::nativeFunctionType>();
        thisObj->get<object::nativeFunctionType&>() = constructorCaller;
        thisObj->addProperty("prototype"_n, args[0]);
        return thisObj;
    });
}