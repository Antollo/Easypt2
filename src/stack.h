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
    static constexpr size_t immediateStorageSize = 8;
    using immediateStorageType = std::array<std::pair<name, objectPtrImpl>, immediateStorageSize>;
    using iterator = storageType::iterator;

    stack() : previous(nullptr), immediateIndex(0) {}
    stack(stack *st) : previous(st), immediateIndex(0) {}

    stack(stack &&) = default;
    stack &operator=(stack &&) = default;
    stack(const stack &) = delete;
    stack &operator=(const stack &) = delete;

    objectPtrImpl &operator[](const name &n);
    objectPtrImpl &insert(const name &n, const objectPtrImpl &obj);

    void copyToObject(objectPtrImpl &obj, bool recursive = false);
    stack copyToFlatStack();
    void clear();
    void erase(const name &n);
    void reserve(size_t count)
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
    void keepAlive(const std::shared_ptr<stack> &st)
    {
        keepAlivePtr = st;
    }

private:
    void copyToFlatStack(stack &st);
    void moveToMap()
    {
        storage.insert(immediateStorage.begin(), immediateStorage.end());
        storage.erase(n::empty);
        immediateIndex = immediateStorage.size();
    }
    storageType storage;
    immediateStorageType immediateStorage;
    std::shared_ptr<stack> keepAlivePtr;
    stack *previous;
    uint8_t immediateIndex;
};

#endif /* !STACK_H_ */
