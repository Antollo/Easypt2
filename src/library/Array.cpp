#include "../library.h"

object::objectPtr arrayViewPrototype;

void Array::fini()
{
    arrayViewPrototype = nullptr;
}

void Array::init(stack *st)
{
    object::objectPtr Array = insertObject("Array"_n, constructorCaller);
    (*Array)[n::classPrototype] = object::arrayPrototype;


    object::objectPtr ArrayView = insertObject(n::ArrayView, constructorCaller);
    arrayViewPrototype = object::makeEmptyObject();
    (*ArrayView)[n::classPrototype] = arrayViewPrototype;

    addFunctionL(Array, n::readOperator, {
        return object::makeObject(args);
    });

    addFunctionL(object::arrayPrototype, n::constructor, {
        thisObj->setType<object::type::Array>();
        return thisObj;
    });

    addFunctionL2(object::arrayPrototype, arrayViewPrototype, n::readOperator, {
        argsConvertibleGuard<number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::ArrayLike me = thisObj->getArrayLike();
        assert<std::greater_equal>(pos, 0);
        assert<std::less>(pos, me.size());
        return me[pos];
    });

    addFunctionL2(object::arrayPrototype, arrayViewPrototype, "length"_n, {
        return object::makeObject(static_cast<number>(thisObj->getArrayLike().size()));
    });

    addFunctionL(object::arrayPrototype, "insert"_n, {
        argsConvertibleGuard<number, nullptr_t>(args);
        object::type::Array &dest = thisObj->get<object::type::Array>();
        size_t destPos= static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        assert<std::greater_equal>(destPos, 0);
        assert<std::less_equal>(destPos, dest.size());
        dest.insert(dest.begin() + destPos, args[1]);
        return thisObj;
    });

    addFunctionL(object::arrayPrototype, "insertFrom"_n, {
        argsConvertibleGuard<number, object::ArrayLike>(args);
        object::type::Array &dest = thisObj->get<object::type::Array>();
        object::ArrayLike src = args[1]->getArrayLike();
        size_t destPos= static_cast<size_t>(args[0]->getConverted<object::type::Number>());
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

    addFunctionL(object::arrayPrototype, "append"_n, {
        argsConvertibleGuard<object::ArrayLike>(args);
        object::type::Array &dest = thisObj->get<object::type::Array>();
        object::ArrayLike src = args[0]->getArrayLike();
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

    addFunctionL2(object::arrayPrototype, arrayViewPrototype, "subarray"_n, {
        argsConvertibleGuard<number>(args);
        object::ArrayLike me = thisObj->getArrayLike();
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

    addFunctionL2(object::arrayPrototype, arrayViewPrototype, "subarrayView"_n, {
        argsConvertibleGuard<number>(args);
        object::ArrayLike me = thisObj->getArrayLike();
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

        auto newObj = object::makeEmptyObject();
        newObj->setType<object::type::ArrayView>();
        (*newObj)[n::prototype] = arrayViewPrototype;
        newObj->get<object::type::ArrayView>() = object::type::ArrayView(me.begin() + pos, me.begin() + pos + length);
        newObj->addProperty(n::__view, thisObj);

        return newObj;
    });

    addFunctionL(object::arrayPrototype, "erase"_n, {
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

    addFunctionL2(object::arrayPrototype, arrayViewPrototype, "forEach"_n, {
        argsConvertibleGuard<nullptr_t>(args);
        object::ArrayLike me = thisObj->getArrayLike();
        for (size_t i = 0; i < me.size(); i++)
            (*args[0])(args[0], {me[i], object::makeObject(static_cast<number>(i)), thisObj}, st);
        return thisObj;
    });

    addFunctionL(arrayViewPrototype, n::constructor, {
        argsConvertibleGuard<object::type::Array>(args);
        object::type::Array &src = args[0]->get<object::type::Array>();
        thisObj->setType<object::type::ArrayView>();
        thisObj->get<object::type::ArrayView>() = object::type::ArrayView(src.begin(), src.end());
        thisObj->addProperty(n::__view, args[0]);
        return thisObj;
    });
}