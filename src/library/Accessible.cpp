#include "../library.h"

void Accessible::init(stack *st)
{
    object::objectPtr Accessible = insertObject("Accessible"_n, constructorCaller);
    object::objectPtr accessablePrototype = object::makeEmptyObject();

    (*Accessible)["classPrototype"_n] = accessablePrototype;

    addFunctionL(accessablePrototype, "constructor"_n, {
        thisObj->setType<nullptr_t>();
        thisObj->setAccessible();
        return thisObj;
    });
}