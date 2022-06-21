#ifndef FILE_H_
#define FILE_H_

#include <cctype>
#include <cstdio>
#include <cstring>
#include <string>
#include <filesystem>

class stack;

namespace ChildProcess
{
    void init(stack *st);
};

class file
{
public:
    file() = default;

    file(const std::string &path) : file() { open(path); }
    file(FILE *f) : file() { open(f); }

    ~file()
    {
        if (isOpen())
            close();
    }

    file(const file &) = delete;
    file(file &&rhs)
    {
        f = rhs.f;
        lastOperation = rhs.lastOperation;
        path = rhs.path;
        rhs.f = nullptr;
        rhs.lastOperation = lastOperationType::openOrClose;
        rhs.path.clear();
    }

    file &operator=(const file &) = delete;
    file &operator=(file &&rhs)
    {
        if (isOpen())
            close();
        f = rhs.f;
        lastOperation = rhs.lastOperation;
        path = rhs.path;
        rhs.f = nullptr;
        rhs.lastOperation = lastOperationType::openOrClose;
        rhs.path.clear();
        return *this;
    }

    void open(const std::string &path, const char * = "rb+");
    void open(FILE *f);
    void create(const std::string &path);
    void remove();
    std::string read();
    std::string readTo(char delim);
    std::string readLine() { return readTo('\n'); }
    std::string readBytes(size_t count);
    void write(const std::string &str);
    void writeLine(const std::string &str);
    void newLine();
    size_t getReadPosition();
    void setReadPosition(size_t n);
    size_t getWritePosition();
    void setWritePosition(size_t n);
    void clear();
    void close();
    inline bool isOpen() { return f != nullptr; }
    std::filesystem::path getPath() { return path; }
    size_t size();
    void flush();

private:
    std::FILE *f = nullptr;
    std::filesystem::path path;
    enum class lastOperationType
    {
        read,
        write,
        openOrClose
    };
    lastOperationType lastOperation = lastOperationType::openOrClose;
    void readGuard(const std::string &op);
    void writeGuard(const std::string &op);
    void errorGuard(const std::string &op);

    friend void ChildProcess::init(stack *st);
};

#endif /* !FILE_H_ */
