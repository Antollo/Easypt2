#ifndef CONSOLE_H_
#define CONSOLE_H_
#include <type_traits>
#include <iostream>
#include <string>
#include <sstream>
#include "osDependent.h"

#ifdef _WIN32
#define CIO(x) x##C
#define WIO(x) x##W
#define IO(x) x
#else
#define CIO(x) x
#endif

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

    static void stackTrace();

    template <class T>
    static inline void controlSequence(const T &code)
    {
        write("\033["s + code);
    }

#ifdef _WIN32
    template <class T, class... Ts>
    static void WIO(read)(T &t, Ts &... args)
    {
        lastFormattedInputFunction = true;
        if constexpr (std::is_same_v<T, std::string>)
        {
            std::wstring temp;
            std::wcin >> temp;
            t = utf8Encode(temp);
        }
        else
            std::wcin >> t;

        if constexpr (sizeof...(args))
            read(args...);
    }

    template <class T, class... Ts>
    static void WIO(readLine)(T &t, Ts &... args)
    {
        if constexpr (std::is_same_v<T, std::string>)
        {
            std::wstring temp;
            std::getline(lastFormattedInputFunction ? std::wcin >> std::ws : std::wcin, temp);
            t = utf8Encode(temp);
        }
        else
        {
            std::wstring temp;
            std::getline(lastFormattedInputFunction ? std::wcin >> std::ws : std::wcin, temp);
            t = static_cast<T>(utf8Encode(temp));
        }
        lastFormattedInputFunction = false;
        if constexpr (sizeof...(args))
            readLine(args...);
    }

    template <class T, class... Ts>
    static void WIO(write)(const T &t, const Ts &... args)
    {
        if constexpr (std::is_same_v<T, std::string>)
            std::wcout << utf8Decode(t);
        else
            std::wcout << t;
        if constexpr (sizeof...(args))
            write(args...);
    }

    template <class T, class... Ts>
    static void WIO(writeLine)(const T &t, const Ts &... args)
    {
        if constexpr (std::is_same_v<T, std::string>)
            std::wcout << utf8Decode(t);
        else
            std::wcout << t;
        if constexpr (sizeof...(args))
            write(args...);
        std::wcout << std::endl;
    }

    static inline void WIO(newLine)()
    {
        std::wcout << std::endl;
    }

    template <class T, class... Ts>
    static void WIO(writeLineError)(const T &t, const Ts &... args)
    {
        if constexpr (std::is_same_v<T, std::string>)
            std::wcerr << utf8Decode(t);
        else
            std::wcerr << t;
        if constexpr (sizeof...(args))
            writeLineError(args...);
        else
            std::wcerr << std::endl;
    }

    template <class CB>
    static void WIO(setInput)(const std::string &str, CB &&cb)
    {
        std::wistringstream st(utf8Decode(str));
        std::wstreambuf *buf = std::wcin.rdbuf();
        bool temp = lastFormattedInputFunction;
        std::wcin.rdbuf(st.rdbuf());
        try
        {
            cb();
            std::wcin.rdbuf(buf);
            lastFormattedInputFunction = temp;
        }
        catch (...)
        {
            std::wcin.rdbuf(buf);
            lastFormattedInputFunction = temp;
            throw;
        }
    }

    template <class CB>
    static std::string WIO(getOutput)(CB &&cb)
    {
        std::wostringstream st;
        std::wstreambuf *buf = std::wcout.rdbuf();
        std::wcout.rdbuf(st.rdbuf());
        try
        {
            cb();
            std::wcout.rdbuf(buf);
        }
        catch (...)
        {
            std::wcout.rdbuf(buf);
            throw;
        }
        return utf8Encode(st.str());
    }

