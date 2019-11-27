#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include "common.h"
#include "osDependent.h"
#include "file.h"

file::file()
{
    _f = std::make_shared<decltype(_f)::element_type>();
    lastOperation = lastOperationType::openOrClose;
}
file::file(const std::string &path) : file()
{
    open(path);
}
void file::open(const std::string &path)
{
    (*_f).open(path, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    _path = path;
}
void file::create(const std::string &path)
{
    (*_f).open(path, std::ios_base::in | std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    _path = path;
}
void file::remove()
{
    if ((*_f).is_open())
        close();
    if (std::remove(_path.string().c_str()) != 0)
    {
        throw std::runtime_error(std::strerror(errno));
    }
}
int file::getReadPosition()
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    return (*_f).tellg();
}
void file::setReadPosition(const int &n)
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    (*_f).seekg(n);
}
int file::getWritePosition()
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    return (*_f).tellp();
}
void file::setWritePosition(const int &n)
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    (*_f).seekp(n);
}
void file::clear()
{
    (*_f).close();
    create(_path.string());
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
}
void file::close()
{
    (*_f).close();
    lastOperation = lastOperationType::openOrClose;
}
void file::newLine()
{
    (*_f) << '\n';
    writeGuard();
}
std::string file::read()
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");

    std::string temp;
    (*_f) >> temp;

    readGuard();
    return temp;
}
std::string file::readLine()
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    std::string temp;
    std::getline((*_f) >> std::ws, temp);

    readGuard();
    return temp;
}
std::string file::readTo(char delim)
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    std::string temp;
    std::getline((*_f) >> std::ws, temp, delim);
    
    readGuard();
    return temp;
}
std::string file::readBytes(const int& count)
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    std::string temp(count, ' ');
    (*_f).read(temp.data(), count);
    temp.resize((*_f).gcount());
    
    readGuard();
    return temp;
}
void file::write(const std::string &str)
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    (*_f)<<str;

    writeGuard();
}
void file::writeLine(const std::string &str)
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    (*_f)<<str<<'\n';
    
    writeGuard();
}
void file::readGuard()
{
    if (lastOperation != lastOperationType::read)
    {
        (*_f).flush();
        (*_f).sync();
        (*_f).seekp(0, std::ios::beg);
        lastOperation = lastOperationType::read;
    }
}
void file::writeGuard()
{
    if (lastOperation != lastOperationType::write)
    {
        (*_f).flush();
        (*_f).sync();
        (*_f).seekg(0, std::ios::beg);
        lastOperation = lastOperationType::write;
    }
}