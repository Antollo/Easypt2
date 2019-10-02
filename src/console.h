#ifndef CONSOLE_H_
#define CONSOLE_H_
#include <type_traits>
#include <iostream>
#include "osDependent.h"

namespace console
{

#if defined(_WIN32)
template <class T, class... Ts>
void read(T &t, Ts &... args)
{
    if constexpr (std::is_same_v<T, std::string>)
    {
        std::wstring temp;
        std::wcin >> temp;
        std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        t = utf8_encode(temp);
    }
    else
    {
        std::wcin >> t;
        std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    if constexpr (sizeof...(args))
        read(args...);
}

template <class T, class... Ts>
void readLine(T &t, Ts &... args)
{
    if constexpr (std::is_same_v<T, std::string>)
    {
        std::wstring temp;
        std::getline(std::wcin, temp);
        t = utf8_encode(temp);
    }
    else
    {
        std::wstring temp;
        std::getline(std::wcin, temp);
        t = static_cast<T>(utf8_encode(temp));
    }
    if constexpr (sizeof...(args))
        readLine(args...);
}

template <class T, class... Ts>
void write(const T &t, const Ts &... args)
{
    if constexpr (std::is_same_v<T, std::string>)
        std::wcout << utf8_decode(t);
    else
        std::wcout << t;
    if constexpr (sizeof...(args))
        write(args...);
}

template <class T, class... Ts>
void writeLine(const T &t, const Ts &... args)
{
    if constexpr (std::is_same_v<T, std::string>)
        std::wcout << utf8_decode(t);
    else
        std::wcout << t;
    if constexpr (sizeof...(args))
        write(args...);
    std::wcout << std::endl;
}

inline void newLine()
{
    std::wcout << std::endl;
}

template <class T, class... Ts>
void error(const T &t, const Ts &... args)
{
    std::wcout << utf8_decode("\033[91m\033[1m"s);
    if constexpr (std::is_same_v<T, std::string>)
        std::wcout << utf8_decode(t);
    else
        std::wcout << t;
    std::wcout << utf8_decode("\033[0m"s) << std::endl;
    if constexpr (sizeof...(args))
        error(args...);
}
#else
template <class T, class... Ts>
void read(T &t, Ts &... args)
{
    std::cin >> t;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if constexpr (sizeof...(args))
        read(args...);
}

template <class T, class... Ts>
void readLine(T &t, Ts &... args)
{
    std::string temp;
    std::getline(std::cin, temp);
    t = static_cast<T>(temp);
    if constexpr (sizeof...(args))
        readLine(args...);
}

template <class T, class... Ts>
void write(const T &t, const Ts &... args)
{
    std::cout << t;
    if constexpr (sizeof...(args))
        write(args...);
}

template <class T, class... Ts>
void writeLine(const T &t, const Ts &... args)
{
    std::cout << t;
    if constexpr (sizeof...(args))
        write(args...);
    std::cout << std::endl;
}

inline void newLine()
{
    std::cout << std::endl;
}

template <class T, class... Ts>
void error(const T &t, const Ts &... args)
{
    std::cout << "\033[91m\033[1m"s;
    std::cout << t;
    std::cout << "\033[0m"s << std::endl;
    if constexpr (sizeof...(args))
        error(args...);
}
#endif
} // namespace console

#endif /* !CONSOLE_H_ */
