#include <iterator>
#include "nobject.h"
#include "Node.h"

std::string escapeString(const std::string &str)
{

    std::string result;
    result.reserve(str.size() + 2);

    result.push_back('"');
    for (char c : str)
    {
        if (' ' <= c and c <= '~' and c != '\\' and c != '"')
        {
            result.push_back(c);
        }
        else
        {
            result.push_back('\\');
            switch (c)
            {
            case '"':
                result.push_back('"');
                break;
            case '\\':
                result.push_back('\\');
                break;
            case '\t':
                result.push_back('t');
                break;
            case '\r':
                result.push_back('r');
                break;
            case '\n':
                result.push_back('n');
                break;
            default:
            {
                std::stringstream s;
                s << 'x' << std::setfill('0') << std::setw(2) << std::hex << (int)(c & 0xff);
                result.append(s.str());
                break;
            }
            }
        }
    }
    result.push_back('"');
    return result;
}

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
    if (objects.length == objectBuffer::maxLength)
    {
        delete ptr;
        return;
    }
    objects.data[objects.head++] = ptr;
    if (objects.head == objectBuffer::maxLength)
        objects.head = 0;
    objects.length++;
    ptr->clear();
}

object::objectPtr &object::operator[](const name &n)
{
    if (n == n::prototype)
        return _prototype;
    {
        auto it = _properties->find(n);
        if (it != _properties->end())
            return checkGetter(it->second);
    }
    if (_prototype)
    {
        object::objectPtr &res = _prototype->read(n);
        if (res)
            return res;
    }
    return ((*_properties)[n] = makeEmptyObject());
}

object::objectPtr object::operator()(objectPtr thisObj, type::Array &&args, stack *st)
{
    if (st == nullptr)
        st = globalStack;
    switch (getTypeIndex())
    {
    case typeIndex::NativeFunction:
    {
        if (thisObj)
            return uncheckedGet<type::NativeFunction>()(thisObj, std::move(args), st);
        throw std::runtime_error("invalid this");
        break;
    }

    case typeIndex::Function:
    {
        auto &node = uncheckedGet<type::Function>().first;
        auto capturedStack = uncheckedGet<type::Function>().second;

        if (capturedStack != nullptr)
            st = capturedStack.get();
        stack localStack(st);

        auto &names = node->names();
        const size_t toInsert = std::min(names.size(), args.size());
        localStack.reserve(toInsert + 1 + (bool)thisObj);
        for (size_t i = 0; i < toInsert; i++)
            localStack.insert(names[i], args[i]);
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
        break;
    }

    case typeIndex::ExternalFunction:
    {
        if (thisObj)
            return uncheckedGet<type::ExternalFunction>()(thisObj, std::move(args), st);
        throw std::runtime_error("invalid this");
        break;
    }

    default:
        throw std::runtime_error("object is not a function");
    }
}

object::type::Array object::getOwnPropertyNames() const
{
    type::Array res;
    res.reserve(_properties->size());
    for (const auto &property : *_properties)
        res.push_back(makeObject(static_cast<std::string>(property.first)));
    if (_prototype)
        res.push_back(makeObject("prototype"s));
    return res;
}

object::type::Array object::getOwnPropertyValues() const
{
    type::Array res;
    res.reserve(_properties->size());
    for (const auto &property : *_properties)
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
        auto it = _properties->find(n);
        if (it != _properties->end())
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
        auto it = _properties->find(n);
        if (it != _properties->end())
            return it->second;
    }
    if (_prototype && _prototype.get() != this)
        return _prototype->read(n);
    return notFound;
}

std::string object::toJson() const
{
    std::stringstream str;
    str << std::boolalpha;
    toJson(str);
    return str.str();
}

void object::toJson(std::stringstream &str, const size_t indentation) const
{
    std::visit([this, indentation, &str](auto &&value)
               {
                   using A = std::decay_t<decltype(value)>;

                   if constexpr (std::is_same_v<A, std::string>)
                       str << escapeString(value);
                   else if constexpr (std::is_same_v<A, number>)
                       str << static_cast<std::string>(value);
                   else if constexpr (std::is_same_v<A, type::Array>)
                   {
                       str << '[';
                       for (size_t i = 0; i < value.size(); i++)
                       {
                           if (i)
                               str << ", ";
                           value[i]->toJson(str, indentation + 1);
                       }
                       str << ']';
                   }
                   else if constexpr (std::is_same_v<A, type::Buffer>)
                   {
                       auto toJsonSpan = [&str]<typename T>(const T &span)
                       {
                           str << '[';
                           for (size_t i = 0; i < span.size(); i++)
                           {
                               if (i)
                                   str << ", ";
                               str << static_cast<number>(span[i]);
                           }
                           str << ']';
                       };
                       const buffer &b = *value;
                       switch (b.elementType())
                       {
                       default:
                       case buffer::type::Int8:
                           toJsonSpan(b.asSpan<int8_t>());
                           break;
                       case buffer::type::Int16:
                           toJsonSpan(b.asSpan<int16_t>());
                           break;
                       case buffer::type::Int32:
                           toJsonSpan(b.asSpan<int32_t>());
                           break;
                       case buffer::type::Int64:
                           toJsonSpan(b.asSpan<int64_t>());
                           break;
                       case buffer::type::Float:
                           toJsonSpan(b.asSpan<float>());
                           break;
                       case buffer::type::Double:
                           toJsonSpan(b.asSpan<double>());
                           break;
                       }
                   }
                   else if constexpr (std::is_same_v<A, bool>)
                       str << value ? "true"s : "false"s;
                   else if constexpr (std::is_same_v<A, type::Promise>)
                       str << "\"<promise>\"";
                   else if constexpr (std::is_same_v<A, type::Function> || std::is_same_v<A, type::NativeFunction> || std::is_same_v<A, type::ExternalFunction>)
                       str << "\"<function>\"";
                   else if constexpr (std::is_same_v<A, type::File>)
                       str << "\"<file>\"";
                   else if constexpr (std::is_same_v<A, type::TcpClient>)
                       str << "\"<tcpClient>\"";
                   else if constexpr (std::is_same_v<A, type::TcpServer>)
                       str << "\"<tcpServer>\"";
                   else if constexpr (std::is_same_v<A, type::ChildProcess>)
                       str << "\"<childProcess>\"";
                   else
                   {
                       std::string tabs(indentation * 4, ' ');
                       str << "{\n";
                       for (const auto &property : *_properties)
                       {
                           str << tabs + "\"" << static_cast<std::string>(property.first) << "\": ";
                           property.second->toJson(str, indentation + 1);
                           str << ",\n";
                       }
                       if (!_properties->empty())
                           str.seekp(-2, std::ios_base::end);

                       str << '\n'
                           << tabs.substr(4) << '}';
                   }
               },
               _value);
}

void object::clear()
{
    _properties->clear();
    _value = nullptr;
    _flags.reset();
    _prototype = nullptr;
}