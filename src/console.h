#ifndef CONSOLE_H_
#define CONSOLE_H_
#include <type_traits>
#include <iostream>
#include "osDependent.h"

class console
{
public:
    template <class... Ts>
    static void debug(const Ts &... args)
    {
        writeLine("\033[95m"s, now() + " ", args..., "\033[0m"s);
    }

    template <class... Ts>
    static void log(const Ts &... args)
    {
        writeLine("\033[96m"s, now() + " ", args..., "\033[0m"s);
    }

    template <class... Ts>
    static void warn(const Ts &... args)
    {
        writeLine("\033[93m\033[1m"s, now() + " ", args..., "\033[0m"s);    }

    template <class... Ts>
    static void error(const Ts &... args)
    {
        writeLineError("\033[91m\033[1m"s, now() + " ", args..., "\033[0m"s);
    }

#if defined(_WIN32)
    template <class T, class... Ts>
    static void read(T &t, Ts &... args)
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
    static void readLine(T &t, Ts &... args)
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
    static void write(const T &t, const Ts &... args)
    {
        if constexpr (std::is_same_v<T, std::string>)
            std::wcout << utf8_decode(t);
        else
            std::wcout << t;
        if constexpr (sizeof...(args))
            write(args...);
    }

    template <class T, class... Ts>
    static void writeLine(const T &t, const Ts &... args)
    {
        if constexpr (std::is_same_v<T, std::string>)
            std::wcout << utf8_decode(t);
        else
            std::wcout << t;
        if constexpr (sizeof...(args))
            write(args...);
        std::wcout << std::endl;
    }

    static inline void newLine()
    {
        std::wcout << std::endl;
    }

    template <class T, class... Ts>
    static void writeLineError(const T &t, const Ts &... args)
    {
        if constexpr (std::is_same_v<T, std::string>)
            std::wcerr << utf8_decode(t);
        else
            std::wcerr << t;
        if constexpr (sizeof...(args))
            writeLineError(args...);
        else
            std::wcerr << std::endl;
    }

#else
    template <class T, class... Ts>
    static void read(T &t, Ts &... args)
    {
        std::cin >> t;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if constexpr (sizeof...(args))
            read(args...);
    }

    template <class T, class... Ts>
    static void readLine(T &t, Ts &... args)
    {
        std::string temp;
        std::getline(std::cin, temp);
        t = static_cast<T>(temp);
        if constexpr (sizeof...(args))
            readLine(args...);
    }

    template <class T, class... Ts>
    static void write(const T &t, const Ts &... args)
    {
        std::cout << t;
        if constexpr (sizeof...(args))
            write(args...);
    }

    template <class T, class... Ts>
    static void writeLine(const T &t, const Ts &... args)
    {
        std::cout << t;
        if constexpr (sizeof...(args))
            write(args...);
        std::cout << std::endl;
    }

    static inline void newLine()
    {
        std::cout << std::endl;
    }

    template <class T, class... Ts>
    static void writeLineError(const T &t, const Ts &... args)
    {
        std::cerr << t;
        if constexpr (sizeof...(args))
            writeLineError(args...);
        else
            std::cerr << std::endl;
    }

#endif
private:
    static std::string now();
};

#endif /* !CONSOLE_H_ */
