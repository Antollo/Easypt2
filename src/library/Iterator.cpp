#include "../library.h"
#include "../iterator.h"

void Iterator::init(stack *st)
{
    object::objectPtr Iterator = insertObject("Iterator"_n, constructorCaller);
    (*Iterator)[n::classPrototype] = object::iteratorPrototype;

    object::iteratorPrototype->addFunctionL(n::constructor, {
        thisObj->setType<object::typeIndex::Iterator>();
        thisObj->get<object::type::Iterator>() = std::make_shared<emptyIterator>();
        return thisObj;
    });

    object::iteratorPrototype->addFunctionL("next"_n, {
        return thisObj->get<object::type::Iterator>()->next();
    });

    object::iteratorEnd = object::makeObject(false);
    object::iteratorEnd->setConst();
    Iterator->addProperty("end"_n, object::iteratorEnd);
}