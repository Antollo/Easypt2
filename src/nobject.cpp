#include "nobject.h"
#include "Node.h"

object::objectPtr object::numberPrototype, object::stringPrototype, object::booleanPrototype, object::arrayPrototype,
    object::objectPrototype, object::functionPrototype, object::promisePrototype, object::classPrototype;
stack *object::globalStack;

object::buffer object::memory = {0, 0, 0};
object::buffer object::objects = {0, 0, 0};

void *object::operator new(size_t size)
{
    if (memory.length == 0)
    {
        //console::log("new empty");
        return std::malloc(size);
    }
    
    void *ptr = memory.data[memory.tail++];
    if (memory.tail == buffer::maxLength)
        memory.tail = 0;
    memory.length--;
    return ptr;
}

void object::operator delete(void *ptr)
{
    if (memory.length == buffer::maxLength)
    {
        //console::log("delete full");
        std::free(ptr);
        return;
    }
    memory.data[memory.head++] = reinterpret_cast<object *>(ptr);
    if (memory.head == buffer::maxLength)
        memory.head = 0;
    memory.length++;
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
    if (n == name::prototype)
        return _prototype;
    if (_properties.count(n))
        return _properties[n];
    if (_prototype)
    {
        object::objectPtr &res = _prototype->read(n);
        if (res)
            return res;
    }
    return (_properties[n] = makeEmptyObject());
}

object::objectPtr object::operator()(objectPtr thisObj, arrayType &&args, stack *st)
{
    if (st == nullptr)
        st = globalStack;
    if (isOfType<nativeFunctionType>())
    {
        if (thisObj)
            return get<nativeFunctionType>()(thisObj, std::move(args), st);
        throw std::runtime_error("invalid this");
    }
    if (isOfType<functionType>())
    {
        stack localStack(st);
        const functionType &node = get<const functionType>();
        for (size_t i = 0; i < node.names().size() && i < args.size(); i++)
            localStack.insert(node.names()[i], args[i]);
        localStack.insert(name::args, makeObject(std::move(args)));
        if (thisObj)
            localStack.insert(name::thisObj, thisObj);
        try
        {
            node.evaluate(localStack);
        }
        catch (const object::objectPtr &ret)
        {
            return ret;
        }
        //WARNING: this can be unsafe
        return thisObj;
    }
    throw std::runtime_error("object is not a function");
}

object::arrayType object::getOwnPropertyNames()
{
    arrayType res;
    res.reserve(_properties.size());
    for (const auto &property : _properties)
        res.push_back(makeObject(static_cast<std::string>(property.first)));
    if (_prototype)
        res.push_back(makeObject("prototype"s));
    return res;
}

object::objectPtr &object::read(const name &n)
{
    static object::objectPtr notFound(nullptr);
    if (n == name::prototype)
        return _prototype;
    if (_properties.count(n))
        return _properties[n];
    if (_prototype && _prototype.get() != this)
        return _prototype->read(n);
    return notFound;
}

void object::clear()
{
    _properties.clear();
    _value = nullptr;
    _isConst = false;
    _prototype = nullptr;
    //_thisPtr = nullptr;
}