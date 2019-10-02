#include "stack.h"

std::shared_ptr<object>& stack::operator[] (const name& n)
{
    if (storage.count(n))
        return storage[n];
    if(previous != nullptr)
        return (*previous)[n];
    throw std::runtime_error(static_cast<std::string>(n) + " not found");
}

std::shared_ptr<object>& stack::insert(const name& n, const std::shared_ptr<object>& obj)
{
    return storage.insert(std::make_pair(n, obj)).first->second;
}

void stack::clear()
{
    storage.clear();
}