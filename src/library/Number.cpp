#include "../library.h"

void Number::init(stack *st)
{
    object::objectPtr Number = insertObject("Number"_n, constructorCaller);
    (*Number)["prototype"_n] = object::numberPrototype;
}