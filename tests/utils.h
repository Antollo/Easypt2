#ifndef UTILS_H_
#define UTILS_H_

#include <sstream>
#include <algorithm>
#include "console.h"

bool __staticAssert(std::string message)
{
    console::error(message);
    std::exit(1);
}

#define _staticAssert(expression) (bool(expression)) || (__staticAssert(#expression))

#define staticAssert(...) _staticAssert((__VA_ARGS__))


#endif /* !UTILS_H_ */
