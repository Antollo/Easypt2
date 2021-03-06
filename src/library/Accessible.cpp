#include "../library.h"

void Accessible::init(stack *st)
{
    object::objectPtr Accessible = insertObject("Accessible"_n, constructorCaller);
    object::objectPtr accessablePrototype = object::makeEmptyObject();

    (*Accessible)[n::classPrototype] = accessablePrototype;

    addFunctionL(accessablePrototype, n::constructor, {
        thisObj->setType<nullptr_t>();
        thisObj->setAccessible();
        return thisObj;
    });
}