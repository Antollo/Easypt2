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
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int8_t>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<int8_t>(pos)));
    });

    addFunctionL(bufferPrototype, "getInt16"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int16_t>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<int16_t>(pos)));
    });

    addFunctionL(bufferPrototype, "getInt32"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int32_t>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<int32_t>(pos)));
    });

    addFunctionL(bufferPrototype, "getInt64"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int64_t>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<int64_t>(pos)));
    });

    addFunctionL(bufferPrototype, "getFloat"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<float>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<float>(pos)));
    });

    addFunctionL(bufferPrototype, "getDouble"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<double>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<double>(pos)));
    });

    addFunctionL(bufferPrototype, "setInt8"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int8_t>(pos);
        me->get<int8_t>(pos) = static_cast<int8_t>(args[1]->getConverted<object::type::Number>());
        return thisObj;
    });

    addFunctionL(bufferPrototype, "setInt16"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int16_t>(pos);
        me->get<int16_t>(pos) = static_cast<int16_t>(args[1]->getConverted<object::type::Number>());
        return thisObj;
    });

    addFunctionL(bufferPrototype, "setInt32"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int32_t>(pos);
        me->get<int32_t>(pos) = static_cast<int32_t>(args[1]->getConverted<object::type::Number>());
        return thisObj;
    });

    addFunctionL(bufferPrototype, "setInt64"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int64_t>(pos);
        me->get<int64_t>(pos) = static_cast<int64_t>(args[1]->getConverted<object::type::Number>());
        return thisObj;
    });

    addFunctionL(bufferPrototype, "setFloat"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<float>(pos);
        me->get<float>(pos) = static_cast<float>(args[1]->getConverted<object::type::Number>());
        return thisObj;
    });

    addFunctionL(bufferPrototype, "setDouble"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<double>(pos);
        me->get<double>(pos) = static_cast<double>(args[1]->getConverted<object::type::Number>());
        return thisObj;
    });

    addFunctionL(bufferPrototype, "toString"_n, {
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        return object::makeObject(std::string(reinterpret_cast<const char*>(me->begin()), reinterpret_cast<const char*>(me->end())));
    });

    addFunctionL(bufferPrototype, "toNumber"_n, {
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        me->assert<int8_t>(0);
        switch (me->elementType())
        {
        case buffer::type::Int8:
            return object::makeObject(static_cast<number>(me->get<int8_t>(0)));
        case buffer::type::Int16:
            return object::makeObject(static_cast<number>(me->get<int16_t>(0)));
        case buffer::type::Int32:
            return object::makeObject(static_cast<number>(me->get<int32_t>(0)));
        case buffer::type::Int64:
            return object::makeObject(static_cast<number>(me->get<int64_t>(0)));
        case buffer::type::Float:
            return object::makeObject(static_cast<number>(me->get<float>(0)));
        case buffer::type::Double:
            return object::makeObject(static_cast<number>(me->get<double>(0)));
        
        default:
            throw std::runtime_error("unknow type of Buffer, cannot convert to Number");
            return object::makeEmptyObject();
        }
    });

    addFunctionL(bufferPrototype, "type"_n, {
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        switch (me->elementType())
        {
        case buffer::type::Int8:
            return object::makeObject(std::string("Int8"));
        case buffer::type::Int16:
            return object::makeObject(std::string("Int16"));
        case buffer::type::Int32:
            return object::makeObject(std::string("Int32"));
        case buffer::type::Int64:
            return object::makeObject(std::string("Int64"));
        case buffer::type::Float:
            return object::makeObject(std::string("Float"));
        case buffer::type::Double:
            return object::makeObject(std::string("Double"));
        default:
            return object::makeObject(std::string("Unknown"));
        }
    });

    addFunctionL(bufferPrototype, "allocateInt8"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<int8_t>(size);
        return thisObj;
    });

    addFunctionL(bufferPrototype, "allocateInt16"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<int16_t>(size);
        return thisObj;
    });

    addFunctionL(bufferPrototype, "allocateInt32"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<int32_t>(size);
        return thisObj;
    });

    addFunctionL(bufferPrototype, "allocateInt64"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<int64_t>(size);
        return thisObj;
    });

    addFunctionL(bufferPrototype, "allocateFloat"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<float>(size);
        return thisObj;
    });

    addFunctionL(bufferPrototype, "allocateDouble"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<double>(size);
        return thisObj;
    });

    addFunctionL(bufferPrototype, "reallocate"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->reallocate(size);
        return thisObj;
    });

    addFunctionL(bufferPrototype, "size"_n, {
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        return object::makeObject(static_cast<object::type::Number>(me->size()));
    });

    addFunctionL(bufferPrototype, "length"_n, {
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        return object::makeObject(static_cast<object::type::Number>(me->length()));
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
        auto  arr = args[0]->getConverted<object::type::Array>();
        buffer b;
        b.allocate<int8_t>(arr.size());
        for (size_t i = 0; i < arr.size(); i++)
        {
            assert(arr[i]->isOfType<object::type::Number>(), "element is not a number");
            b.get<int8_t>(i) = static_cast<int8_t>(arr[i]->get<const object::type::Number>());
        }
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(std::move(b));
        return newObj;
    });

    addFunctionL(Buffer, "fromInt16Array"_n, {
        argsConvertibleGuard<object::type::Array>(args);
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        auto  arr = args[0]->getConverted<object::type::Array>();
        buffer b;
        b.allocate<int16_t>(arr.size());
        for (size_t i = 0; i < arr.size(); i++)
        {
            assert(arr[i]->isOfType<object::type::Number>(), "element is not a number");
            b.get<int16_t>(i) = static_cast<int16_t>(arr[i]->get<const object::type::Number>());
        }
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(std::move(b));
        return newObj;
    });

    addFunctionL(Buffer, "fromInt32Array"_n, {
        argsConvertibleGuard<object::type::Array>(args);
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        auto  arr = args[0]->getConverted<object::type::Array>();
        buffer b;
        b.allocate<int32_t>(arr.size());
        for (size_t i = 0; i < arr.size(); i++)
        {
            assert(arr[i]->isOfType<object::type::Number>(), "element is not a number");
            b.get<int32_t>(i) = static_cast<int32_t>(arr[i]->get<const object::type::Number>());
        }
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(std::move(b));
        return newObj;
    });

    addFunctionL(Buffer, "fromInt64Array"_n, {
        argsConvertibleGuard<object::type::Array>(args);
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        auto  arr = args[0]->getConverted<object::type::Array>();
        buffer b;
        b.allocate<int64_t>(arr.size());
        for (size_t i = 0; i < arr.size(); i++)
        {
            assert(arr[i]->isOfType<object::type::Number>(), "element is not a number");
            b.get<int64_t>(i) = static_cast<int64_t>(arr[i]->get<const object::type::Number>());
        }
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(std::move(b));
        return newObj;
    });

    addFunctionL(Buffer, "fromFloatArray"_n, {
        argsConvertibleGuard<object::type::Array>(args);
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        auto  arr = args[0]->getConverted<object::type::Array>();
        buffer b;
        b.allocate<float>(arr.size());
        for (size_t i = 0; i < arr.size(); i++)
            b.get<float>(i) = static_cast<float>(arr[i]->get<const object::type::Number>());
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(std::move(b));
        return newObj;
    });

    addFunctionL(Buffer, "fromDoubleArray"_n, {
        argsConvertibleGuard<object::type::Array>(args);
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        auto  arr = args[0]->getConverted<object::type::Array>();
        buffer b;
        b.allocate<double>(arr.size());
        for (size_t i = 0; i < arr.size(); i++)
            b.get<double>(i) = static_cast<double>(arr[i]->get<const object::type::Number>());
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(std::move(b));
        return newObj;
    });
}