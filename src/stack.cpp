#include "stack.h"
#include "nobject.h"

object::objectPtr &stack::operator[](const name &n)
{
    if (immediateIndex < immediateStorage.size())
    {
        for (uint8_t i = 0; i <= immediateIndex; i++)
            if (immediateStorage[i].first == n)
                return immediateStorage[i].second;
    }
    else
    {
        auto it = storage.find(n);
        if (it != storage.end())
            return it->second;
    }
    if (previous != nullptr)
        return (*previous)[n];
    throw std::runtime_error(static_cast<std::string>(n) + " not found");
}

object::objectPtr &stack::insert(const name &n, const object::objectPtr &obj)
{
    if (immediateIndex < immediateStorage.size())
    {
        for (uint8_t i = 0; i < immediateStorage.size(); i++)
        {
            if (immediateStorage[i].first == n)
                return immediateStorage[i].second = obj;
        }
        if (++immediateIndex < immediateStorage.size())
            return (immediateStorage[immediateIndex] = {n, obj}).second;

        moveToMap();
    }
    return storage.insert_or_assign(n, obj).first->second;
}

void stack::clear()
{
    for (auto &element : immediateStorage)
        element = {name::empty, nullptr};
    immediateIndex = 0;
    storage.clear();
}

void stack::erase(const name &n)
{
    if (immediateIndex < immediateStorage.size())
    {
        for (uint8_t i = 0; i < immediateStorage.size(); i++)
            if (immediateStorage[i].first == n)
            {
                immediateStorage[i] = {name::empty, nullptr};
                break;
            }
    }
    else
        storage.erase(n);
}

object::objectPtr stack::toObject()
{
    auto obj = object::makeEmptyObject();
    copyToObject(obj, true);
    return obj;
}

void stack::copyToObject(object::objectPtr &obj, bool recursive)
{
    if (immediateIndex < immediateStorage.size())
        obj->addProperties(immediateStorage.begin(), immediateStorage.end());
    else
        obj->addProperties(storage.begin(), storage.end());
    if (recursive && previous != nullptr)
        previous->copyToObject(obj, recursive);
}

stack stack::flatCopy()
{
    stack st;
    flatCopy(st);
    return st;
}

void stack::flatCopy(stack &st)
{
    st.immediateIndex = immediateStorage.size();
    if (immediateIndex < immediateStorage.size())
    {
        st.storage.insert(immediateStorage.begin(), immediateStorage.end());
        st.storage.erase(name::empty);
    }
    else
        st.storage.insert(storage.begin(), storage.end());
    if (previous != nullptr)
        previous->flatCopy(st);
}