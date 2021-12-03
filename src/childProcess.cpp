#include <utility>
#include <stdexcept>

#include "osDependent.h"
#include "childProcess.h"

childProcess::childProcess(const std::string &command, std::vector<std::string> args, std::string cwd)
{
#ifdef _WIN32
    HANDLE childStdinRd = nullptr;
    HANDLE childStdinWr = nullptr;
    HANDLE childStdoutRd = nullptr;
    HANDLE childStdoutWr = nullptr;
    HANDLE childStderrRd = nullptr;
    HANDLE childStderrWr = nullptr;

    SECURITY_ATTRIBUTES saAttr;

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = true;
    saAttr.lpSecurityDescriptor = nullptr;

    if (!CreatePipe(&childStdinRd, &childStdinWr, &saAttr, 0))
    {
        throwError("childProcess", "CreatePipe");
    }

    if (!SetHandleInformation(childStdinWr, HANDLE_FLAG_INHERIT, 0))
    {
        CloseHandle(childStdinRd);
        CloseHandle(childStdinWr);
        throwError("childProcess", "SetHandleInformation");
    }

    if (!CreatePipe(&childStdoutRd, &childStdoutWr, &saAttr, 0))
    {
        CloseHandle(childStdinRd);
        CloseHandle(childStdinWr);
        throwError("childProcess", "CreatePipe");
    }

    if (!SetHandleInformation(childStdoutRd, HANDLE_FLAG_INHERIT, 0))
    {
        CloseHandle(childStdinRd);
        CloseHandle(childStdinWr);
        CloseHandle(childStdoutRd);
        CloseHandle(childStdoutWr);
        throwError("childProcess", "SetHandleInformation");
    }

    if (!CreatePipe(&childStderrRd, &childStderrWr, &saAttr, 0))
    {
        CloseHandle(childStdinRd);
        CloseHandle(childStdinWr);
        CloseHandle(childStdoutRd);
        CloseHandle(childStdoutWr);
        throwError("childProcess", "CreatePipe");
    }

    if (!SetHandleInformation(childStderrRd, HANDLE_FLAG_INHERIT, 0))
    {
        CloseHandle(childStdinRd);
        CloseHandle(childStdinWr);
        CloseHandle(childStdoutRd);
        CloseHandle(childStdoutWr);
        CloseHandle(childStderrRd);
        CloseHandle(childStderrWr);
        throwError("childProcess", "SetHandleInformation");
    }

    std::string commandline = command;
    for (const auto &arg : args)
        commandline += " \"" + replaceAll(arg, "\"", "\"\"") + "\"";

    STARTUPINFOA siStartInfo;
    PROCESS_INFORMATION piProcInfo;
    bool success = false;

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    ZeroMemory(&siStartInfo, sizeof(STARTUPINFOA));
    siStartInfo.cb = sizeof(STARTUPINFOA);
    siStartInfo.hStdInput = childStdinRd;
    siStartInfo.hStdOutput = childStdoutWr;
    siStartInfo.hStdError = childStderrWr;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    success = CreateProcessA(nullptr,
                             &commandline[0],
                             nullptr,
                             nullptr,
                             true,
                             0,
                             nullptr,
                             cwd.size() > 0 ? &cwd[0] : nullptr,
                             &siStartInfo,
                             &piProcInfo);

    if (!success)
    {
        CloseHandle(childStdinRd);
        CloseHandle(childStdinWr);
        CloseHandle(childStdoutRd);
        CloseHandle(childStdoutWr);
        CloseHandle(childStderrRd);
        CloseHandle(childStderrWr);
        throwError("childProcess", "CreateProcess");
    }

    CloseHandle(childStdinRd);
    CloseHandle(childStdoutWr);
    CloseHandle(childStderrWr);
    CloseHandle(piProcInfo.hThread);
    hProcess = piProcInfo.hProcess;

    input = _fdopen(_open_osfhandle(intptr_t(childStdinWr), O_WRONLY | _O_BINARY), "wb");
    output = _fdopen(_open_osfhandle(intptr_t(childStdoutRd), O_RDONLY | _O_BINARY), "rb");
    error = _fdopen(_open_osfhandle(intptr_t(childStderrRd), O_RDONLY | _O_BINARY), "rb");
#else
    int childStdin[2];
    int childStdout[2];
    int childStderr[2];

    int &childStdinRd = childStdin[0];
    int &childStdinWr = childStdin[1];
    int &childStdoutRd = childStdout[0];
    int &childStdoutWr = childStdout[1];
    int &childStderrRd = childStderr[0];
    int &childStderrWr = childStderr[1];

    int child;

    if (pipe(childStdin) < 0)
        throwError("childProcess", "pipe");

    if (pipe(childStdout) < 0)
    {
        ::close(childStdinRd);
        ::close(childStdinWr);
        throwError("childProcess", "pipe");
    }

    if (pipe(childStderr) < 0)
    {
        ::close(childStdinRd);
        ::close(childStdinWr);
        ::close(childStdoutRd);
        ::close(childStdoutWr);
        throwError("childProcess", "pipe");
    }

    child = fork();
    if (child == 0)
    {
        if (dup2(childStdinRd, STDIN_FILENO) == -1)
            exit(errno);

        if (dup2(childStdoutWr, STDOUT_FILENO) == -1)
            exit(errno);

        if (dup2(childStderrWr, STDERR_FILENO) == -1)
            exit(errno);

        ::close(childStdinRd);
        ::close(childStdinWr);
        ::close(childStdoutRd);
        ::close(childStdoutWr);
        ::close(childStderrRd);
        ::close(childStderrWr);

        std::vector<char *> argv(args.size(), nullptr);
        for (size_t i = 0; i < argv.size(); i++)
            argv[i] = &args[i][0];

        if (cwd.size() > 0)
            chdir(cwd.c_str());

        int code = execvp(command.c_str(), argv.data());
        exit(code);
    }
    else if (child > 0)
    {
        pid = child;
        ::close(childStdinRd);
        ::close(childStdoutWr);
        ::close(childStderrWr);

        output = fdopen(childStdoutRd, "r");
        input = fdopen(childStdinWr, "w");
        error = fdopen(childStderrRd, "r");
    }
    else
    {
        ::close(childStdinRd);
        ::close(childStdinWr);
        ::close(childStdoutRd);
        ::close(childStdoutWr);
        ::close(childStderrRd);
        ::close(childStderrWr);
        throwError("childProcess", "fork");
    }
#endif
}

