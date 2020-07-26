#ifndef STACK_H_
#define STACK_H_

#include <memory>
#include "name.h"
#include "objectPtrImpl.h"
#include "allocator.h"

class stack
{
public:
    using storageType = std::unordered_map<name, objectPtrImpl, std::hash<name>, std::equal_to<name>, allocator<std::pair<const name, objectPtrImpl>>>;
    using iterator = storageType::iterator;

    stack() : previous(nullptr) {}
    stack(stack *st) : previous(st) {}

    stack(stack&&) = default;
    stack& operator=(stack&&) = default;
    stack(const stack&) = delete;
    stack& operator=(const stack&) = delete;

    objectPtrImpl &operator[](const name &n);
    objectPtrImpl &insert(const name &n, const objectPtrImpl &obj);
    iterator begin();
    iterator end();
    objectPtrImpl toObject();
    stack flatCopy();
    void clear();
    void erase(const name &n);

private:
    void toObject(objectPtrImpl& obj);
    void flatCopy(stack& st);
    stack *previous;
    storageType storage;
};

#endif /* !STACK_H_ */
