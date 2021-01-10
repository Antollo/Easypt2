#ifndef CHILDPROCESS_H
#define CHILDPROCESS_H

#include <string>
#include <cstdio>
#include <vector>

class childProcess
{
public:
    childProcess(const std::string &command, std::vector<std::string> args = {}, std::string cwd = "");
    childProcess(childProcess &&rhs);
    childProcess &operator=(childProcess &&rhs);
    childProcess(const childProcess &) = delete;
    childProcess &operator=(const childProcess &) = delete;

    FILE *&getInput()
    {
        return input;
    }

    FILE *&getOutput()
    {
        return output;
    }

    FILE *&getError()
    {
        return error;
    }

    int wait();
    bool running();
    int kill();

    ~childProcess()
    {
        close();
    }

private:
    FILE *input;
    FILE *output;
    FILE *error;

    void close();

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

    void throwError(const std::string &op, const std::string &f);

#ifdef _WIN32
    void* hProcess;
#else
    int pid = 0;
#endif
};

#endif /* CHILDPROCESS_H */
