#ifndef NOBJECT_H
#define NOBJECT_H

#include <any>
#include <unordered_map>
#include <vector>
#include <type_traits>
#include "name.h"
#include "stack.h"
#include "number.h"
#include "statement.h"

template <class T>
using remove_cref_t = std::remove_const_t<std::remove_reference_t<T>>;

#define makeObject(...) std::shared_ptr<object>(new object{__VA_ARGS__})
#define makeUndefined(...) std::shared_ptr<object>(new object{nullptr})

class object : public std::enable_shared_from_this<object>
{
public:
    using objectPtr = std::shared_ptr<object>;
    using objectRawPtr = object *;
    using arrayType = std::vector<objectPtr>;
    using nativeFunctionType = objectPtr (*)(objectPtr, arrayType &&, stack *);
    using propertiesType = std::unordered_map<name, objectPtr>;

    template <class T>
    explicit object(const T& t) : value(std::move(t)), beingDestructed(false), _isConst(false)
    {
        if constexpr (std::is_same_v<std::decay_t<T>, nullptr_t>)
            properties["prototype"_n] = objectPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, number>)
            properties["prototype"_n] = numberPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, std::string>)
            properties["prototype"_n] = stringPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, arrayType>)
            properties["prototype"_n] = arrayPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, bool>)
            properties["prototype"_n] = booleanPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, compoundStatement> || std::is_same_v<std::decay_t<T>, nativeFunctionType>)
            properties["prototype"_n] = functionPrototype;
        else
            throw std::runtime_error("wrong type "s + typeid(T).name());
    };

    object(object &&) = default;
    object &operator=(object &&) = default;
    object(const object &) = default;
    object &operator=(const object &) = default;
    ~object();
    std::shared_ptr<object> shared_from_this();

    template <class T>
    bool isOfType()
    {
        return typeid(remove_cref_t<T>).hash_code() == value.type().hash_code();
    }

    template <class T>
    std::remove_reference_t<T> &get()
    {
        if constexpr (std::is_const_v<T>)
        {
            return std::any_cast<remove_cref_t<T> &>(value);
        }
        else
        {
            if (_isConst)
                throw std::runtime_error("tried to modify constant value");
            return std::any_cast<remove_cref_t<T> &>(value);
        }
    }

    template <class T>
    bool isConvertible()
    {
        if constexpr (std::is_same_v<remove_cref_t<T>, number> || std::is_same_v<remove_cref_t<T>, std::string> || std::is_same_v<remove_cref_t<T>, arrayType>)
        {
            return isOfType<number>() || isOfType<std::string>() || isOfType<arrayType>();
        }
        // TODO user conversions
        else return false;
    }

    template <class T>
    remove_cref_t<T> getConverted()
    {
        if (isOfType<T>())
            return get<const remove_cref_t<T>>();
        if constexpr (std::is_same_v<remove_cref_t<T>, number>)
        {
            if (isOfType<std::string>())
                return static_cast<number>(get<const std::string>());
            if (isOfType<arrayType>())
                return static_cast<number>(static_cast<int>(get<const arrayType>().size()));
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
        }
        //TODO user conversions
        throw std::runtime_error("unsupported conversion: "s + value.type().name() + " to "s + typeid(T).name());
    }

    inline void setConst(bool v = true) { _isConst = v; }
    inline bool isConst() { return _isConst; }
    objectPtr &operator[](const name &n);
    objectPtr operator()(objectPtr thisObj, arrayType &&args, stack *st);
    void clear();

    static objectPtr numberPrototype, stringPrototype, booleanPrototype, arrayPrototype, objectPrototype, functionPrototype;

private:
    std::any value;
    propertiesType properties;
    bool beingDestructed, _isConst;

    struct fakeDeleter
    {
        void operator()(objectRawPtr) const {};
    };

    objectPtr &read(const name &n);
};

class objectException : public std::exception, object::objectPtr
{
public:
    objectException(object::objectPtr &&obj) : object::objectPtr(obj) {}
    objectException(const object::objectPtr &obj) : object::objectPtr(obj) {}

    const char *what() const noexcept override { return "objectException"; }
    object::objectPtr &getPtr()
    {
        return dynamic_cast<object::objectPtr &>(*this);
    }
};

#endif
