#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include <iostream>
#include <typeinfo>
#include "console.h"

template <class... Args>
struct isAlignmentOk
{
    static constexpr bool value = true;
};

template <class A, class B, class... Args>
struct isAlignmentOk<A, B, Args...>
{
    static constexpr bool value = alignof(A) >= alignof(B) && isAlignmentOk<B, Args...>::value;
};

template <class... Args>
inline constexpr bool isAlignmentOkV = isAlignmentOk<Args...>::value;

template <class... Args>
struct checkAlignment 
{
    static inline std::string value = "";
};

template <class A, class... Args>
struct checkAlignment<A, Args...> : checkAlignment<Args...>
{
    static inline std::string value = std::string(typeid(A).name()) + " " + std::to_string(alignof(A)) + "\n" + checkAlignment<Args...>::value;
};

template <class... Args>
inline std::string checkAlignmentV = checkAlignment<Args...>::value;

#endif /* ALIGNMENT_H */
