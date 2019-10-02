#ifndef STACK_H_
#define STACK_H_

#include <memory>
#include "name.h"

class object;

class stack
{
public:
    stack() : previous(nullptr) {}
    stack(stack *st) : previous(st) {}
    std::shared_ptr<object>& operator[](const name &n);
    std::shared_ptr<object>& insert(const name &n, const std::shared_ptr<object> &obj);
    void clear();

private:
    stack *previous;
    std::unordered_map<name, std::shared_ptr<object>> storage;
};

#endif /* !STACK_H_ */
