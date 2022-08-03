#include "../library.h"

void Accessible::init(stack *st)
{
    object::objectPtr Accessible = insertObject("Accessible"_n, constructorCaller);
    object::objectPtr accessiblePrototype = object::makeEmptyObject();

    (*Accessible)[n::classPrototype] = accessiblePrototype;

    accessiblePrototype->addFunctionL(n::constructor, {
        thisObj->setType<object::typeIndex::Object>();
        thisObj->setAccessible();

        thisObj->addProperty("owner"_n, (*st)["this"_n]);
        if (args.size() > 0)
        {
            if (args[0]->hasOwnProperty("get"_n))
                thisObj->addProperty("get"_n, (*args[0])["get"_n]);

            if (args[0]->hasOwnProperty("set"_n))
                thisObj->addProperty("set"_n, (*args[0])["set"_n]);
        }
        return thisObj;
    });
}