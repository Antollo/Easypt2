#ifndef UTILS_H_
#define UTILS_H_

#include <sstream>
#include <algorithm>
#include "console.h"

bool __assert(std::string message)
{
    console::error(message);
    std::exit(1);
}

#define _assert(expression) (bool(expression)) || (__assert(#expression))

#define assert(...) _assert((__VA_ARGS__))


#endif /* !UTILS_H_ */
