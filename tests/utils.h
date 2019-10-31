#ifndef UTILS_H_
#define UTILS_H_

#include <sstream>
#include <algorithm>
#include "console.h"

bool __staticAssert(std::string message)
{
    std::cerr<<message;
    std::exit(1);
}

#define _staticAssert(expression) (bool(expression)) || (__staticAssert(#expression))

#define staticAssert(...) _staticAssert((__VA_ARGS__))

#if defined(_WIN32)
template <class CB>
void in(const std::string &str, CB cb)
{
    std::wistringstream st(utf8_decode(str));
    std::wstreambuf *buf = std::wcin.rdbuf();
    std::wcin.rdbuf(st.rdbuf());
    cb();
    std::wcin.rdbuf(buf);
}

template <class CB>
std::string out(CB cb)
{
    std::wostringstream st;
    std::wstreambuf *buf = std::wcout.rdbuf();
    std::wcout.rdbuf(st.rdbuf());
    cb();
    std::wcout.rdbuf(buf);
    return utf8_encode(st.str());
}
//std::string utf8_encode(const std::wstring &wstr);
//std::wstring utf8_decode(const std::string &str);
#else
template <class CB>
void in(const std::string &str, CB cb)
{
    std::istringstream st(str);
    std::streambuf *buf = std::cin.rdbuf();
    std::cin.rdbuf(st.rdbuf());
    cb();
    std::cin.rdbuf(buf);
}

template <class CB>
std::string out(CB cb)
{
    std::ostringstream st;
    std::streambuf *buf = std::cout.rdbuf();
    std::cout.rdbuf(st.rdbuf());
    cb();
    std::cout.rdbuf(buf);
    return st.str();
}
#endif

#endif /* !UTILS_H_ */
