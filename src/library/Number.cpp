#include "../library.h"

void Number::init(stack *st)
{
    object::objectPtr Number = insertObject("Number"_n, constructorCaller);
    (*Number)[n::classPrototype] = object::numberPrototype;

    object::numberPrototype->addFunctionL("toInteger"_n, {
        return object::makeObject(thisObj->get<const number>().toInteger());
    });

    object::numberPrototype->addFunctionL("toFloatingPoint"_n, {
        return object::makeObject(thisObj->get<const number>().toFloatingPoint());
    });
}