childProcess::childProcess(childProcess &&rhs)
{
    input = rhs.input;
    output = rhs.output;
    error = rhs.error;

    rhs.input = nullptr;
    rhs.output = nullptr;
    rhs.error = nullptr;

#ifdef _WIN32
    hProcess = rhs.hProcess;
    rhs.hProcess = nullptr;
#else
    pid = rhs.pid;
    rhs.pid = 0;
#endif
}

childProcess &childProcess::operator=(childProcess &&rhs)
{
    close();

    input = rhs.input;
    output = rhs.output;
    error = rhs.error;

    rhs.input = nullptr;
    rhs.output = nullptr;
    rhs.error = nullptr;

#ifdef _WIN32
    hProcess = rhs.hProcess;
    rhs.hProcess = nullptr;
#else
    pid = rhs.pid;
    rhs.pid = 0;
#endif
    return *this;
}

int childProcess::wait()
{
#ifdef _WIN32
    if (hProcess != nullptr)
    {
        DWORD exitCode;
        if (WaitForSingleObject(hProcess, INFINITE) == WAIT_FAILED)
            throwError("wait", "WaitForSingleObject");
        if (!GetExitCodeProcess(hProcess, &exitCode))
            throwError("wait", "GetExitCodeProcess");
        close();
        return exitCode;
    }
#else
    if (pid != 0)
    {
        int exitCode;
        if (waitpid(pid, &exitCode, 0) == -1)
            throwError("wait", "waitpid");
        close();
        return exitCode;
    }
#endif
    else
        return -1;
}

bool childProcess::running()
{
#ifdef _WIN32
    if (hProcess != nullptr)
    {
        DWORD exitCode;
        if (!GetExitCodeProcess(hProcess, &exitCode))
            throwError("running", "GetExitCodeProcess");
        return exitCode == STILL_ACTIVE;
    }
#else
    if (pid != 0)
    {
        return ::kill(pid, 0) == 0;
    }
#endif
    else
        return false;
}

int childProcess::kill()
{
#ifdef _WIN32
    if (hProcess != nullptr)
    {
        DWORD exitCode;
        if (!TerminateProcess(hProcess, 0))
            throwError("kill", "TerminateProcess");
        if (!GetExitCodeProcess(hProcess, &exitCode))
            throwError("kill", "GetExitCodeProcess");
        close();
        return exitCode;
    }
#else
    if (pid != 0)
    {
        int exitCode;
        if (::kill(pid, SIGKILL) != 0)
            throwError("kill", "kill");
        if (waitpid(pid, &exitCode, 0) == -1)
            throwError("kill", "waitpid");
        close();
        return exitCode;
    }
#endif
    else
        return -1;
}

void childProcess::close()
{
#ifdef _WIN32
    if (hProcess != nullptr)
    {
        if (!CloseHandle(hProcess))
            throwError("close", "CloseHandle");
        hProcess = nullptr;
    }
#else
    pid = 0;
#endif
}

static std::string replaceAll(std::string str, const std::string &from, const std::string &to)
{
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos)
    {
        str.replace(pos, from.size(), to);
        pos += to.size();
    }
    return str;
}

void childProcess::throwError(const std::string &op, const std::string &f)
{
#ifdef _WIN32
    LPWSTR messageBuffer = nullptr;
    size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 nullptr,
                                 GetLastError(),
                                 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                 (LPWSTR)&messageBuffer,
                                 0,
                                 nullptr);

    std::string message = osDependant::utf8Encode(std::wstring(messageBuffer, size));
    LocalFree(messageBuffer);
#else
    std::string message(std::strerror(errno));
#endif
    throw std::runtime_error(op + ": " + f + ", " + message);
}