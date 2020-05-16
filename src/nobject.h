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

#define makeObject(...) objectPtrImpl(new object{__VA_ARGS__})
#define makeEmptyObject(...) objectPtrImpl(new object{nullptr})

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
    explicit object(const T &t) : _value(std::move(t)), _isConst(false)
    {
        if constexpr (std::is_same_v<std::decay_t<T>, nullptr_t>)
            _properties["prototype"_n] = objectPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, number>)
            _properties["prototype"_n] = numberPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, std::string>)
            _properties["prototype"_n] = stringPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, arrayType>)
            _properties["prototype"_n] = arrayPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, bool>)
            _properties["prototype"_n] = booleanPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, objectPromise::promisePtr>)
            _properties["prototype"_n] = promisePrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, functionType> || std::is_same_v<std::decay_t<T>, nativeFunctionType>)
        {
            if constexpr (std::is_same_v<std::decay_t<T>, nativeFunctionType>)
            {
                if (t == constructorCaller)
                    _properties["prototype"_n] = classPrototype;
                else
                    _properties["prototype"_n] = functionPrototype;
            }
            else
                _properties["prototype"_n] = functionPrototype;
        }
        else
            throw std::runtime_error("wrong type "s + typeid(T).name());
    };

    object(object &&) = default;
    object &operator=(object &&) = default;
    object(const object &) = default;
    object &operator=(const object &) = default;

    template <class T>
    bool isOfType() const
    {
        //return typeid(remove_cref_t<T>).hash_code() == _value.type().hash_code();
        return typeid(remove_cref_t<T>).hash_code() == getTypeHashCode();
    }

    template <class T>
    void setType()
    {
        //_value = std::any(std::in_place_type_t<T>());
        _value.emplace<remove_cref_t<T>>();
    }

    template <class T>
    std::remove_reference_t<T> &get()
    {
        if constexpr (std::is_const_v<T>)
        {
            //return std::any_cast<remove_cref_t<T> &>(_value);
            return std::get<remove_cref_t<T>>(_value);
        }
        else
        {
            if (_isConst)
                throw std::runtime_error("tried to modify constant value");
            //return std::any_cast<remove_cref_t<T> &>(_value);
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
                else
                    return static_cast<number>(0);
            }

            else if constexpr (std::is_same_v<remove_cref_t<T>, std::string>)
            {
                if constexpr (std::is_same_v<A, number>)
                    return static_cast<std::string>(value);
                else if constexpr (std::is_same_v<A, arrayType>)
                    return "["s + std::to_string(value.size()) + "]"s;
                else if constexpr (std::is_same_v<A, bool>)
                    return value ? "true"s : "false"s;
                else
                    return ""s;
            }

            else if constexpr (std::is_same_v<remove_cref_t<T>, arrayType>)
            {
                if constexpr (std::is_same_v<A, number>)
                    return arrayType{makeObject(value)};
                else if constexpr (std::is_same_v<A, std::string>)
                    return arrayType{makeObject(value)};
                else if constexpr (std::is_same_v<A, bool>)
                    return arrayType{makeObject(value)};
                else
                    return arrayType{};
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
                    return !(hasOwnProperty("prototype"_n) && _properties.size() == 1);
                else if constexpr (std::is_same_v<A, functionType> || std::is_same_v<A, nativeFunctionType>)
                    return true;
                else
                    return false;
            }

            else
                throw std::runtime_error("unsupported conversion: "s + getTypeName() + " to "s + typeid(T).name());
        },
                          _value);

        /*if (isOfType<T>())
            return get<const remove_cref_t<T>>();
        if constexpr (std::is_same_v<remove_cref_t<T>, number>)
        {
            if (isOfType<std::string>())
                return static_cast<number>(get<const std::string>());
            if (isOfType<arrayType>())
                return static_cast<number>(get<const arrayType>().size());
            if (isOfType<bool>())
                return static_cast<number>(get<const bool>());
        }
        if constexpr (std::is_same_v<remove_cref_t<T>, std::string>)
        {
            if (isOfType<number>())
                return static_cast<std::string>(get<const number>());
            if (isOfType<arrayType>())
                return "["s + std::to_string(get<const arrayType>().size()) + "]"s;
            if (isOfType<bool>())
                return get<const bool>() ? "true"s : "false"s;
        }
        if constexpr (std::is_same_v<remove_cref_t<T>, arrayType>)
        {
            if (isOfType<number>())
                return arrayType{makeObject(get<const number>())};
            if (isOfType<std::string>())
                return arrayType{makeObject(get<const std::string>())};
            if (isOfType<bool>())
                return arrayType{makeObject(get<const bool>())};
        }
        if constexpr (std::is_same_v<remove_cref_t<T>, bool>)
        {
            if (isOfType<number>())
                return static_cast<bool>(get<const number>());
            if (isOfType<std::string>())
                return static_cast<bool>(get<const std::string>().size());
            if (isOfType<arrayType>())
                return static_cast<bool>(get<const arrayType>().size());
            if (isOfType<nullptr_t>())
                return !(hasOwnProperty("prototype"_n) && _properties.size() == 1);
            if (isOfType<compoundStatement>() || isOfType<nativeFunctionType>())
                return true;
        }*/
        //TODO user conversions
    }

    inline void setConst(bool v = true) { _isConst = v; }
    inline bool isConst() { return _isConst; }
    objectPtr &operator[](const name &n);
    objectPtr operator()(objectPtr thisObj, arrayType &&args, stack *st);
    inline bool hasOwnProperty(const name &n) { return _properties.count(n); }
    inline void addProperty(const name &n, objectPtr ptr) { _properties.insert_or_assign(n, ptr); }
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
    //std::any _value;
    std::variant<nullptr_t, bool, number, std::string, arrayType, objectPromise::promisePtr, functionType, nativeFunctionType, file> _value;
    propertiesType _properties;
    bool _isConst;
    const objectPtrImpl *_thisPtr;
    static stack *globalStack;
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
