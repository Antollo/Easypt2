#include "stack.h"
#include "nobject.h"

object::objectPtr& stack::operator[] (const name& n)
{
    if (storage.count(n))
        return storage[n];
    if(previous != nullptr)
        return (*previous)[n];
    throw std::runtime_error(static_cast<std::string>(n) + " not found");
}

object::objectPtr& stack::insert(const name& n, const object::objectPtr& obj)
{
    return storage.insert_or_assign(n, obj).first->second;
}

void stack::clear()
{
    storage.clear();
}

stack::iterator stack::begin()
{
    return storage.begin();
}
stack::iterator stack::end()
{
    return storage.end();
}

object::objectPtr stack::toObject()
{
    auto obj = makeUndefined();
    toObject(obj);
    return obj;
}

void stack::toObject(object::objectPtr& obj)
{
    if (previous != nullptr)
        previous->toObject(obj);
        obj->addProperties(begin(), end());
}