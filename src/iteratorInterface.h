#ifndef SRC_PARSER_ITERATOR_INTERFACE_H
#define SRC_PARSER_ITERATOR_INTERFACE_H

#include "objectPtrImpl.h"

class objectIteratorInterface
{
public:
    virtual ~objectIteratorInterface() = default;
    virtual objectPtrImpl next() = 0;
};

#endif /* SRC_PARSER_ITERATOR_INTERFACE_H */
