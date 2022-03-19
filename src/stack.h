#ifndef STACK_H_
#define STACK_H_

#include <memory>
#include "name.h"
#include "objectPtrImpl.h"
#include "allocator.h"

/// Stack for Easypt runtime.
class stack
{
public:
    using storageType = std::unordered_map<name, objectPtrImpl, std::hash<name>, std::equal_to<name>, allocator<std::pair<const name, objectPtrImpl>>>;
    // Optimization: small stack is stored in array.
    static constexpr size_t immediateStorageSize = 8;
    using immediateStorageType = std::array<std::pair<name, objectPtrImpl>, immediateStorageSize>;
    using iterator = storageType::iterator;

    stack() : parent(nullptr), immediateIndex(0) {}
    stack(stack *st) : parent(st), immediateIndex(0) {}

    stack(stack &&) = default;
    stack &operator=(stack &&) = default;
    stack(const stack &) = delete;
    stack &operator=(const stack &) = delete;

    /// Access specified element. Throws if element not found.
    objectPtrImpl &operator[](const name &n);
    /// Inserts specified element. Assigns if element already exists.
    objectPtrImpl &insert(const name &n, const objectPtrImpl &obj);
    /// Copies stack elements to object properties
    void copyToObject(objectPtrImpl &obj, bool recursive = false);
    /// Creates new stack containing elements from this stack and its ancestors.
    stack copyToFlatStack();
    /// Clears the contents.
    void clear();
    /// Erases specified element.
    void erase(const name &n);
    /// Reserve place for count elements
    void reserve(size_t count);

private:
    /// Helper for copyToFlatStack
    void copyToFlatStack(stack &st);
    /// Copies elements from immediateStorage to storage
    void moveToMap()
    {
        storage.insert(immediateStorage.begin(), immediateStorage.end());
        storage.erase(n::empty);
        immediateIndex = immediateStorage.size();
    }
    
    immediateStorageType immediateStorage;
    storageType storage;
    stack *parent;
    uint8_t immediateIndex;
};

#endif /* !STACK_H_ */
