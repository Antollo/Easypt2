#include "../library.h"

void Array::init(stack *st)
{
    object::objectPtr Array = insertObject("Array"_n, constructorCaller);
    (*Array)[n::classPrototype] = object::arrayPrototype;

    Array->addFunctionL(n::readOperator, {
        return object::makeObject(args);
    });

    object::arrayPrototype->addFunctionL(n::constructor, {
        thisObj->setType<object::type::Array>();
        return thisObj;
    });

    object::arrayPrototype->addFunctionL(n::readOperator, {
        argsConvertibleGuard<number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Array &me = thisObj->get<object::type::Array>();
        assert<std::greater_equal>(pos, 0);
        assert<std::less>(pos, me.size());
        return me[pos];
    });

    object::arrayPrototype->addFunctionL("length"_n, {
        return object::makeObject(static_cast<number>(thisObj->get<object::type::Array>().size()));
    });

    object::arrayPrototype->addFunctionL("insert"_n, {
        argsConvertibleGuard<number, nullptr_t>(args);
        object::type::Array &dest = thisObj->get<object::type::Array>();
        size_t destPos= static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        assert<std::greater_equal>(destPos, 0);
        assert<std::less_equal>(destPos, dest.size());
        dest.insert(dest.begin() + destPos, args[1]);
        return thisObj;
    });

    object::arrayPrototype->addFunctionL("insertFrom"_n, {
        argsConvertibleGuard<number, object::type::Array>(args);
        object::type::Array &dest = thisObj->get<object::type::Array>();
        const object::type::Array &src = args[1]->get<const object::type::Array>();
        size_t destPos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        size_t srcPos = 0;
        size_t srcLength = src.size();
        if (args.size() > 2)
        {
            argsConvertibleGuard<nullptr_t, nullptr_t, number>(args);
            srcPos = static_cast<size_t>(args[2]->getConverted<object::type::Number>());
            srcLength = srcLength - srcPos;
            if (args.size() > 3)
            {
                argsConvertibleGuard<nullptr_t, nullptr_t, number, number>(args);
                srcLength = std::min(srcLength, static_cast<size_t>(args[3]->getConverted<object::type::Number>()));
            }
        }
        assert<std::greater_equal>(destPos, 0);
        assert<std::less_equal>(destPos, dest.size());
        assert<std::greater_equal>(srcPos, 0);
        assert<std::greater_equal>(srcLength, 0);
        assert<std::less_equal>(srcPos + srcLength, src.size());
        dest.insert(dest.begin() + destPos, src.begin() + srcPos, src.begin() + srcPos + srcLength);
        return thisObj;
    });

    object::arrayPrototype->addFunctionL("append"_n, {
        argsConvertibleGuard<object::type::Array>(args);
        object::type::Array &dest = thisObj->get<object::type::Array>();
        const object::type::Array &src = args[0]->get<const object::type::Array>();
        size_t srcPos = 0;
        size_t srcLength = src.size();
        if (args.size() > 1)
        {
            argsConvertibleGuard<nullptr_t, number>(args);
            srcPos = static_cast<size_t>(args[1]->getConverted<object::type::Number>());
            srcLength = srcLength - srcPos;
            if (args.size() > 2)
            {
                argsConvertibleGuard<nullptr_t, nullptr_t, number>(args);
                srcLength = std::min(srcLength, static_cast<size_t>(args[2]->getConverted<object::type::Number>()));
            }
        }
        assert<std::greater_equal>(srcPos, 0);
        assert<std::greater_equal>(srcLength, 0);
        assert<std::less_equal>(srcPos + srcLength, src.size());
        dest.insert(dest.end(), src.begin() + srcPos, src.begin() + srcPos + srcLength);
        return thisObj;
    });

    object::arrayPrototype->addFunctionL("subarray"_n, {
        argsConvertibleGuard<number>(args);
        object::type::Array &me = thisObj->get<object::type::Array>();
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        size_t length = me.size() - pos;
        if (args.size() > 1)
        {
            argsConvertibleGuard<nullptr_t, number>(args);
            length = std::min(length, static_cast<size_t>(args[1]->getConverted<object::type::Number>()));
        }
        assert<std::greater_equal>(pos, 0);
        assert<std::greater_equal>(length, 0);
        assert<std::less_equal>(pos + length, me.size());
        return object::makeObject(object::type::Array(me.begin() + pos, me.begin() + pos + length));
    });

    object::arrayPrototype->addFunctionL("erase"_n, {
        argsConvertibleGuard<number>(args);
        object::type::Array &me = thisObj->get<object::type::Array>();
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        size_t length = me.size() - pos;
        if (args.size() > 1)
        {
            argsConvertibleGuard<nullptr_t, number>(args);
            length = std::min(length, static_cast<size_t>(args[1]->getConverted<object::type::Number>()));
        }
        assert<std::greater_equal>(pos, 0);
        assert<std::greater_equal>(length, 0);
        assert<std::less_equal>(pos + length, me.size());
        me.erase(me.begin() + pos, me.begin() + pos + length);
        return thisObj;
    });

    object::arrayPrototype->addFunctionL("forEach"_n, {
        argsConvertibleGuard<nullptr_t>(args);
        object::type::Array &me = thisObj->get<object::type::Array>();
        for (size_t i = 0; i < me.size(); i++)
            (*args[0])(args[0], {me[i], object::makeObject(static_cast<number>(i)), thisObj}, st);
        return thisObj;
    });
}