#ifndef EC156D2C_842B_4A3D_A420_CD0530F231DA
#define EC156D2C_842B_4A3D_A420_CD0530F231DA

#include "externalLibraryCommon.h"

template <class T, class V = void>
struct type
{
    static constexpr int8_t id = errorType;
    static auto fromVoidPtr(void *ptr)
    {
        std::cout << "error: type::fromVoidPtr" << std::endl;
        return *reinterpret_cast<T *>(ptr);
    }
};

template <>
struct type<void>
{
    static constexpr int8_t id = voidType;
};

template <class T>
struct type<T *, std::enable_if_t<std::is_arithmetic_v<T> && sizeof(T) != 1>>
{
    static constexpr int8_t id = bufferType;
    static T *fromVoidPtr(void *ptr)
    {
        return reinterpret_cast<T *>(ptr);
    }
};

template <class T>
struct type<T *, std::enable_if_t<std::is_void_v<T>>>
{
    static constexpr int8_t id = bufferType;
    static T *fromVoidPtr(void *ptr)
    {
        return reinterpret_cast<T *>(ptr);
    }
};

template <class T>
struct type<T *, std::enable_if_t<std::is_arithmetic_v<T> && sizeof(T) == 1>>
{
    static constexpr int8_t id = stringType;
    static T *fromVoidPtr(void *ptr)
    {
        return reinterpret_cast<T *>(ptr);
    }
};

template <class T>
struct type<T *, std::enable_if_t<!std::is_arithmetic_v<T> && !std::is_pointer_v<T> && !std::is_void_v<T>>>
{
    static constexpr int8_t id = pointerAsBufferType;
    static T *fromVoidPtr(void *ptr)
    {
        return ptr ? *reinterpret_cast<T **>(ptr) : nullptr;
    }
};

template <class T>
struct type<T **>
{
    static constexpr int8_t id = arrayType;
    static T **fromVoidPtr(void *ptr)
    {
        return reinterpret_cast<T **>(ptr);
    }
};

template <class T>
struct type<T, std::enable_if_t<std::is_arithmetic_v<T>>>
{
    static constexpr int8_t id = numberType;
    static T fromVoidPtr(void *ptr)
    {
        return static_cast<T>(*reinterpret_cast<number *>(ptr));
    }
};

template <class T>
inline constexpr auto typeId = type<T>::id;

template <class R, class... Args, std::size_t... Is>
inline int64_t functionTypeIdHelper(R (*f)(Args...), std::index_sequence<Is...> = std::index_sequence_for<Args...>())
{
    int64_t type = 0;
    //std::cout << (int)typeId<R> << ' ';
    //(std::cout << ... << (int)typeId<Args>) << std::endl;

    ((type = (type | int64_t(typeId<Args> << (Is * 3)))), ...);
    type <<= 3;
    type |= typeId<R>;
    return type;
}

template <class R, class... Args>
inline int64_t functionTypeId(R (*f)(Args...))
{
    return functionTypeIdHelper(f, std::index_sequence_for<Args...>());
}

template <class R, class... Args, std::size_t... Is>
void *call(R (*f)(Args...), void **args, std::index_sequence<Is...>)
{
    //(std::cout << ... << typeid(Args).name()) << std::endl;
    if constexpr (std::is_same_v<void, R>)
    {
        f((type<Args>::fromVoidPtr(args[Is]))...);
        return nullptr;
    }
    else if constexpr (std::is_same_v<R, unsigned long>)
    {
        static number result;
        result = static_cast<uint64_t>(f((type<Args>::fromVoidPtr(args[Is]))...));
        return &result;
    }
    else if constexpr (std::is_arithmetic_v<R>)
    {
        static number result;
        result = f((type<Args>::fromVoidPtr(args[Is]))...);
        return &result;
    }
    else
    {
        static R result;
        result = f((type<Args>::fromVoidPtr(args[Is]))...);
        //std::cout << "call " << result << std::endl;
        return &result;
    }
}

template <class R, class... Args>
void *function(R (*f)(Args...), void **args)
{
    //(std::cout << ... << typeid(Args).name()) << std::endl;
    return call(f, args, std::index_sequence_for<Args...>());
}

#define registerConstantM(x) if constexpr(std::is_integral_v<decltype(x)>) registerIntegerConstant(#x, x); else registerDoubleConstant(#x, x);
#define registerFunctionM(x) \
    registerFunction(#x, reinterpret_cast<void *>(static_cast<void *(*)(void **)>([](void **args) -> void * { return function(x, args); })), functionTypeId(x))

#endif /* EC156D2C_842B_4A3D_A420_CD0530F231DA */
