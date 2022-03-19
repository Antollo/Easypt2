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
    if (parent != nullptr)
        return (*parent)[n];
    throw std::runtime_error(static_cast<std::string>(n) + " not found");
}

object::objectPtr &stack::insert(const name &n, const object::objectPtr &obj)
{
    if (immediateIndex < immediateStorage.size())
    {
        for (uint8_t i = 0; i <= immediateIndex; i++)
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
        element = {n::empty, nullptr};
    immediateIndex = 0;
    storage.clear();
}

void stack::erase(const name &n)
{
    if (immediateIndex < immediateStorage.size())
    {
        for (uint8_t i = 0; i <= immediateIndex; i++)
            if (immediateStorage[i].first == n)
            {
                immediateStorage[i] = {n::empty, nullptr};
                break;
            }
    }
    else
        storage.erase(n);
}

void stack::reserve(size_t count)
    {
        if (immediateIndex < immediateStorage.size())
        {
            if (count > immediateStorage.size())
            {
                moveToMap();
                storage.reserve(count);
            }
        }
        else
            storage.reserve(count);
    }

void stack::copyToObject(object::objectPtr &obj, bool recursive)
{
    if (immediateIndex < immediateStorage.size())
        obj->addProperties(immediateStorage.begin(), immediateStorage.end());
    else
        obj->addProperties(storage.begin(), storage.end());
    if (recursive && parent != nullptr)
        parent->copyToObject(obj, recursive);
}

stack stack::copyToFlatStack()
{
    stack st;
    copyToFlatStack(st);
    return st;
}

void stack::copyToFlatStack(stack &st)
{
    st.immediateIndex = immediateStorage.size();
    if (immediateIndex < immediateStorage.size())
    {
        st.storage.insert(immediateStorage.begin(), immediateStorage.end());
        st.storage.erase(n::empty);
    }
    else
        st.storage.insert(storage.begin(), storage.end());
    if (parent != nullptr)
        parent->copyToFlatStack(st);
}