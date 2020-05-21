#ifndef NOBJECT_H
#define NOBJECT_H

#include <any>
#include <unordered_map>
#include <vector>
#include <type_traits>
#include <cstddef>
#include <array>
#include "objectPtrImpl.h"
#include "name.h"
#include "stack.h"
#include "number.h"
//#include "statement.h"
#include "Node.h"
#include "promise.h"
#include "file.h"

template <class T>
struct always_false : std::false_type
{
};

template <class T>
using remove_cref_t = std::remove_const_t<std::remove_reference_t<T>>;

objectPtrImpl constructorCaller(objectPtrImpl thisObj, std::vector<objectPtrImpl> &&args, stack *st);
class objectMemory;

class object
{
public:
    using objectPtr = objectPtrImpl;
    using arrayType = std::vector<objectPtr>;
    using nativeFunctionType = objectPtr (*)(objectPtr, arrayType &&, stack *);
    using propertiesType = std::unordered_map<name, objectPtr>;
    using objectPromise = promise<objectPtr>;
    using functionType = Node;

    static functionType makeFunction()
    {
        return Node(0, "root");
    }

    template <class T>
    static objectPtr makeObject(T &&t)
    {
        if (objects.length == 0)
        {
            //console::log("makeObject empty");
            return objectPtr(new object(std::forward<T>(t)));
        }

        object *ptr = objects.data[objects.tail++];
        if (objects.tail == buffer::maxLength)
            objects.tail = 0;
        objects.length--;
        ptr->_value = std::forward<T>(t);
        if constexpr (std::is_same_v<std::decay_t<T>, nativeFunctionType>)
            ptr->init<T>(t == constructorCaller);
        else
            ptr->init<T>();
        return ptr;
    }

    static objectPtr
    makeEmptyObject()
    {
        return makeObject(nullptr);
    }

    void *operator new(size_t size);
    void operator delete(void *p);

    template <class T>
    explicit object(T &&t) : _value(std::forward<T>(t)), _isConst(false)
    {
        if constexpr (std::is_same_v<std::decay_t<T>, nativeFunctionType>)
            init<T>(t == constructorCaller);
        else
            init<T>();
    }

