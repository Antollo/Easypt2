#ifndef SRC_PARSER_ITERATORBASE_H
#define SRC_PARSER_ITERATORBASE_H

#include "objectPtrImpl.h"

class objectIteratorBase
{
public:
    objectIteratorBase(const objectPtrImpl &obj)
        : obj(obj) {}
    
    objectIteratorBase(objectPtrImpl &&obj)
        : obj(obj) {}

    objectIteratorBase(const objectIteratorBase &) = default;
    objectIteratorBase(objectIteratorBase &&) = default;
    objectIteratorBase &operator=(const objectIteratorBase &) = default;
    objectIteratorBase &operator=(objectIteratorBase &&) = default;

    virtual ~objectIteratorBase() = default;

    virtual objectPtrImpl next() = 0;

protected:
    objectPtrImpl obj;
};

#endif /* SRC_PARSER_ITERATORBASE_H */
