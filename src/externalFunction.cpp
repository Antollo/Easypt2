#include "externalFunction.h"
#include "nobject.h"

object::objectPtr externalFunction::operator()(object::objectPtr thisObj, object::type::Array &&args, stack *st)
{
    int8_t argType, i = 0, returnType = _type & 7;
    int64_t type = _type >> 3;
    std::vector<void *> ptrArgs;
    std::list<number> numbers;
    std::list<buffer> buffers;
    while (type)
    {
        if (i >= args.size())
            throw std::runtime_error("wrong number of arguments");
        switch (type & 3)
        {
        case bufferType:
        case pointerAsBufferType:
            if (args[i]->isOfType<object::type::Buffer>())
                ptrArgs.push_back(&args[i]->uncheckedGet<object::type::Buffer>()->get<char>(0));
            else
                throw std::runtime_error("wrong type of argument, expected Buffer");
            break;

        case stringType:
            if (args[i]->isOfType<object::type::String>())
                ptrArgs.push_back(args[i]->uncheckedGet<object::type::String>().data());
            else if (args[i]->isOfType<object::type::Buffer>())
                ptrArgs.push_back(&args[i]->uncheckedGet<object::type::Buffer>()->get<char>(0));
            else
                throw std::runtime_error("wrong type of argument, expected Buffer or String");
            break;

        case numberType:
        {
            numbers.push_back(args[i].getConverted<object::type::Number>());
            //console::debug(static_cast<int64_t>(numbers.back()));
            ptrArgs.push_back(&numbers.back());
            break;
        }

        case arrayType:
        {
            buffers.emplace_back();
            auto array = args[i].getConverted<object::type::Array>();
            buffers.back().allocate<void*>(array.size());
            for (size_t i = 0; i < array.size(); i++)
            {
                if (array[i]->isOfType<object::type::Buffer>())
                    buffers.back().get<void*>(i) = array[i]->uncheckedGet<object::type::Buffer>()->begin();
                else
                    throw std::runtime_error("wrong type of argument, expected Array of Buffers");
            }
            ptrArgs.push_back(buffers.back().begin());
            break;
        }

        default:
            throw std::runtime_error("unsupported type of argument");
            break;
        }
        type >>= 3;
        i++;
    }
    if (i < args.size())
        throw std::runtime_error("wrong number of arguments");

    void *result = reinterpret_cast<void *(*)(void **)>(_functionPtr)(ptrArgs.data());
    switch (returnType)
    {
    case bufferType:
        console::warn("Buffer is not currently supported as return value from external function");
        return thisObj;

    case pointerAsBufferType:
    {
        auto Buffer = (*st)[n::Buffer];
        auto newObj = (*Buffer)(Buffer, {}, st);
        buffer b;
        b.allocate<void *>(1);
        b.get<void *>(0) = result ? *reinterpret_cast<void **>(result) : nullptr;
        newObj->get<object::type::Buffer>() = std::make_shared<buffer>(std::move(b));
        return newObj;
    }

    case stringType:
        return object::makeObject(std::string(*reinterpret_cast<const char **>(result)));

    case numberType:
        return object::makeObject(*reinterpret_cast<number *>(result));

    case voidType:
        return thisObj;

    default:
        throw std::runtime_error("unsupported type of return value");
        break;
    }

    return object::makeEmptyObject();
}
