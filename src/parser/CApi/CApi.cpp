#include <deque>
#include "CApi.h"
#include "../../nobject.h"

#undef contextType
class contextType
{
public:
    contextType(stack &st) : localStack(&st) {}
    stack localStack;
    std::deque<object::objectPtr> temporaryObjects;
};

#undef objectPtrPtr
using objectPtrPtr = object::objectPtr*;

#undef closureType
using closureType = closure_type(objectPtrPtr, (contextType *));

void withContext(closureType closure, contextType *context)
{
    
}