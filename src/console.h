#ifndef CONSOLE_H_
#define CONSOLE_H_
#include <type_traits>
#include <iostream>
#include <string>
#include <sstream>
#include "osDependent.h"

using namespace std::string_literals;

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
        writeLine("\033[93m\033[1m"s, now() + " ", args..., "\033[0m"s);
    }

    template <class... Ts>
    static void error(const Ts &... args)
    {
        writeLineError("\033[91m\033[1m"s, now() + " ", args..., "\033[0m"s);
    }

    template <class T>
    static inline void controlSequence(const T& code)
    {
        write("\033["s + code);
    }

#if defined(_WIN32)
    template <class T, class... Ts>
    static void read(T &t, Ts &... args)
    {
        if constexpr (std::is_same_v<T, std::string>)
        {
            std::wstring temp;
            std::wcin >> temp;
            //std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), std::wcin.peek());
            t = utf8_encode(temp);
        }
        else
        {
            std::wcin >> t;
            //std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), std::wcin.peek());
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
            std::getline(std::wcin >> std::ws, temp);
            t = utf8_encode(temp);
        }
        else
        {
            std::wstring temp;
            std::getline(std::wcin >> std::ws, temp);
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

    template <class CB>
    static void setInput(const std::string &str, CB cb)
    {
        std::wistringstream st(utf8_decode(str));
        std::wstreambuf *buf = std::wcin.rdbuf();
        std::wcin.rdbuf(st.rdbuf());
        cb();
        std::wcin.rdbuf(buf);
    }

    template <class CB>
    static std::string getOutput(CB cb)
    {
        std::wostringstream st;
        std::wstreambuf *buf = std::wcout.rdbuf();
        std::wcout.rdbuf(st.rdbuf());
        cb();
        std::wcout.rdbuf(buf);
        return utf8_encode(st.str());
    }

#else
    template <class T, class... Ts>
    static void read(T &t, Ts &... args)
    {
        std::cin >> t;
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if constexpr (sizeof...(args))
            read(args...);
    }

    template <class T, class... Ts>
    static void readLine(T &t, Ts &... args)
    {
        std::string temp;
        std::getline(std::cin >> std::ws, temp);
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

    template <class CB>
    static void setInput(const std::string &str, CB cb)
    {
        std::istringstream st(str);
        std::streambuf *buf = std::cin.rdbuf();
        std::cin.rdbuf(st.rdbuf());
        cb();
        std::cin.rdbuf(buf);
    }

    template <class CB>
    static std::string getOutput(CB cb)
    {
        std::ostringstream st;
        std::streambuf *buf = std::cout.rdbuf();
        std::cout.rdbuf(st.rdbuf());
        cb();
        std::cout.rdbuf(buf);
        return st.str();
    }

#endif

private:
    static std::string now();
};

#endif /* !CONSOLE_H_ */
