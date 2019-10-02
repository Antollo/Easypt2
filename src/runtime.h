#ifndef RUNTIME_H_
#define RUNTIME_H_

#include "token.h"

class runtime
{
public:
    static void init(stack* st);
    static void fini(stack* st);
};

#endif /* !RUNTIME_H_ */
