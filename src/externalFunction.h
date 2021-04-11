#ifndef E9B6A8DE_98E2_46B8_954C_0E6246396F11
#define E9B6A8DE_98E2_46B8_954C_0E6246396F11

#include <vector>
#include "objectPtrImpl.h"
#include "allocator.h"
#include "externalLibraryCommon.h"

class stack;

class externalFunction
{
public:
    externalFunction() = default;
    externalFunction(externalFunction &&) = default;
    externalFunction(const externalFunction &) = default;
    externalFunction &operator=(externalFunction &&) = default;
    externalFunction &operator=(const externalFunction &) = default;
    externalFunction(int64_t type, void *functionPtr) : _type(type), _functionPtr(functionPtr) {}
    bool operator==(const externalFunction &rhs) const
    {
        return _type == rhs._type && _functionPtr == rhs._functionPtr;
    }
    objectPtrImpl operator()(objectPtrImpl thisObj, std::vector<objectPtrImpl, allocator<objectPtrImpl>> &&args, stack *st);

private:
    int64_t _type;
    void *_functionPtr;
};

#endif /* E9B6A8DE_98E2_46B8_954C_0E6246396F11 */
