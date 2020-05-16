#include "nobject.h"
#include "Node.h"

object::objectPtr object::numberPrototype, object::stringPrototype, object::booleanPrototype, object::arrayPrototype,
    object::objectPrototype, object::functionPrototype, object::promisePrototype, object::classPrototype;
stack *object::globalStack;

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
    if (isOfType<functionType>())
    {
        stack localStack(st);
        const functionType &node = get<const functionType>();
        for (size_t i = 0; i < node.names().size() && i < args.size(); i++)
            localStack.insert(node.names()[i], args[i]);
        localStack.insert("args"_n, makeObject(std::move(args)));
        if (thisObj)
            localStack.insert("this"_n, thisObj);
        try
        {
            node.evaluate(localStack);
        }
        catch (const object::objectPtr &ret)
        {
            return ret;
        }
        //WARNING: this can be unsafe
        return nullptr;
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