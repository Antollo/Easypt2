#ifndef LIBRARY_H_
#define LIBRARY_H_

#pragma warning(disable : 4100)

#include "Node.h"
#include "nobject.h"
#include "runtime.h"
#include "common.h"
#include "console.h"
#include "defines.h"
#include "core.h"
#include "objectPtrImpl.h"

namespace operators
{
    void init(stack *st);
};

namespace Object
{
    void init(stack *st);
};

namespace Array
{
    void init(stack *st);
};

namespace String
{
    void init(stack *st);
};

namespace Number
{
    void init(stack *st);
};

namespace Boolean
{
    void init(stack *st);
};

namespace consoleObj
{
    void init(stack *st);
};

namespace File
{
    void init(stack *st);
};

namespace Class
{
    void init(stack *st);
};

namespace Promise
{
    void init(stack *st);
};

namespace Time
{
    void init(stack *st);
};

namespace Tcp
{
    void init(stack *st);
};

namespace Accessible
{
    void init(stack *st);
};

namespace ChildProcess
{
    void init(stack *st);
};

namespace Buffer
{
    void init(stack *st);
};

namespace Iterator
{
    void init(stack *st);
};

namespace Range
{
    void init(stack *st);
};
#endif /* !LIBRARY_H_ */
