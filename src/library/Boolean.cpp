#include "../library.h"

void Boolean::init(stack *st)
{
    object::objectPtr Boolean = insertObject("Boolean"_n, constructorCaller);

    (*Boolean)["prototype"_n] = object::booleanPrototype;
}