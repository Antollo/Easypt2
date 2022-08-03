#include "../library.h"
#include "../buffer.h"

void Buffer::init(stack *st)
{
    object::objectPtr Buffer = insertObject(n::Buffer, constructorCaller);
    object::objectPtr bufferPrototype = object::makeEmptyObject();

    (*Buffer)[n::classPrototype] = bufferPrototype;

    bufferPrototype->addFunctionL(n::constructor, {
        thisObj->setType<object::typeIndex::Buffer>();
        thisObj->get<object::type::Buffer>() = std::make_shared<buffer>();
        return thisObj;
    });

    bufferPrototype->addFunctionL(n::readOperator, {
        argsConvertibleGuard<number>(args);
        int64_t pos = static_cast<int64_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        negativeIndexing(pos, *me);
        assert<std::greater_equal>(pos, 0);

        switch (me->elementType())
        {
        case buffer::type::Int8:
            me->assert<int8_t>(pos);
            return object::makeObject(static_cast<object::type::Number>(me->get<int8_t>(pos)));
        case buffer::type::Int16:
            me->assert<int16_t>(pos);
            return object::makeObject(static_cast<object::type::Number>(me->get<int16_t>(pos)));
        case buffer::type::Int32:
            me->assert<int32_t>(pos);
            return object::makeObject(static_cast<object::type::Number>(me->get<int32_t>(pos)));
        case buffer::type::Int64:
            me->assert<int64_t>(pos);
            return object::makeObject(static_cast<object::type::Number>(me->get<int64_t>(pos)));
        case buffer::type::Uint8:
            me->assert<uint8_t>(pos);
            return object::makeObject(static_cast<object::type::Number>(me->get<uint8_t>(pos)));
        case buffer::type::Uint16:
            me->assert<uint16_t>(pos);
            return object::makeObject(static_cast<object::type::Number>(me->get<uint16_t>(pos)));
        case buffer::type::Uint32:
            me->assert<uint32_t>(pos);
            return object::makeObject(static_cast<object::type::Number>(me->get<uint32_t>(pos)));
        case buffer::type::Uint64:
            me->assert<uint64_t>(pos);
            return object::makeObject(static_cast<object::type::Number>(me->get<uint64_t>(pos)));
        case buffer::type::Float:
            me->assert<float>(pos);
            return object::makeObject(static_cast<object::type::Number>(me->get<float>(pos)));
        case buffer::type::Double:
            me->assert<double>(pos);
            return object::makeObject(static_cast<object::type::Number>(me->get<double>(pos)));
        
        default:
            throw std::runtime_error("unknow type of Buffer, cannot convert to Number");
        }
    });

    bufferPrototype->addFunctionL("getInt8"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int8_t>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<int8_t>(pos)));
    });

    bufferPrototype->addFunctionL("getInt16"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int16_t>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<int16_t>(pos)));
    });

    bufferPrototype->addFunctionL("getInt32"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int32_t>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<int32_t>(pos)));
    });

    bufferPrototype->addFunctionL("getInt64"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int64_t>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<int64_t>(pos)));
    });

    bufferPrototype->addFunctionL("getFloat"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<float>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<float>(pos)));
    });

    bufferPrototype->addFunctionL("getDouble"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<double>(pos);
        return object::makeObject(static_cast<object::type::Number>(me->get<double>(pos)));
    });

    bufferPrototype->addFunctionL("setInt8"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int8_t>(pos);
        me->get<int8_t>(pos) = static_cast<int8_t>(args[1]->getConverted<object::type::Number>());
        return thisObj;
    });

    bufferPrototype->addFunctionL("setInt16"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int16_t>(pos);
        me->get<int16_t>(pos) = static_cast<int16_t>(args[1]->getConverted<object::type::Number>());
        return thisObj;
    });

    bufferPrototype->addFunctionL("setInt32"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int32_t>(pos);
        me->get<int32_t>(pos) = static_cast<int32_t>(args[1]->getConverted<object::type::Number>());
        return thisObj;
    });

    bufferPrototype->addFunctionL("setInt64"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<int64_t>(pos);
        me->get<int64_t>(pos) = static_cast<int64_t>(args[1]->getConverted<object::type::Number>());
        return thisObj;
    });

    bufferPrototype->addFunctionL("setFloat"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<float>(pos);
        me->get<float>(pos) = static_cast<float>(args[1]->getConverted<object::type::Number>());
        return thisObj;
    });

    bufferPrototype->addFunctionL("setDouble"_n, {
        argsConvertibleGuard<object::type::Number, object::type::Number>(args);
        size_t pos = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        assert<std::greater_equal>(pos, 0);
        me->assert<double>(pos);
        me->get<double>(pos) = static_cast<double>(args[1]->getConverted<object::type::Number>());
        return thisObj;
    });

    bufferPrototype->addFunctionL("toString"_n, {
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        return object::makeObject(std::string(reinterpret_cast<const char*>(me->begin()), reinterpret_cast<const char*>(me->end())));
    });

    bufferPrototype->addFunctionL("toArray"_n, {
        const buffer &me = *thisObj->get<const object::type::Buffer>();
        size_t size = me.length();
        object::type::Array arr;
        arr.reserve(size);

        switch (me.elementType())
        {
        default:
        case buffer::type::Int8:
            for (auto element : me.asSpan<int8_t>())
                arr.emplace_back(object::makeObject(number(element)));
            break;

        case buffer::type::Int16:
            for (auto element : me.asSpan<int16_t>())
                arr.emplace_back(object::makeObject(number(element)));
            break;

        case buffer::type::Int32:
            for (auto element : me.asSpan<int32_t>())
                arr.emplace_back(object::makeObject(number(element)));
            break;

        case buffer::type::Int64:
            for (auto element : me.asSpan<int64_t>())
                arr.emplace_back(object::makeObject(number(element)));
            break;

        case buffer::type::Uint8:
            for (auto element : me.asSpan<uint8_t>())
                arr.emplace_back(object::makeObject(number(element)));
            break;

        case buffer::type::Uint16:
            for (auto element : me.asSpan<uint16_t>())
                arr.emplace_back(object::makeObject(number(element)));
            break;

        case buffer::type::Uint32:
            for (auto element : me.asSpan<uint32_t>())
                arr.emplace_back(object::makeObject(number(element)));
            break;

        case buffer::type::Uint64:
            for (auto element : me.asSpan<uint64_t>())
                arr.emplace_back(object::makeObject(number(element)));
            break;

        case buffer::type::Float:
            for (auto element : me.asSpan<float>())
                arr.emplace_back(object::makeObject(number(element)));
            break;

        case buffer::type::Double:
            for (auto element : me.asSpan<double>())
                arr.emplace_back(object::makeObject(number(element)));
            break;
        }

        return object::makeObject(arr);
    });

    bufferPrototype->addFunctionL("toNumber"_n, {
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
        case buffer::type::Uint8:
            return object::makeObject(static_cast<number>(me->get<uint8_t>(0)));
        case buffer::type::Uint16:
            return object::makeObject(static_cast<number>(me->get<uint16_t>(0)));
        case buffer::type::Uint32:
            return object::makeObject(static_cast<number>(me->get<uint32_t>(0)));
        case buffer::type::Uint64:
            return object::makeObject(static_cast<number>(me->get<uint64_t>(0)));
        case buffer::type::Float:
            return object::makeObject(static_cast<number>(me->get<float>(0)));
        case buffer::type::Double:
            return object::makeObject(static_cast<number>(me->get<double>(0)));
        
        default:
            throw std::runtime_error("unknow type of Buffer, cannot convert to Number");
        }
    });

    bufferPrototype->addFunctionL("type"_n, {
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
        case buffer::type::Uint8:
            return object::makeObject(std::string("Uint8"));
        case buffer::type::Uint16:
            return object::makeObject(std::string("Uint16"));
        case buffer::type::Uint32:
            return object::makeObject(std::string("Uint32"));
        case buffer::type::Uint64:
            return object::makeObject(std::string("Uint64"));
        case buffer::type::Float:
            return object::makeObject(std::string("Float"));
        case buffer::type::Double:
            return object::makeObject(std::string("Double"));
        default:
            return object::makeObject(std::string("Unknown"));
        }
    });

    bufferPrototype->addFunctionL("allocateInt8"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<int8_t>(size);
        return thisObj;
    });

    bufferPrototype->addFunctionL("allocateInt16"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<int16_t>(size);
        return thisObj;
    });

    bufferPrototype->addFunctionL("allocateInt32"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<int32_t>(size);
        return thisObj;
    });

    bufferPrototype->addFunctionL("allocateInt64"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<int64_t>(size);
        return thisObj;
    });

    bufferPrototype->addFunctionL("allocateUint8"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<uint8_t>(size);
        return thisObj;
    });

    bufferPrototype->addFunctionL("allocateUint16"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<uint16_t>(size);
        return thisObj;
    });

    bufferPrototype->addFunctionL("allocateUint32"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<uint32_t>(size);
        return thisObj;
    });

    bufferPrototype->addFunctionL("allocateUint64"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<uint64_t>(size);
        return thisObj;
    });

    bufferPrototype->addFunctionL("allocateFloat"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<float>(size);
        return thisObj;
    });

    bufferPrototype->addFunctionL("allocateDouble"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->allocate<double>(size);
        return thisObj;
    });

    bufferPrototype->addFunctionL("reallocate"_n, {
        argsConvertibleGuard<object::type::Number>(args);
        size_t size = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        object::type::Buffer &me = thisObj->get<object::type::Buffer>();
        me->reallocate(size);
        return thisObj;
    });

    bufferPrototype->addFunctionL("size"_n, {
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        return object::makeObject(static_cast<object::type::Number>(me->size()));
    });

    bufferPrototype->addFunctionL("length"_n, {
        const object::type::Buffer &me = thisObj->get<const object::type::Buffer>();
        return object::makeObject(static_cast<object::type::Number>(me->length()));
    });

    Buffer->addFunctionL("fromString"_n, {
        argsGuard<object::type::String>(args);
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(args[0]->get<const object::type::String>());
        return newObj;
    });

    Buffer->addFunctionL("fromInt8Array"_n, {
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

    Buffer->addFunctionL("fromInt16Array"_n, {
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

    Buffer->addFunctionL("fromInt32Array"_n, {
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

    Buffer->addFunctionL("fromInt64Array"_n, {
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

    Buffer->addFunctionL("fromUint8Array"_n, {
        argsConvertibleGuard<object::type::Array>(args);
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        auto  arr = args[0]->getConverted<object::type::Array>();
        buffer b;
        b.allocate<uint8_t>(arr.size());
        for (size_t i = 0; i < arr.size(); i++)
        {
            assert(arr[i]->isOfType<object::type::Number>(), "element is not a number");
            b.get<uint8_t>(i) = static_cast<uint8_t>(arr[i]->get<const object::type::Number>());
        }
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(std::move(b));
        return newObj;
    });

    Buffer->addFunctionL("fromUint16Array"_n, {
        argsConvertibleGuard<object::type::Array>(args);
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        auto  arr = args[0]->getConverted<object::type::Array>();
        buffer b;
        b.allocate<uint16_t>(arr.size());
        for (size_t i = 0; i < arr.size(); i++)
        {
            assert(arr[i]->isOfType<object::type::Number>(), "element is not a number");
            b.get<uint16_t>(i) = static_cast<uint16_t>(arr[i]->get<const object::type::Number>());
        }
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(std::move(b));
        return newObj;
    });

    Buffer->addFunctionL("fromUint32Array"_n, {
        argsConvertibleGuard<object::type::Array>(args);
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        auto  arr = args[0]->getConverted<object::type::Array>();
        buffer b;
        b.allocate<uint32_t>(arr.size());
        for (size_t i = 0; i < arr.size(); i++)
        {
            assert(arr[i]->isOfType<object::type::Number>(), "element is not a number");
            b.get<uint32_t>(i) = static_cast<uint32_t>(arr[i]->get<const object::type::Number>());
        }
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(std::move(b));
        return newObj;
    });

    Buffer->addFunctionL("fromUint64Array"_n, {
        argsConvertibleGuard<object::type::Array>(args);
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        auto  arr = args[0]->getConverted<object::type::Array>();
        buffer b;
        b.allocate<uint64_t>(arr.size());
        for (size_t i = 0; i < arr.size(); i++)
        {
            assert(arr[i]->isOfType<object::type::Number>(), "element is not a number");
            b.get<uint64_t>(i) = static_cast<uint64_t>(arr[i]->get<const object::type::Number>());
        }
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(std::move(b));
        return newObj;
    });

    Buffer->addFunctionL("fromFloatArray"_n, {
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

    Buffer->addFunctionL("fromDoubleArray"_n, {
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