    template <class T>
    void init(bool isConstructorCaller = false)
    {
        if constexpr (std::is_same_v<std::decay_t<T>, nullptr_t>)
            _prototype = objectPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, number>)
            _prototype = numberPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, std::string>)
            _prototype = stringPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, arrayType>)
            _prototype = arrayPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, bool>)
            _prototype = booleanPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, objectPromise::promisePtr>)
            _prototype = promisePrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, functionType> || std::is_same_v<std::decay_t<T>, nativeFunctionType>)
        {
            if constexpr (std::is_same_v<std::decay_t<T>, nativeFunctionType>)
            {
                if (isConstructorCaller)
                    _prototype = classPrototype;
                else
                    _prototype = functionPrototype;
            }
            else
                _prototype = functionPrototype;
        }
        else
            throw std::runtime_error("wrong type "s + typeid(T).name());
    }

    object(object &&) = default;
    object &operator=(object &&) = default;
    object(const object &) = default;
    object &operator=(const object &) = default;

    template <class T>
    bool isOfType() const
    {
        return typeid(remove_cref_t<T>).hash_code() == getTypeHashCode();
    }

    template <class T>
    void setType()
    {
        _value.emplace<remove_cref_t<T>>();
    }

    template <class T>
    std::remove_reference_t<T> &get()
    {
        if constexpr (std::is_const_v<T>)
        {
            return std::get<remove_cref_t<T>>(_value);
        }
        else
        {
            if (_isConst)
                throw std::runtime_error("tried to modify constant value");
            return std::get<remove_cref_t<T>>(_value);
        }
    }

    template <class T>
    const remove_cref_t<T> &get() const
    {
        return std::get<remove_cref_t<T>>(_value);
    }

    template <class T>
    bool isConvertible() const
    {
        if constexpr (std::is_same_v<remove_cref_t<T>, bool>)
            if (isOfType<nullptr_t>() || isOfType<functionType>() || isOfType<nativeFunctionType>())
                return true;
        if constexpr (std::is_same_v<remove_cref_t<T>, number> || std::is_same_v<remove_cref_t<T>, std::string> || std::is_same_v<remove_cref_t<T>, arrayType> || std::is_same_v<remove_cref_t<T>, bool>)
        {
            return isOfType<number>() || isOfType<std::string>() || isOfType<arrayType>() || isOfType<bool>();
        }
        // TODO user conversions
        else
            return false;
    }

    template <class T>
    remove_cref_t<T> getConverted()
    {
        return std::visit([this](auto &&value) -> remove_cref_t<T> {
            using A = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<remove_cref_t<T>, A>)
                return value;

            else if constexpr (std::is_same_v<remove_cref_t<T>, number>)
            {
                if constexpr (std::is_same_v<A, std::string>)
                    return static_cast<number>(value);
                else if constexpr (std::is_same_v<A, arrayType>)
                    return static_cast<number>(value.size());
                else if constexpr (std::is_same_v<A, bool>)
                    return static_cast<number>(value);
                /*else
                {
                    auto converter = read("number"_n);
                    if (converter)
                        return (*converter)(thisObj, {}, nullptr);
                }*/
            }

            else if constexpr (std::is_same_v<remove_cref_t<T>, std::string>)
            {
                if constexpr (std::is_same_v<A, number>)
                    return static_cast<std::string>(value);
                else if constexpr (std::is_same_v<A, arrayType>)
                    return "["s + std::to_string(value.size()) + "]"s;
                else if constexpr (std::is_same_v<A, bool>)
                    return value ? "true"s : "false"s;
                /*else
                {
                    auto converter = read("string"_n);
                    if (converter)
                        return (*converter)(thisObj, {}, nullptr);
                }*/
            }

            else if constexpr (std::is_same_v<remove_cref_t<T>, arrayType>)
            {
                if constexpr (std::is_same_v<A, number>)
                    return arrayType{object::makeObject(value)};
                else if constexpr (std::is_same_v<A, std::string>)
                    return arrayType{object::makeObject(value)};
                else if constexpr (std::is_same_v<A, bool>)
                    return arrayType{object::makeObject(value)};
                /*else
                {
                    auto converter = read("array"_n);
                    if (converter)
                        return (*converter)(thisObj, {}, nullptr);
                }*/
            }

            else if constexpr (std::is_same_v<remove_cref_t<T>, bool>)
            {
                if constexpr (std::is_same_v<A, number>)
                    return static_cast<bool>(value);
                else if constexpr (std::is_same_v<A, std::string>)
                    return static_cast<bool>(value.size());
                else if constexpr (std::is_same_v<A, arrayType>)
                    return static_cast<bool>(value.size());
                else if constexpr (std::is_same_v<A, nullptr_t>)
                    return _properties.size() != 0;
                else if constexpr (std::is_same_v<A, functionType> || std::is_same_v<A, nativeFunctionType>)
                    return true;
                /*else
                {
                    auto converter = read("boolean"_n);
                    if (converter)
                        return (*converter)(thisObj, {}, nullptr);
                }*/
            }

            throw std::runtime_error("unsupported conversion: "s + getTypeName() + " to "s + typeid(T).name());
        },
                          _value);

        //TODO user conversions
    }

    inline void setConst(bool v = true) { _isConst = v; }
    inline bool isConst() { return _isConst; }
    objectPtr &operator[](const name &n);
    objectPtr operator()(objectPtr thisObj, arrayType &&args, stack *st);
    inline bool hasOwnProperty(const name &n) { return _properties.count(n) || (n == name::prototype && _prototype); }
    inline void addProperty(const name &n, objectPtr ptr)
    {
        if (n == name::prototype)
        {
            _prototype = ptr;
            return;
        }
        _properties.insert_or_assign(n, ptr);
    }
    inline void addProperties(propertiesType::iterator begin, propertiesType::iterator end) { _properties.insert(begin, end); }
    inline void removeProperty(const name &n) { _properties.erase(n); }
    arrayType getOwnPropertyNames();
    void clear();

    static objectPtr numberPrototype, stringPrototype, booleanPrototype, arrayPrototype, objectPrototype, functionPrototype, promisePrototype, classPrototype;
    static void setGlobalStack(stack *newGlobalStack)
    {
        globalStack = newGlobalStack;
    }

private:
    friend class objectPtrImpl;
    std::variant<nullptr_t, bool, number, std::string, arrayType, objectPromise::promisePtr, functionType, nativeFunctionType, file> _value;
    propertiesType _properties;
    bool _isConst;
    objectPtr _prototype;

    static stack *globalStack;

    struct buffer
    {
        int length, head, tail;
        static constexpr int maxLength = 64;
        std::array<object *, maxLength> data;
    };
    static buffer memory, objects;
    static void reuse(object *ptr);

    objectPtr &read(const name &n);
    const char *getTypeName() const
    {
        return std::visit([](auto &&v) { return typeid(v).name(); }, _value);
    }
    std::size_t getTypeHashCode() const
    {
        return std::visit([](auto &&v) { return typeid(v).hash_code(); }, _value);
    }
};

class objectException : public std::exception
{
public:
    objectException(object::objectPtr &&obj) : ptr(obj) {}
    objectException(const object::objectPtr &obj) : ptr(obj) {}

    const char *what() const noexcept override { return "object exception"; }
    object::objectPtr &getPtr()
    {
        return ptr;
    }

private:
    object::objectPtr ptr;
};

#endif
