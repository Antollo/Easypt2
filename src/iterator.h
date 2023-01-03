#ifndef SRC_ITERATOR_H
#define SRC_ITERATOR_H

#include <stdexcept>
#include "nobject.h"

class iteratorBase : public object::iteratorInterface
{
public:
    iteratorBase()
        : obj(nullptr) {}

    iteratorBase(const object::objectPtr &obj)
        : obj(obj) {}
    
    iteratorBase(object::objectPtr &&obj)
        : obj(obj) {}

    iteratorBase(const iteratorBase &) = default;
    iteratorBase(iteratorBase &&) = default;
    iteratorBase &operator=(const iteratorBase &) = default;
    iteratorBase &operator=(iteratorBase &&) = default;

protected:
    object::objectPtr obj;
};


class arrayIterator : public iteratorBase
{
public:
    using iteratorBase::iteratorBase;

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

class stringIterator : public iteratorBase
{
public:
    using iteratorBase::iteratorBase;

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

class objectIterator : public iteratorBase
{
public:
    using iteratorBase::iteratorBase;

    object::objectPtr next() override
    {
        auto &next = (*obj)[n::next];
        return (*next)(obj, {}, nullptr);
    }
};

class emptyIterator : public object::iteratorInterface
{
public:

    object::objectPtr next() override
    {
        return object::iteratorEnd;
    }
};

class rangeIterator : public object::iteratorInterface
{
public:
    rangeIterator(number start, number stop, number step)
        : current(start), stop(stop), step(step) {}

    rangeIterator(const range &r)
        : current(r.getStart()), stop(r.getStop()), step(r.getStep()) {}
    
    rangeIterator(range &&r)
        : current(r.getStart()), stop(r.getStop()), step(r.getStep()) {}

    object::objectPtr next() override
    {
        if ((step > 0_n && current < stop) || (step < 0_n && current > stop))
        {
            auto val = current;
            current += step;
            return object::makeObject(val);
        }
        else
            return object::iteratorEnd;
    }

private:
    number current, stop, step;
};

#endif /* SRC_ITERATOR_H */
