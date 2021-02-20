#include "../library.h"

void Number::init(stack *st)
{
    object::objectPtr Number = insertObject("Number"_n, constructorCaller);
    (*Number)[n::classPrototype] = object::numberPrototype;

    addFunctionL(object::numberPrototype, "toInteger"_n, {
        return object::makeObject(thisObj->get<const number>().toInteger());
    });
    addFunctionL(object::numberPrototype, "toFloatingPoint"_n, {
        return object::makeObject(thisObj->get<const number>().toFloatingPoint());
    });
}