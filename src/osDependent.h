#ifndef OSDEPENDENT_H
#define OSDEPENDENT_H

#include <string>
#include <algorithm>
#include <vector>
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

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#pragma comment(linker, "/STACK:4194304")
#pragma comment(linker, "/HEAP:4194304")
#include <Windows.h>
#include <eh.h>
#include <io.h>
#include <fcntl.h>
using libraryType = HMODULE;
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#elif defined(__linux__)
#include <cstring>
#include <csignal>
#include <cstdio>
#include <unistd.h>
#include <linux/limits.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/wait.h>
using libraryType = void *;
#else
#pragma message("OS not fully supported")
using libraryType = bool;
#endif

std::filesystem::path getExecutablePath();
void initialize();
void initializeThread();

class dynamicLibrary
{
public:
    static unsigned int loadLibrary(const std::filesystem::path &fileName);
    static void *getFunction(unsigned int library, const std::string &functionName);
    static void unloadLibraries();
private:
    static inline std::vector<libraryType> libraries;
};

#ifdef _WIN32
std::string utf8Encode(const std::wstring &wstr);
std::wstring utf8Decode(const std::string &str);
const bool isAttyInput();
const bool isAttyOutput();
#endif

#endif // OSDEPENDENT_H