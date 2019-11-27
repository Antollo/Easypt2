#ifndef STACK_H_
#define STACK_H_

#include <memory>
#include "name.h"
#include "objectPtrImpl.h"

class stack
{
public:
    using storageType = std::unordered_map<name, objectPtrImpl>;
    using iterator = storageType::iterator;

    stack() : previous(nullptr) {}
    stack(stack *st) : previous(st) {}

    stack(const stack&) = delete;
    stack& operator=(const stack&) = delete;

    objectPtrImpl &operator[](const name &n);
    objectPtrImpl &insert(const name &n, const objectPtrImpl &obj);
    iterator begin();
    iterator end();
    objectPtrImpl toObject();
    void clear();

private:
    void toObject(objectPtrImpl& obj);
    stack *previous;
    storageType storage;
};

#endif /* !STACK_H_ */
