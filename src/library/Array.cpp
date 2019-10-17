#include "../library.h"

void Array::init(stack *st)
{
    auto Array = insertObject("Array"_n, constructorCaller);

    addFunctionL(Array, "readOperator"_n, {
        return makeObject(args);
    });

    (*Array)["prototype"_n] = object::arrayPrototype;

    addFunctionL(object::arrayPrototype, "constructor"_n, {
        thisObj->setType<object::arrayType>();
        return thisObj;
    });

    addFunctionL(object::arrayPrototype, "readOperator"_n, {
        argsConvertibleGuard<number>(args);
        int pos = static_cast<int>(args[0]->getConverted<number>());
        const object::arrayType& me = thisObj->get<const object::arrayType>();
        assert<std::greater_equal>(pos, 0);
        assert<std::less>(pos, me.size());
        return me[pos];
    });

    addFunctionL(object::arrayPrototype, "length"_n, {
        return makeObject(static_cast<number>(thisObj->get<const object::arrayType>().size()));
    });

    addFunctionL(object::arrayPrototype, "insertFrom"_n, {
        argsConvertibleGuard<number, object::arrayType, number, number>(args);
        argsGuard<nullptr_t, object::arrayType>(args);
        object::arrayType &dest = thisObj->get<object::arrayType>();
        const object::arrayType &src = args[1]->get<const object::arrayType>();
        int destPos = static_cast<int>(args[0]->getConverted<number>());
        int srcPos = static_cast<int>(args[2]->getConverted<number>());
        int srcLength = static_cast<int>(args[3]->getConverted<number>());
        assert<std::greater_equal>(destPos, 0);
        assert<std::less_equal>(destPos, dest.size());
        assert<std::greater_equal>(srcPos, 0);
        assert<std::greater_equal>(srcLength, 0);
        assert<std::less_equal>(srcPos + srcLength, src.size());
        dest.insert(dest.begin() + destPos, src.begin() + srcPos, src.begin() + srcPos + srcLength);
        return thisObj;
    });

    addFunctionL(object::arrayPrototype, "subarray"_n, {
        argsConvertibleGuard<number, number>(args);
        const object::arrayType &me = thisObj->get<const object::arrayType>();
        int pos = static_cast<int>(args[0]->getConverted<number>());
        int length = static_cast<int>(args[1]->getConverted<number>());
        assert<std::greater_equal>(pos, 0);
        assert<std::greater_equal>(length, 0);
        assert<std::less_equal>(pos + length, me.size());
        return makeObject(object::arrayType(me.begin() + pos, me.begin() + pos + length));
    });

    addFunctionL(object::arrayPrototype, "erase"_n, {
        argsConvertibleGuard<number, number>(args);
        object::arrayType &me = thisObj->get<object::arrayType>();
        int pos = static_cast<int>(args[0]->getConverted<number>());
        int length = static_cast<int>(args[1]->getConverted<number>());
        assert<std::greater_equal>(pos, 0);
        assert<std::greater_equal>(length, 0);
        assert<std::less_equal>(pos + length, me.size());
        me.erase(me.begin() + pos, me.begin() + pos + length);
        return thisObj;
    });
}