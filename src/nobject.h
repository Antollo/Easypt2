#ifndef NOBJECT_H
#define NOBJECT_H

#include <any>
#include <unordered_map>
#include <vector>
#include <type_traits>
#include <cstddef>
#include <array>
#include <bitset>
#include "objectPtrImpl.h"
#include "name.h"
#include "stack.h"
#include "number.h"
#include "Node.h"
#include "coroutine.h"
#include "file.h"
#include "childProcess.h"
#include "allocator.h"

template <class T>
using remove_cref_t = std::remove_const_t<std::remove_reference_t<T>>;

objectPtrImpl constructorCaller(objectPtrImpl thisObj, std::vector<objectPtrImpl, allocator<objectPtrImpl>> &&args, stack *st);

class tcpClient;
class tcpServer;

class object
{
public:
    using objectPtr = objectPtrImpl;
    using propertiesType = std::unordered_map<name, objectPtr, std::hash<name>, std::equal_to<name>, allocator<std::pair<const name, objectPtr>>>;

    struct type
    {
        using Object = nullptr_t;
        using Boolean = bool;
        using Number = number;
        using String = std::string;
        using Array = std::vector<objectPtr, allocator<objectPtr>>;
        using Promise = std::shared_ptr<coroutine<objectPtr>>;
        using Function = std::shared_ptr<Node>;
        using NativeFunction = objectPtr (*)(objectPtr, Array &&, stack *);
        using File = std::shared_ptr<file>;
        using TcpClient = std::shared_ptr<tcpClient>;
        using TcpServer = std::shared_ptr<tcpServer>;
        using ChildProcess = std::shared_ptr<childProcess>;
    };

    enum class typeIndex
    {
        Object,
        Boolean,
        Number,
        String,
        Array,
        Promise,
        Function,
        NativeFunction,
        File,
        TcpClient,
        TcpServer
    };

    static type::Function makeFunction()
    {
        return std::make_shared<Node>(0, "root");
    }

    template <class T>
    static objectPtr makeObject(T &&t)
    {
        if (objects.length == 0)
            return objectPtr(new object(std::forward<T>(t)));

        object *ptr = objects.data[objects.tail++];
        if (objects.tail == buffer::maxLength)
            objects.tail = 0;
        objects.length--;
        ptr->_value = std::forward<T>(t);

        if constexpr (std::is_same_v<std::decay_t<T>, type::NativeFunction>)
            ptr->init<T>(t == constructorCaller);
        else
            ptr->init<T>();
        return ptr;
    }

    static objectPtr makeEmptyObject()
    {
        return makeObject(nullptr);
    }

    void *operator new(size_t size);
    void operator delete(void *p);

    template <class T>
    explicit object(T &&t) : _value(std::forward<T>(t))
    {
        if constexpr (std::is_same_v<std::decay_t<T>, type::NativeFunction>)
            init<T>(t == constructorCaller);
        else
            init<T>();
    }

