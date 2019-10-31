#include <fstream>
#include <iostream>
#include "common.h"
#include "osDependent.h"
#include "file.h"

file::file()
{
    _f = std::make_shared<decltype(_f)::element_type>();
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
    (*_f).open(path, std::ios_base::in | std::ios_base::out | std::ios_base::binary, std::ios_base::trunc);
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    _path = path;
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
    (*_f).open(_path);
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
}
void file::close()
{
    (*_f).close();
}
void file::newLine()
{
    (*_f) << std::endl;
}

#if defined(WIDE_FILES)
std::string file::read()
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    std::wstring temp;
    (*_f) >> temp;
    return utf8_encode(temp);
}
std::string file::readLine()
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    std::wstring temp;
    std::getline((*_f) >> std::ws, temp);
    return utf8_encode(temp);
}
std::string file::readTo(char delim)
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    std::wstring temp;
    std::getline((*_f) >> std::ws, temp, static_cast<wchar_t>(delim));
    return utf8_encode(temp);
}
std::string file::readBytes(const int& count)
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    std::wstring temp(count, ' ');
    (*_f).read(temp.data(), count);
    temp.resize((*_f).gcount());
    return utf8_encode(temp);
}
void file::write(const std::string &str)
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    (*_f)<<utf8_decode(str);
}
void file::writeLine(const std::string &str)
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    (*_f)<<utf8_decode(str)<<std::endl;
}
#else
std::string file::read()
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    std::string temp;
    (*_f) >> temp;
    return temp;
}
std::string file::readLine()
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    std::string temp;
    std::getline((*_f) >> std::ws, temp);
    return temp;
}
std::string file::readTo(char delim)
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    std::string temp;
    std::getline((*_f) >> std::ws, temp, delim);
    return temp;
}
std::string file::readBytes(const int& count)
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    std::string temp(count, ' ');
    (*_f).read(temp.data(), count);
    temp.resize((*_f).gcount());
    return temp;
}
void file::write(const std::string &str)
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    (*_f)<<str;
}
void file::writeLine(const std::string &str)
{
    assert((*_f).is_open(), "file not opened");
    assert((*_f).good(), "file not good");
    (*_f)<<str<<std::endl;
}
#endif