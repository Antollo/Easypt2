#include "nobject.h"
#include "Node.h"

allocatorBuffer<sizeof(object)> objectMemoryBuffer;

void *object::operator new(size_t)
{
    return static_cast<object *>(objectMemoryBuffer.allocate());
}

void object::operator delete(void *ptr)
{
    objectMemoryBuffer.deallocate(ptr);
}

void object::reuse(object *ptr)
{
    if (objects.length == buffer::maxLength)
    {
        //console::log("reuse full");
        delete ptr;
        return;
    }
    objects.data[objects.head++] = ptr;
    if (objects.head == buffer::maxLength)
        objects.head = 0;
    objects.length++;
    ptr->clear();
}

object::objectPtr &object::operator[](const name &n)
{
    if (n == n::prototype)
        return _prototype;
    {
        auto it = _properties.find(n);
        if (it != _properties.end())
            return checkGetter(it->second);
    }
    if (_prototype)
    {
        object::objectPtr &res = _prototype->read(n);
        if (res)
            return res;
    }
    return (_properties[n] = makeEmptyObject());
}

object::objectPtr object::operator()(objectPtr thisObj, type::Array &&args, stack *st)
{
    if (st == nullptr)
        st = globalStack;
    if (isOfType<type::NativeFunction>())
    {
        if (thisObj)
            return get<type::NativeFunction>()(thisObj, std::move(args), st);
        throw std::runtime_error("invalid this");
    }
    else if (isOfType<type::Function>())
    {
        if (_capturedStack != nullptr)
            st = _capturedStack.get();
        stack localStack(st);
        if (_capturedStack != nullptr)
        {
            auto referenceToFather = localStack.insert(n::__stack, makeEmptyObject());
            referenceToFather->captureStack(_capturedStack);
        }
        const type::Function &node = get<const type::Function>();
        for (size_t i = 0; i < node->names().size() && i < args.size(); i++)
            localStack.insert(node->names()[i], args[i]);
        localStack.insert(n::args, makeObject(std::move(args)));
        if (thisObj)
            localStack.insert(n::thisObj, thisObj);
        try
        {
            auto evalRet = node->evaluate(localStack);
            if (evalRet != nullptr)
                return evalRet;
        }
        catch (const object::objectPtr &ret)
        {
            return ret;
        }
        if (thisObj)
            return thisObj;
        else
            return makeEmptyObject();
    }
    throw std::runtime_error("object is not a function");
}

object::type::Array object::getOwnPropertyNames() const
{
    type::Array res;
    res.reserve(_properties.size());
    for (const auto &property : _properties)
        res.push_back(makeObject(static_cast<std::string>(property.first)));
    if (_prototype)
        res.push_back(makeObject("prototype"s));
    return res;
}

object::type::Array object::getOwnPropertyValues() const
{
    type::Array res;
    res.reserve(_properties.size());
    for (const auto &property : _properties)
        res.push_back(property.second);
    if (_prototype)
        res.push_back(_prototype);
    return res;
}

object::objectPtr &object::read(const name &n)
{
    static object::objectPtr notFound(nullptr);
    if (n == n::prototype)
        return _prototype;
    {
        auto it = _properties.find(n);
        if (it != _properties.end())
            return it->second;
    }
    if (_prototype && _prototype.get() != this)
        return _prototype->read(n);
    return notFound;
}

const object::objectPtr &object::read(const name &n) const
{
    static object::objectPtr notFound(nullptr);
    if (n == n::prototype)
        return _prototype;
    {
        auto it = _properties.find(n);
        if (it != _properties.end())
            return it->second;
    }
    if (_prototype && _prototype.get() != this)
        return _prototype->read(n);
    return notFound;
}

std::string object::toJson() const
{
    std::string str;
    toJson(str);
    return str;
}

void object::toJson(std::string &str, const int indentation) const
{
    std::visit([this, indentation, &str](auto &&value) {
        std::string tabs(indentation, '\t');
        using A = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<A, std::string>)
            str += "\"" + value + "\"";
        else if constexpr (std::is_same_v<A, number>)
            str += static_cast<std::string>(value);
        else if constexpr (std::is_same_v<A, type::Array>)
        {
            str += "[";
            for (int i = 0; i < value.size(); i++)
            {
                if (i)
                    str += ", ";
                value[i]->toJson(str, indentation + 1);
            }
            str += "]";
        }
        else if constexpr (std::is_same_v<A, bool>)
            str += value ? "true"s : "false"s;
        else if constexpr (std::is_same_v<A, type::Promise>)
            str += "<promise>";
        else if constexpr (std::is_same_v<A, type::Function> || std::is_same_v<A, type::NativeFunction>)
            str += "<function>";
        else if constexpr (std::is_same_v<A, std::shared_ptr<file>>)
            str += "<file>";
        else if constexpr (std::is_same_v<A, std::shared_ptr<tcpClient>>)
            str += "<tcpClient>";
        else if constexpr (std::is_same_v<A, std::shared_ptr<tcpServer>>)
            str += "<tcpServer>";
        else
        {
            std::string tabs(indentation * 4, ' ');
            str += "{\n";
            for (const auto &property : _properties)
            {
                str += tabs + "\"" + static_cast<std::string>(property.first) + "\": ";
                property.second->toJson(str, indentation + 1);
                str += ",\n";
            }
            if (!_properties.empty())
            {
                str.pop_back();
                str.pop_back();
            }
            str += "\n" + tabs.substr(4) + "}";
        }
    },
               _value);
}

void object::clear()
{
    _properties.clear();
    _capturedStack.reset();
    _value = nullptr;
    _flags.reset();
    _prototype = nullptr;
}