#ifndef SRC_ITERATOR_H
#define SRC_ITERATOR_H

#include <stdexcept>
#include "nobject.h"

class arrayIterator : public object::iteratorBase
{
public:
    using object::iteratorBase::objectIteratorBase;

    object::objectPtr next() override
    {
        auto &val = obj->get<object::type::Array>();
        if (pos < val.size())
            return val[pos++];
        else
            return object::iteratorEnd;
    }

private:
    size_t pos = 0;
};

class stringIterator : public object::iteratorBase
{
public:
    using object::iteratorBase::objectIteratorBase;

    object::objectPtr next() override
    {
        auto &val = obj->get<const object::type::String>();
        if (pos < val.size())
            return object::makeObject(std::string(1, val[pos++]));
        else
            return object::iteratorEnd;
    }

private:
    size_t pos = 0;
};

class objectIterator : public object::iteratorBase
{
public:
    using object::iteratorBase::objectIteratorBase;

    object::objectPtr next() override
    {
        auto &next = (*obj)[n::next];
        return (*next)(obj, {}, nullptr);
    }
};

class emptyIterator : public object::iteratorBase
{
public:
    emptyIterator()
        : object::iteratorBase(nullptr) { }

    object::objectPtr next() override
    {
        return object::iteratorEnd;
    }
};

#endif /* SRC_ITERATOR_H */
