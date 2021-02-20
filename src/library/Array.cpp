#include "../library.h"

void Array::init(stack *st)
{
    object::objectPtr Array = insertObject("Array"_n, constructorCaller);

    addFunctionL(Array, n::readOperator, {
        return object::makeObject(args);
    });

    (*Array)[n::classPrototype] = object::arrayPrototype;

    addFunctionL(object::arrayPrototype, n::constructor, {
        thisObj->setType<object::type::Array>();
        return thisObj;
    });

    addFunctionL(object::arrayPrototype, n::readOperator, {
        argsConvertibleGuard<number>(args);
        int pos = static_cast<int>(args[0]->getConverted<number>());
        const object::type::Array &me = thisObj->get<const object::type::Array>();
        assert<std::greater_equal>(pos, 0);
        assert<std::less>(pos, me.size());
        return me[pos];
    });

    addFunctionL(object::arrayPrototype, "length"_n, {
        return object::makeObject(static_cast<number>(thisObj->get<const object::type::Array>().size()));
    });

    addFunctionL(object::arrayPrototype, "insert"_n, {
        argsConvertibleGuard<number, nullptr_t>(args);
        object::type::Array &dest = thisObj->get<object::type::Array>();
        int destPos = static_cast<int>(args[0]->getConverted<number>());
        assert<std::greater_equal>(destPos, 0);
        assert<std::less_equal>(destPos, dest.size());
        dest.insert(dest.begin() + destPos, args[1]);
        return thisObj;
    });

    addFunctionL(object::arrayPrototype, "insertFrom"_n, {
        argsConvertibleGuard<number, object::type::Array>(args);
        object::type::Array &dest = thisObj->get<object::type::Array>();
        const object::type::Array &src = args[1]->get<const object::type::Array>();
        int destPos = static_cast<int>(args[0]->getConverted<number>());
        int srcPos = 0;
        int srcLength = src.size();
        if (args.size() > 2)
        {
            argsConvertibleGuard<nullptr_t, nullptr_t, number>(args);
            srcPos = static_cast<int>(args[2]->getConverted<number>());
            srcLength = srcLength - srcPos;
            if (args.size() > 3)
            {
                argsConvertibleGuard<nullptr_t, nullptr_t, number, number>(args);
                srcLength = std::min(srcLength, static_cast<int>(args[3]->getConverted<number>()));
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

    addFunctionL(object::arrayPrototype, "subarray"_n, {
        argsConvertibleGuard<number>(args);
        const object::type::Array &me = thisObj->get<const object::type::Array>();
        int pos = static_cast<int>(args[0]->getConverted<number>());
        int length = me.size() - pos;
        if (args.size() > 1)
        {
            argsConvertibleGuard<nullptr_t, number>(args);
            length = std::min(length, static_cast<int>(args[1]->getConverted<number>()));
        }
        assert<std::greater_equal>(pos, 0);
        assert<std::greater_equal>(length, 0);
        assert<std::less_equal>(pos + length, me.size());
        return object::makeObject(object::type::Array(me.begin() + pos, me.begin() + pos + length));
    });

    addFunctionL(object::arrayPrototype, "erase"_n, {
        argsConvertibleGuard<number>(args);
        object::type::Array &me = thisObj->get<object::type::Array>();
        int pos = static_cast<int>(args[0]->getConverted<number>());
        int length = me.size() - pos;
        if (args.size() > 1)
        {
            argsConvertibleGuard<nullptr_t, number>(args);
            length = std::min(length, static_cast<int>(args[1]->getConverted<number>()));
        }
        assert<std::greater_equal>(pos, 0);
        assert<std::greater_equal>(length, 0);
        assert<std::less_equal>(pos + length, me.size());
        me.erase(me.begin() + pos, me.begin() + pos + length);
        return thisObj;
    });

    addFunctionL(object::arrayPrototype, "forEach"_n, {
        argsConvertibleGuard<nullptr_t>(args);
        object::type::Array &me = thisObj->get<object::type::Array>();
        for (int i = 0; i < me.size(); i++)
            (*args[0])(args[0], {me[i], object::makeObject(static_cast<number>(i)), thisObj}, st);
        return thisObj;
    });
}