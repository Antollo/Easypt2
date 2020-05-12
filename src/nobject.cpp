#include "nobject.h"
#include "statement.h"

object::objectPtr object::numberPrototype, object::stringPrototype, object::booleanPrototype, object::arrayPrototype,
    object::objectPrototype, object::functionPrototype, object::promisePrototype, object::classPrototype;
stack *object::globalStack;

//std::vector<std::array<std::byte, sizeof(object)>> objectMemory::memoryBlock(100000);
//std::vector<void *> objectMemory::freeElements;

/*void *object::operator new(size_t)
{
    static int i = 0;
    if (!objectMemory::freeElements.empty())
    {
        void *res = objectMemory::freeElements.back();
        objectMemory::freeElements.pop_back();
        return res;
    }
    return &objectMemory::memoryBlock[i++];
}

void object::operator delete(void *ptr)
{
    objectMemory::freeElements.push_back(ptr);
}*/

object::objectPtr &object::operator[](const name &n)
{
    if (_properties.count(n))
        return _properties[n];
    if (_properties.count("prototype"_n) && _properties["prototype"_n])
    {
        object::objectPtr &res = _properties["prototype"_n]->read(n);
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
        return get<nativeFunctionType>()(*_thisPtr, std::move(args), st);
    }
    if (isOfType<compoundStatement>())
    {
        stack localStack(st);
        executionMemory memory;
        if (thisObj)
            localStack.insert("this"_n, thisObj);
        localStack.insert("args"_n, makeObject(std::move(args)));
        try
        {
            get<compoundStatement>()(localStack, memory);
        }
        catch (const object::objectPtr &ret)
        {
            return ret;
        }
        return makeEmptyObject();
    }
    throw std::runtime_error("object is not a function");

    //TODO implement call operator
    //return (*(*this)["callOperator"_n])(thisObj, std::move(args), st);
}

object::arrayType object::getOwnPropertyNames()
{
    arrayType res;
    res.reserve(_properties.size());
    for (const auto &property : _properties)
        res.push_back(makeObject(static_cast<std::string>(property.first)));
    return res;
}

object::objectPtr &object::read(const name &n)
{
    static object::objectPtr notFound(nullptr);
    if (_properties.count(n))
        return _properties[n];
    if (_properties.count("prototype"_n) && _properties["prototype"_n] && _properties["prototype"_n].get() != this)
        return _properties["prototype"_n]->read(n);
    return notFound;
}

void object::clear()
{
    _properties.clear();
}