#include "osDependent.h"
#include "error.h"
#include "console.h"

std::filesystem::path getExecutablePath()
{
    static std::string ret;
    if (!ret.empty())
        return ret;
#ifdef _WIN32
    char result[MAX_PATH];
    ret = std::string(result, GetModuleFileNameA(NULL, result, MAX_PATH));
#elif defined(__linux__)
    char result[PATH_MAX];
    ssize_t length = readlink("/proc/self/exe", result, PATH_MAX);
    ret = std::string(result, (length > 0) ? length : 0);
#else
    return "";
#endif
    return ret;
}

#ifdef _WIN32
void translateSEH(unsigned int u, EXCEPTION_POINTERS *)
{
    std::string name = [&u]() {
        switch (u)
        {
        case EXCEPTION_ACCESS_VIOLATION:
            return "EXCEPTION_ACCESS_VIOLATION";
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
        case EXCEPTION_BREAKPOINT:
            return "EXCEPTION_BREAKPOINT";
        case EXCEPTION_DATATYPE_MISALIGNMENT:
            return "EXCEPTION_DATATYPE_MISALIGNMENT";
        case EXCEPTION_FLT_DENORMAL_OPERAND:
            return "EXCEPTION_FLT_DENORMAL_OPERAND";
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
        case EXCEPTION_FLT_INEXACT_RESULT:
            return "EXCEPTION_FLT_INEXACT_RESULT";
        case EXCEPTION_FLT_INVALID_OPERATION:
            return "EXCEPTION_FLT_INVALID_OPERATION";
        case EXCEPTION_FLT_OVERFLOW:
            return "EXCEPTION_FLT_OVERFLOW";
        case EXCEPTION_FLT_STACK_CHECK:
            return "EXCEPTION_FLT_STACK_CHECK";
        case EXCEPTION_FLT_UNDERFLOW:
            return "EXCEPTION_FLT_UNDERFLOW";
        case EXCEPTION_ILLEGAL_INSTRUCTION:
            return "EXCEPTION_ILLEGAL_INSTRUCTION";
        case EXCEPTION_IN_PAGE_ERROR:
            return "EXCEPTION_IN_PAGE_ERROR";
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            return "EXCEPTION_INT_DIVIDE_BY_ZERO";
        case EXCEPTION_INT_OVERFLOW:
            return "EXCEPTION_INT_OVERFLOW";
        case EXCEPTION_INVALID_DISPOSITION:
            return "EXCEPTION_INVALID_DISPOSITION";
        case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
        case EXCEPTION_PRIV_INSTRUCTION:
            return "EXCEPTION_PRIV_INSTRUCTION";
        case EXCEPTION_SINGLE_STEP:
            return "EXCEPTION_SINGLE_STEP";
        case EXCEPTION_STACK_OVERFLOW:
            _resetstkoflw();
            console::error("Win32 exception EXCEPTION_STACK_OVERFLOW");
            std::exit(1);
        default:
            return "UNKNOWN EXCEPTION";
        }
    }();
    throw os_error("Win32 exception " + name);
}

std::string utf8Encode(const std::wstring &wstr)
{
    if (wstr.empty())
        return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

std::wstring utf8Decode(const std::string &str)
{
    if (str.empty())
        return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

const bool isAttyInput()
{
    static const bool attyInput = _isatty(_fileno(stdin));
    return attyInput;
}

const bool isAttyOutput()
{
    static const bool attyOutput = _isatty(_fileno(stdout));
    return attyOutput;
}

/*BOOL WINAPI consoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT)
        console::error("CTRL_C_EVENT");
    return TRUE;
}*/

#else

static void sigaction_segv(int signal, siginfo_t *si, void *arg)
{
    printf("Caught SIGSEGV, addr %p\n", si->si_addr);
    std::exit(1);
}
#endif

void initialize()
{
    initializeThread();
#ifdef _WIN32
    setlocale(LC_ALL, ".UTF8");

    if (isAttyInput())
        _setmode(_fileno(stdin), _O_WTEXT);
    else
        _setmode(_fileno(stdin), _O_BINARY);

    if (isAttyOutput())
        _setmode(_fileno(stdout), _O_WTEXT);
    else
        _setmode(_fileno(stdout), _O_BINARY);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
        return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
        return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
        return;
#endif
    console::write(std::boolalpha);
}

void initializeThread()
{
#ifdef _WIN32

    //Handle structured exceptions
    _set_se_translator(translateSEH);

    //Prevent stack overflow
    /*ULONG_PTR lowLimit;
    ULONG_PTR highLimit;
    GetCurrentThreadStackLimits(&lowLimit, &highLimit);
    static ULONG size = (highLimit - lowLimit) / 32 * 29;
    SetThreadStackGuarantee(&size);*/
#else
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = sigaction_segv;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGSEGV, &sa, NULL);
#endif
}

size_t dynamicLibrary::loadLibrary(const std::filesystem::path &filePath)
{
#ifdef _WIN32
    libraryType library = LoadLibraryA(filePath.string().c_str());
    if (!library)
        throw std::runtime_error("library " + filePath.string() + " not found");

#elif defined(__linux__)
    libraryType library = dlopen(filePath.string().c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (!library)
        throw std::runtime_error("library " + filePath.string() + " not found: " + dlerror());
#else
    throw std::runtime_error("dynamic libraries are not supported on this OS");
#endif
    libraries.push_back(library);
    return libraries.size() - 1;
}

void *dynamicLibrary::getFunction(size_t i, const std::string &functionName)
{
    if (i >= libraries.size())
        throw std::runtime_error("library does not exist");
#ifdef _WIN32
    void *function = reinterpret_cast<void *>(GetProcAddress(libraries[i], functionName.c_str()));
    if (!function)
        throw std::runtime_error("function " + functionName + " not found");
    return function;
#elif defined(__linux__)
    void *function = reinterpret_cast<void *>(dlsym(libraries[i], functionName.c_str()));
    if (!function)
        throw std::runtime_error("function " + functionName + " not found");
    return function;
#else
    throw std::runtime_error("dynamic libraries are not supported on this OS");
#endif
}

void dynamicLibrary::unloadLibraries()
{
    for (const auto &library : libraries)
#ifdef _WIN32
        FreeLibrary(library);
#elif defined(__linux__)
        dlclose(library);
#endif
}