    template <class T>
    void init(const bool isConstructorCaller = false)
    {
        if constexpr (std::is_same_v<std::decay_t<T>, type::Object>)
            _prototype = objectPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, type::Number>)
            _prototype = numberPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, type::String>)
            _prototype = stringPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, type::Array>)
            _prototype = arrayPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, type::Boolean>)
            _prototype = booleanPrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, type::Promise>)
            _prototype = promisePrototype;
        else if constexpr (std::is_same_v<std::decay_t<T>, type::Function> || std::is_same_v<std::decay_t<T>, type::NativeFunction>)
        {
            if constexpr (std::is_same_v<std::decay_t<T>, type::NativeFunction>)
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
    object &operator=(const object &rhs) = default;

    object &operator=(const objectPtr &rhs)
    {
        if (!isSettable())
            return *this = *rhs;
        objectPtr parent = read(n::__parent);
        if (!parent)
            throw std::runtime_error("__parent not found");
        objectPtr set = parent->read(n::set);
        if (!set)
            throw std::runtime_error("set not found");
        return *(*set)(parent, {rhs}, nullptr);
    }

    template <class T>
    bool isOfType() const
    {
        return std::holds_alternative<remove_cref_t<T>>(_value);
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
            if (isConst())
                throw std::runtime_error("tried to modify constant value");
            return std::get<remove_cref_t<T>>(_value);
        }
    }

    template <class T>
    const remove_cref_t<T> &get() const
    {
        return std::get<remove_cref_t<T>>(_value);
    }

    template <class Visitor>
    auto visit(objectPtr b, Visitor &&visitor)
    {
        return std::visit(std::forward<Visitor>(visitor), _value, b->_value);
    }

    template <class T>
    constexpr bool isConvertible() const
    {
        using RT = remove_cref_t<T>;
        return std::is_same_v<RT, type::Object> || std::is_same_v<RT, type::Boolean> || std::is_same_v<RT, type::Number> 
        || std::is_same_v<RT, type::String> || std::is_same_v<RT, type::Array>;
    }

    template <class T>
    remove_cref_t<T> getConverted()
    {
        using RT = remove_cref_t<T>;

        return std::visit([this](auto &&value) -> RT {
            using A = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<RT, A>)
                return value;

            else if constexpr (std::is_same_v<RT, type::Number>)
            {
                if constexpr (std::is_same_v<A, type::String>)
                    return static_cast<type::Number>(value);
                else if constexpr (std::is_same_v<A, type::Array>)
                    return static_cast<type::Number>(value.size());
                else if constexpr (std::is_same_v<A, type::Boolean>)
                    return static_cast<type::Number>(value);
                else
                    return static_cast<type::Number>(_properties.size());
            }

            else if constexpr (std::is_same_v<RT, type::String>)
            {
                if constexpr (std::is_same_v<A, type::Number>)
                    return static_cast<type::String>(value);
                else if constexpr (std::is_same_v<A, type::Array>)
                    return toJson();
                else if constexpr (std::is_same_v<A, type::Boolean>)
                    return value ? "true"s : "false"s;
                else
                    return toJson();
            }

            else if constexpr (std::is_same_v<RT, type::Array>)
            {
                return type::Array{object::makeObject(value)};
            }

            else if constexpr (std::is_same_v<RT, type::Boolean>)
            {
                if constexpr (std::is_same_v<A, type::Number>)
                    return static_cast<type::Boolean>(value);
                else if constexpr (std::is_same_v<A, type::String>)
                    return static_cast<type::Boolean>(value.size());
                else if constexpr (std::is_same_v<A, type::Array>)
                    return static_cast<type::Boolean>(value.size());
                else
                    return _properties.size() != 0;
            }

            throw std::runtime_error("unsupported conversion: "s + getTypeName() + " to "s + typeid(T).name());
        },
                          _value);
    }

    void setConst(bool v = true) { _flags[_const] = v; }
    void setAccessible(bool v = true) { _flags[_accessible] = v; }
    void setSettable(bool v = true) { _flags[_setttable] = v; }
    bool isConst() const { return _flags[_const]; }
    bool isAccessible() const { return _flags[_accessible]; }
    bool isSettable() const { return _flags[_setttable]; }
    objectPtr &operator[](const name &n);
    objectPtr operator()(objectPtr thisObj, type::Array &&args, stack *st);
    bool hasOwnProperty(const name &n) const { return _properties.count(n) || (n == n::prototype && _prototype); }
    void addProperty(const name &n, objectPtr ptr)
    {
        if (n == n::prototype)
            _prototype = ptr;
        else
            _properties.insert_or_assign(n, ptr);
    }
    template <class It>
    void addProperties(It begin, It end)
    {
        _properties.insert(begin, end);
        _properties.erase(n::empty);
    }
    void removeProperty(const name &n) { _properties.erase(n); }
    type::Array getOwnPropertyNames() const;
    std::string toJson() const;
    void clear();

    void captureStack(stack &&st) { _capturedStack = std::make_shared<stack>(std::move(st)); }
    void captureStack(const std::shared_ptr<stack> &st) { _capturedStack = st; }
    void captureStack(std::shared_ptr<stack> &&st) { _capturedStack = std::move(st); }

    static objectPtr numberPrototype, stringPrototype, booleanPrototype, arrayPrototype, objectPrototype, functionPrototype, promisePrototype, classPrototype;

    static void setGlobalStack(stack *newGlobalStack) { globalStack = newGlobalStack; }

    static objectPtr &checkGetter(objectPtr &ptr)
    {
        if (!ptr->isAccessible())
            return ptr;
        static objectPtr temp;
        temp = (*(*ptr)[n::get])(ptr, {}, nullptr);
        temp->addProperty(n::__parent, ptr);
        temp->setSettable();
        return temp;
    }

private:
    friend class objectPtrImpl;
    friend class Import;
public:
    std::variant<type::Object, type::Boolean, type::Number, type::String, type::Array, type::Promise, type::Function, type::NativeFunction,
    type::File, type::TcpClient, type::TcpServer, type::ChildProcess> _value;
private:
    propertiesType _properties;
    objectPtr _prototype;
    std::shared_ptr<stack> _capturedStack;
    std::bitset<3> _flags;
    static constexpr int _const = 0;
    static constexpr int _accessible = 1;
    static constexpr int _setttable = 2;

    static stack *globalStack;

    struct buffer
    {
        int length, head, tail;
        static constexpr int maxLength = 128;
        std::array<object *, maxLength> data;
    };
    inline static buffer objects;
    static void reuse(object *ptr);

    objectPtr &read(const name &n);
    const char *getTypeName() const
    {
        return std::visit([](auto &&v) { return typeid(v).name(); }, _value);
    }
    void toJson(std::string &str, const int indentation = 1) const;
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
