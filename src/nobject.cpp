#include "nobject.h"
#include "statement.h"

object::objectPtr object::numberPrototype, object::stringPrototype, object::booleanPrototype, object::arrayPrototype, object::objectPrototype, object::functionPrototype;

object::~object()
{
    beingDestructed = true;
}

std::shared_ptr<object> object::shared_from_this()
{
    if (beingDestructed)
        return std::shared_ptr<object>(this, fakeDeleter());
    return std::enable_shared_from_this<object>::shared_from_this();
}

object::objectPtr& object::operator[](const name &n)
{
    if (properties.count(n))
        return properties[n];
    if (properties.count("prototype"_n) && properties["prototype"_n])
    {
        object::objectPtr& res = properties["prototype"_n]->read(n);
        if (res)
            return res;
    }
    return (properties[n] = makeUndefined());
}

object::objectPtr object::operator()(objectPtr thisObj, arrayType &&args, stack *st)
{
    if (isOfType<nativeFunctionType>())
    {
        return get<nativeFunctionType>()(thisObj, std::move(args), st);
    }
    if (isOfType<compoundStatement>())
    {
        stack localStack(st);
        if (thisObj)
            localStack.insert("this"_n, thisObj);
        localStack.insert("args"_n, makeObject(std::move(args)));
        //TODO implement returning
        get<compoundStatement>()(&localStack);
        return makeUndefined();
    }
    throw std::runtime_error("object is not a function");

    //TODO implement call operator
    //return (*(*this)["callOperator"_n])(thisObj, std::move(args), st);
}

object::objectPtr& object::read(const name &n)
{
    static object::objectPtr notFound(nullptr);
    if (properties.count(n))
        return properties[n];
    if (properties.count("prototype"_n) && properties["prototype"_n])
        return properties["prototype"_n]->read(n);
    return notFound;
}

void object::clear()
{
    properties.clear();
}