#endif

    template <class T, class... Ts>
    static void CIO(read)(T &t, Ts &... args)
    {
        lastFormattedInputFunction = true;
        std::cin >> t;
        if constexpr (sizeof...(args))
            read(args...);
    }

    template <class T, class... Ts>
    static void CIO(readLine)(T &t, Ts &... args)
    {
        std::string temp;
        std::getline(lastFormattedInputFunction ? std::cin >> std::ws : std::cin, temp);
        t = static_cast<T>(temp);
        lastFormattedInputFunction = false;
        if constexpr (sizeof...(args))
            readLine(args...);
    }

    template <class T, class... Ts>
    static void CIO(write)(const T &t, const Ts &... args)
    {
        std::cout << t;
        if constexpr (sizeof...(args))
            write(args...);
    }

    template <class T, class... Ts>
    static void CIO(writeLine)(const T &t, const Ts &... args)
    {
        std::cout << t;
        if constexpr (sizeof...(args))
            write(args...);
        std::cout << std::endl;
    }

    static inline void CIO(newLine)()
    {
        std::cout << std::endl;
    }

    template <class T, class... Ts>
    static void CIO(writeLineError)(const T &t, const Ts &... args)
    {
        std::cerr << t;
        if constexpr (sizeof...(args))
            writeLineError(args...);
        else
            std::cerr << std::endl;
    }

    template <class CB>
    static void CIO(setInput)(const std::string &str, CB &&cb)
    {
        std::istringstream st(str);
        std::streambuf *buf = std::cin.rdbuf();
        std::cin.rdbuf(st.rdbuf());
        bool temp = lastFormattedInputFunction;
        try
        {
            cb();
            std::cin.rdbuf(buf);
            lastFormattedInputFunction = temp;
        }
        catch (...)
        {
            std::cin.rdbuf(buf);
            lastFormattedInputFunction = temp;
            throw;
        }
    }

    template <class CB>
    static std::string CIO(getOutput)(CB &&cb)
    {
        std::ostringstream st;
        std::streambuf *buf = std::cout.rdbuf();
        std::cout.rdbuf(st.rdbuf());
        try
        {
            cb();
            std::cout.rdbuf(buf);
        }
        catch (...)
        {
            std::cout.rdbuf(buf);
            throw;
        }
        return st.str();
    }

#ifdef _WIN32
    template <class... Ts>
    static inline void IO(read)(Ts &&... args)
    {
        if (isAttyInput())
            WIO(read)
        (std::forward<Ts>(args)...);
        else CIO(read)(std::forward<Ts>(args)...);
    }

    template <class... Ts>
    static inline void IO(readLine)(Ts &&... args)
    {
        if (isAttyInput())
            WIO(readLine)
        (std::forward<Ts>(args)...);
        else CIO(readLine)(std::forward<Ts>(args)...);
    }

    template <class... Ts>
    static inline void IO(write)(Ts &&... args)
    {
        if (isAttyOutput())
            WIO(write)
        (std::forward<Ts>(args)...);
        else CIO(write)(std::forward<Ts>(args)...);
    }

    template <class... Ts>
    static inline void IO(writeLine)(Ts &&... args)
    {
        if (isAttyOutput())
            WIO(writeLine)
        (std::forward<Ts>(args)...);
        else CIO(writeLine)(std::forward<Ts>(args)...);
    }

    static inline void IO(newLine)()
    {
        if (isAttyOutput())
            WIO(newLine)
        ();
        else CIO(newLine)();
    }

    template <class... Ts>
    static inline void IO(writeLineError)(Ts &&... args)
    {
        if (isAttyOutput())
            WIO(writeLineError)
        (std::forward<Ts>(args)...);
        else CIO(writeLineError)(std::forward<Ts>(args)...);
    }

    template <class CB>
    static inline void IO(setInput)(const std::string &str, CB &&cb)
    {
        if (isAttyInput())
            WIO(setInput)
        (str, std::forward<CB>(cb));
        else CIO(setInput)(str, std::forward<CB>(cb));
    }

    template <class CB>
    static inline std::string IO(getOutput)(CB &&cb)
    {
        if (isAttyOutput())
            return WIO(getOutput)(std::forward<CB>(cb));
        else
            return CIO(getOutput)(std::forward<CB>(cb));
    }

#endif

private:
    static std::string now();
    static bool lastFormattedInputFunction;
};

#endif /* !CONSOLE_H_ */
