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

/// Class responsible for console IO
class console
{
public:
    template <class... Ts>
    static void debug(const Ts &...args)
    {
        writeLine("\033[95m"s, now() + " ", args..., "\033[0m"s);
    }

    template <class... Ts>
    static void log(const Ts &...args)
    {
        writeLine("\033[96m"s, now() + " ", args..., "\033[0m"s);
    }

    template <class... Ts>
    static void ok(const Ts &...args)
    {
        writeLine("\033[92m\033[1m"s, now() + " ", args..., "\033[0m"s);
    }

    template <class... Ts>
    static void warn(const Ts &...args)
    {
        writeLine("\033[93m\033[1m"s, now() + " ", args..., "\033[0m"s);
    }

    template <class... Ts>
    static void error(const Ts &...args)
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
    static void WIO(read)(T &t, Ts &...args)
    {
        lastFormattedInputFunction = true;
        if constexpr (std::is_same_v<T, std::string>)
        {
            std::wstring temp;
            std::wcin >> temp;
            t = osDependant::utf8Encode(temp);
        }
        else
            std::wcin >> t;

        if constexpr (sizeof...(args))
            read(args...);
    }

    template <class T, class... Ts>
    static void WIO(readLine)(T &t, Ts &...args)
    {
        if constexpr (std::is_same_v<T, std::string>)
        {
            std::wstring temp;
            std::getline(lastFormattedInputFunction ? std::wcin >> std::ws : std::wcin, temp);
            t = osDependant::utf8Encode(temp);
        }
        else
        {
            std::wstring temp;
            std::getline(lastFormattedInputFunction ? std::wcin >> std::ws : std::wcin, temp);
            t = static_cast<T>(osDependant::utf8Encode(temp));
        }
        lastFormattedInputFunction = false;
        if constexpr (sizeof...(args))
            readLine(args...);
    }

    template <class T, class... Ts>
    static void WIO(write)(const T &t, const Ts &...args)
    {
        if constexpr (std::is_same_v<T, std::string>)
            std::wcout << osDependant::utf8Decode(t);
        else
            std::wcout << t;
        if constexpr (sizeof...(args))
            write(args...);
    }

    template <class T, class... Ts>
    static void WIO(writeLine)(const T &t, const Ts &...args)
    {
        if constexpr (std::is_same_v<T, std::string>)
            std::wcout << osDependant::utf8Decode(t);
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
    static void WIO(writeLineError)(const T &t, const Ts &...args)
    {
        if constexpr (std::is_same_v<T, std::string>)
            std::wcerr << osDependant::utf8Decode(t);
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
        std::wistringstream st(osDependant::utf8Decode(str));
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
        return osDependant::utf8Encode(st.str());
    }

#endif

    template <class... Ts>
    static void CIO(read)(Ts&... args)
    {
        lastFormattedInputFunction = true;
        (std::cin >> ... >> args);
    }

    template <class T, class... Ts>
    static void CIO(readLine)(T &t, Ts &...args)
    {
        std::string temp;
        std::getline(lastFormattedInputFunction ? std::cin >> std::ws : std::cin, temp);
        t = static_cast<T>(temp);
        lastFormattedInputFunction = false;
        if constexpr (sizeof...(args))
            readLine(args...);
    }

    template <class... Ts>
    static void CIO(write)(const Ts &...args)
    {
        (std::cout << ... << args);
    }

    template <class... Ts>
    static void CIO(writeLine)(const Ts &...args)
    {
        (std::cout << ... << args) << std::endl;
    }

    static inline void CIO(newLine)()
    {
        std::cout << std::endl;
    }

    template <class... Ts>
    static void CIO(writeLineError)(const Ts &...args)
    {
        (std::cerr << ... << args) << std::endl;
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
    static inline void IO(read)(Ts &&...args)
    {
        if (osDependant::isAttyInput())
            readW(std::forward<Ts>(args)...);
        else
            readC(std::forward<Ts>(args)...);
    }

    template <class... Ts>
    static inline void IO(readLine)(Ts &&...args)
    {
        if (osDependant::isAttyInput())
            readLineW(std::forward<Ts>(args)...);
        else
            readLineC(std::forward<Ts>(args)...);
    }

    template <class... Ts>
    static inline void IO(write)(Ts &&...args)
    {
        if (osDependant::isAttyOutput())
            writeW(std::forward<Ts>(args)...);
        else
            writeC(std::forward<Ts>(args)...);
    }

    template <class... Ts>
    static inline void IO(writeLine)(Ts &&...args)
    {
        if (osDependant::isAttyOutput())
            writeLineW(std::forward<Ts>(args)...);
        else
            writeLineC(std::forward<Ts>(args)...);
    }

    static inline void IO(newLine)()
    {
        if (osDependant::isAttyOutput())
            newLineW();
        else
            newLineC();
    }

    template <class... Ts>
    static inline void IO(writeLineError)(Ts &&...args)
    {
        if (osDependant::isAttyOutput())
            writeLineErrorW(std::forward<Ts>(args)...);
        else
            writeLineErrorC(std::forward<Ts>(args)...);
    }

    template <class CB>
    static inline void IO(setInput)(const std::string &str, CB &&cb)
    {
        if (osDependant::isAttyInput())
            setInputW(str, std::forward<CB>(cb));
        else
            setInputC(str, std::forward<CB>(cb));
    }

    template <class CB>
    static inline std::string IO(getOutput)(CB &&cb)
    {
        if (osDependant::isAttyOutput())
            return getOutputW(std::forward<CB>(cb));
        else
            return getOutputC(std::forward<CB>(cb));
    }

#endif

private:
    static std::string now();
    static bool lastFormattedInputFunction;
};

#endif /* !CONSOLE_H_ */
