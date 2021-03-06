#include "../library.h"
#include "../buffer.h"

void Buffer::init(stack *st)
{
    object::objectPtr Buffer = insertObject(n::Buffer, constructorCaller);
    object::objectPtr bufferPrototype = object::makeEmptyObject();

    (*Buffer)[n::classPrototype] = bufferPrototype;

    addFunctionL(bufferPrototype, n::constructor, {
        thisObj->setType<object::type::Buffer>();
        thisObj->get<object::type::Buffer>() = std::make_shared<buffer>();
        return thisObj;
    });

    addFunctionL(bufferPrototype, "getInt8"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        int pos = static_cast<int>(args[0].getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int8_t>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<int8_t>(pos)));
    });

    addFunctionL(bufferPrototype, "getInt16"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        int pos = static_cast<int>(args[0].getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int16_t>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<int16_t>(pos)));
    });

    addFunctionL(bufferPrototype, "getInt32"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        int pos = static_cast<int>(args[0].getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int32_t>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<int32_t>(pos)));
    });

    addFunctionL(bufferPrototype, "getFloat"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        int pos = static_cast<int>(args[0].getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<float>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<float>(pos)));
    });

    addFunctionL(bufferPrototype, "getDouble"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        int pos = static_cast<int>(args[0].getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<double>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<double>(pos)));
    });

    addFunctionL(bufferPrototype, "setInt8"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        int pos = static_cast<int>(args[0].getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int8_t>(pos);
        me->get<int8_t>(pos) = static_cast<int>(args[1].getConverted<object::type::Number>());
        return thisObj;
    });

    addFunctionL(bufferPrototype, "setInt16"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        int pos = static_cast<int>(args[0].getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int16_t>(pos);
        me->get<int16_t>(pos) = static_cast<int>(args[1].getConverted<object::type::Number>());
        return thisObj;
    });

    addFunctionL(bufferPrototype, "setInt32"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        int pos = static_cast<int>(args[0].getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int32_t>(pos);
        me->get<int32_t>(pos) = static_cast<int>(args[1].getConverted<object::type::Number>());
        return thisObj;
    });

    addFunctionL(bufferPrototype, "setFloat"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        int pos = static_cast<int>(args[0].getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<float>(pos);
        me->get<float>(pos) = static_cast<double>(args[1].getConverted<object::type::Number>());
        return thisObj;
    });

    addFunctionL(bufferPrototype, "setDouble"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        int pos = static_cast<int>(args[0].getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<double>(pos);
        me->get<double>(pos) = static_cast<double>(args[1].getConverted<object::type::Number>());
        return thisObj;
    });

    addFunctionL(bufferPrototype, "toString"_n, {
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        return object::makeObject(std::string(reinterpret_cast<const char*>(me->begin()), reinterpret_cast<const char*>(me->end())));
    });

    addFunctionL(bufferPrototype, "allocate"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        int size = static_cast<int>(args[0].getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<char>(size);
        return thisObj;
    });

    addFunctionL(bufferPrototype, "reallocate"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        int size = static_cast<int>(args[0].getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->reallocate<char>(size);
        return thisObj;
    });

    addFunctionL(bufferPrototype, "size"_n, {
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        return object::makeObject(static_cast<object::type::Number>(me->size()));
    });

    addFunctionL(Buffer, "fromString"_n, {
        argsGuard<object::type::String>(args);
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(args[0]->get<const object::type::String>());
        return newObj;
    });

    addFunctionL(Buffer, "fromInt8Array"_n, {
        argsConvertibleGuard<object::type::Array>(args);
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        auto  arr = args[0].getConverted<object::type::Array>();
        buffer b;
        b.allocate<int8_t>(arr.size());
        for (size_t i = 0; i < arr.size(); i++)
        {
            assert(arr[i]->isOfType<object::type::Number>(), "element is not a number");
            b.get<int8_t>(i) = static_cast<int>(arr[i]->get<const object::type::Number>());
        }
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(std::move(b));
        return newObj;
    });

    addFunctionL(Buffer, "fromInt16Array"_n, {
        argsConvertibleGuard<object::type::Array>(args);
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        auto  arr = args[0].getConverted<object::type::Array>();
        buffer b;
        b.allocate<int16_t>(arr.size());
        for (size_t i = 0; i < arr.size(); i++)
        {
            assert(arr[i]->isOfType<object::type::Number>(), "element is not a number");
            b.get<int16_t>(i) = static_cast<int>(arr[i]->get<const object::type::Number>());
        }
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(std::move(b));
        return newObj;
    });

    addFunctionL(Buffer, "fromInt32Array"_n, {
        argsConvertibleGuard<object::type::Array>(args);
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        auto  arr = args[0].getConverted<object::type::Array>();
        buffer b;
        b.allocate<int32_t>(arr.size());
        for (size_t i = 0; i < arr.size(); i++)
        {
            assert(arr[i]->isOfType<object::type::Number>(), "element is not a number");
            b.get<int32_t>(i) = static_cast<int>(arr[i]->get<const object::type::Number>());
        }
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(std::move(b));
        return newObj;
    });

    addFunctionL(Buffer, "fromFloatArray"_n, {
        argsConvertibleGuard<object::type::Array>(args);
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        auto  arr = args[0].getConverted<object::type::Array>();
        buffer b;
        b.allocate<float>(arr.size());
        for (size_t i = 0; i < arr.size(); i++)
            b.get<float>(i) = static_cast<double>(arr[i]->get<const object::type::Number>());
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(std::move(b));
        return newObj;
    });

    addFunctionL(Buffer, "fromDoubleArray"_n, {
        argsConvertibleGuard<object::type::Array>(args);
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        auto  arr = args[0].getConverted<object::type::Array>();
        buffer b;
        b.allocate<double>(arr.size());
        for (size_t i = 0; i < arr.size(); i++)
            b.get<double>(i) = static_cast<double>(arr[i]->get<const object::type::Number>());
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(std::move(b));
        return newObj;
    });
}