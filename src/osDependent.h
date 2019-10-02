#ifndef OSDEPENDENT_H
#define OSDEPENDENT_H

#include <string>
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>

#if defined(_WIN32)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker, "/STACK:134217728")
#include <Windows.h>
#include <eh.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#elif defined(__linux__)
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <linux/limits.h>
#include <dlfcn.h>

#include <cstdio>
#include <cstring>
#include <csignal>
#include <ucontext.h>
#else
#pragma message("OS not fully supported")
#endif

std::filesystem::path getExecutablePath();
void initialize();
void initializeThread();

#if defined(_WIN32)
std::string utf8_encode(const std::wstring &wstr);
std::wstring utf8_decode(const std::string &str);
#endif

#endif // OSDEPENDENT_H