#ifndef ASSERT_H
#define ASSERT_H

#include <string>
#include <stdexcept>
#include <functional>

#ifdef assert
#undef assert
#endif

template <class T>
void assert(const T &a, const std::string &message = "assertion error")
{
    if (a)
        return;
    throw std::runtime_error(message);
}

template <template <class> class O, class T, class U>
void assert(const T &a, const U &b, const std::string &message = "assertion error: ")
{
    if (O<T>()(a, b))
        return;
    if constexpr (std::is_same_v<O<T>, std::less<T>>)
        throw std::runtime_error(message + " " + std::to_string(a) + " < " + std::to_string(b));
    else if constexpr (std::is_same_v<O<T>, std::less_equal<T>>)
        throw std::runtime_error(message + " " + std::to_string(a) + " <= " + std::to_string(b));
    else if constexpr (std::is_same_v<O<T>, std::greater<T>>)
        throw std::runtime_error(message + " " + std::to_string(a) + " > " + std::to_string(b));
    else if constexpr (std::is_same_v<O<T>, std::greater_equal<T>>)
        throw std::runtime_error(message + " " + std::to_string(a) + " >= " + std::to_string(b));
    else if constexpr (std::is_same_v<O<T>, std::equal_to<T>>)
        throw std::runtime_error(message + " " + std::to_string(a) + " == " + std::to_string(b));
    else if constexpr (std::is_same_v<O<T>, std::not_equal_to<T>>)
        throw std::runtime_error(message + " " + std::to_string(a) + " != " + std::to_string(b));
    else
        throw std::runtime_error(message + std::to_string(a) + " op " + std::to_string(b));
}

#endif /* ASSERT_H */
