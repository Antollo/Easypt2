#include "../library.h"

void Number::init(stack *st)
{
    object::objectPtr Number = insertObject("Number"_n, constructorCaller);
    (*Number)["classPrototype"_n] = object::numberPrototype;
}