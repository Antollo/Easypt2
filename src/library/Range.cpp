#include "../library.h"
#include "../iterator.h"

void Range::init(stack *st)
{
    object::objectPtr Range = insertObject("Range"_n, constructorCaller);

    (*Range)[n::classPrototype] = object::rangePrototype;

    object::rangePrototype->addFunctionL(n::constructor, {
        thisObj->setType<object::typeIndex::Range>();
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);

        number start = args[0]->getConverted<object::type::Number>();
        number stop = args[1]->getConverted<object::type::Number>();
        number step = args.size() > 2 ? args[2]->getConverted<object::type::Number>() : (start < stop ? 1_n : -1_n);
        thisObj->get<object::type::Range>() = std::make_shared<range>(start, stop, step);
        return thisObj;
    });

    object::rangePrototype->addFunctionL("iterator"_n, {
        return object::makeObject(thisObj->iterator());
    });

    object::rangePrototype->addFunctionL("start"_n, {
        return object::makeObject(thisObj->get<object::type::Range>()->getStart());
    });

    object::rangePrototype->addFunctionL("stop"_n, {
        return object::makeObject(thisObj->get<object::type::Range>()->getStop());
    });

    object::rangePrototype->addFunctionL("step"_n, {
        return object::makeObject(thisObj->get<object::type::Range>()->getStep());
    });

    object::rangePrototype->addFunctionL("length"_n, {
        return object::makeObject(thisObj->get<object::type::Range>()->size());
    });

    object::rangePrototype->addFunctionL(n::readOperator, {
        argsConvertibleGuard<object::type::Number>(args);
        return object::makeObject((*thisObj->get<object::type::Range>())[args[0]->getConverted<object::type::Number>()]);
    });
}