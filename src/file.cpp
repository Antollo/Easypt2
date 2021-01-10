#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include "common.h"
#include "file.h"

void file::open(const std::string &_path, const char *mode)
{
    if (isOpen())
        close();
    f = std::fopen(_path.c_str(), mode);
    assert(isOpen(), std::string("open: file not open ") + std::strerror(errno));
    path = std::filesystem::absolute(_path);
    lastOperation = lastOperationType::openOrClose;
}

void file::open(FILE *_f)
{
    if (isOpen())
        close();
    f = _f;
    assert(isOpen(), "open: file not open");
    path.clear();
    lastOperation = lastOperationType::openOrClose;
}

void file::create(const std::string &_path)
{
    open(_path, "w+");
}

void file::close()
{
    assert(isOpen(), "close: file not open");
    std::fclose(f);
    f = nullptr;
    lastOperation = lastOperationType::openOrClose;
}

void file::remove()
{
    assert(std::filesystem::exists(path), "remove: file \"" + path.string() + "\" does not exist");
    auto p = path;
    if (isOpen())
        close();
    path.clear();
    assert(std::remove(p.string().c_str()) == 0, std::string("remove: ") + std::strerror(errno));
}

std::string file::read()
{
    readGuard("read");

    char c;
    while (std::isspace(c = std::getc(f)))
        ;
    std::ungetc(c, f);

    std::string result;
    result.reserve(16);
    while (!std::isspace(c = std::getc(f)) && c != EOF)
        result.push_back(c);
    std::ungetc(c, f);

    while (std::isspace(c = std::getc(f)) && c != '\n')
        ;
    std::ungetc(c, f);

    errorGuard("read");
    return result;
}

std::string file::readTo(char delim)
{
    readGuard("readTo");

    char c;
    while ((c = std::getc(f)) == delim)
        ;
    std::ungetc(c, f);

    std::string result;
    result.reserve(32);
    while ((c = std::getc(f)) != delim && c != EOF)
        result.push_back(c);
    errorGuard("readTo");
    return result;
}

std::string file::readBytes(size_t count)
{
    readGuard("readBytes");

    std::string result(count + 1, 0);
    size_t pos = 0;
    while (pos < count && std::fgets(&result[pos], count - pos + 1, f))
        pos += std::strlen(&result[pos]);
    result.resize(pos);

    errorGuard("readBytes");
    return result;
}

void file::write(const std::string &str)
{
    writeGuard("write");
    std::fputs(str.c_str(), f);
    errorGuard("write");
}

void file::writeLine(const std::string &str)
{
    writeGuard("writeLine");
    std::fputs(str.c_str(), f);
    std::putc('\n', f);
    errorGuard("writeLine");
}

void file::newLine()
{
    writeGuard("newLine");
    std::putc('\n', f);
    errorGuard("newLine");
}

size_t file::getReadPosition()
{
    readGuard("getReadPosition");
    return std::ftell(f);
}

void file::setReadPosition(size_t n)
{
    readGuard("setReadPosition");
    std::fseek(f, n, SEEK_SET);
    errorGuard("setReadPosition");
}

size_t file::getWritePosition()
{
    writeGuard("getWritePosition");
    return std::ftell(f);
}

void file::setWritePosition(size_t n)
{
    writeGuard("setWritePosition");
    std::fseek(f, n, SEEK_SET);
    errorGuard("setWritePosition");
}

void file::clear()
{
    assert(isOpen(), "clear: file not open");
    f = std::freopen(nullptr, "w+", f);
    assert(isOpen(), "clear: failed to reopen");
}

void file::readGuard(const std::string &op)
{
    assert(isOpen(), op + ": file not open");
    if (lastOperation != lastOperationType::read)
    {
        std::fflush(f);
        std::fseek(f, 0, SEEK_SET);
        lastOperation = lastOperationType::read;
    }
}

void file::writeGuard(const std::string &op)
{
    assert(isOpen(), op + ": file not open");
    if (lastOperation != lastOperationType::write)
    {
        std::fflush(f);
        std::fseek(f, 0, SEEK_SET);
        lastOperation = lastOperationType::write;
    }
}

size_t file::size()
{
    assert(isOpen(), "size: file not open");
    size_t pos = std::ftell(f);
    fseek(f, 0, SEEK_END);
    size_t result = ftell(f);
    fseek(f, pos, SEEK_SET);
    return result;
}

void file::flush()
{
    assert(isOpen(), "flush: file not open");
    std::fflush(f);
}

void file::errorGuard(const std::string &op)
{
    if (std::ferror(f))
    {
        std::clearerr(f);
        assert(false, op + ": " + std::strerror(errno));
    }
}