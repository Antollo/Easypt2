#ifndef OSDEPENDENT_H
#define OSDEPENDENT_H

#include <string>
#include <algorithm>

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

#if __GNUC__ == 7
#include <experimental/filesystem>
namespace std
{
namespace filesystem = std::experimental::filesystem;
};
#else
#include <filesystem>
#endif

#if defined(_WIN32)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#pragma comment(linker, "/STACK:134217728")
#include <Windows.h>
#include <eh.h>
#include <io.h>
#include <fcntl.h>
//using libraryType = HMODULE;
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#elif defined(__linux__)
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <linux/limits.h>
//#include <dlfcn.h>
//using libraryType = void *;

#include <cstdio>
#include <cstring>
#include <csignal>
#else
#pragma message("OS not fully supported")
//using libraryType = bool;
#endif

std::filesystem::path getExecutablePath();
void initialize();
void initializeThread();

/*class dynamicLibrary
{
public:
    dynamicLibrary();
    void loadLibrary(const std::string &fileName);
    void *getFunction(const std::string &functionName);
    ~dynamicLibrary();

private:
    libraryType library;
};*/

#if defined(_WIN32)
std::string utf8_encode(const std::wstring &wstr);
std::wstring utf8_decode(const std::string &str);
class useStdio
{
public:
    useStdio()
    {
        _setmode(_fileno(stdout), _O_TEXT);
        _setmode(_fileno(stdin), _O_TEXT);
    }
    ~useStdio()
    {
        _setmode(_fileno(stdout), _O_WTEXT);
        _setmode(_fileno(stdin), _O_WTEXT);
    }
};
#else
class useStdio
{
};
#endif

#endif // OSDEPENDENT_H