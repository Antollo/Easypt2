#ifndef FILE_H_
#define FILE_H_

#include <string>
#include <fstream>
#include <filesystem>
class file
{
public:
    file();
    file(const std::string &path);
    void open(const std::string &path);
    void create(const std::string &path);
    void remove();
    std::string read();
    std::string readLine();
    std::string readTo(char delim);
    std::string readBytes(const int &count);
    void write(const std::string &str);
    void writeLine(const std::string &str);
    void newLine();
    int getReadPosition();
    void setReadPosition(const int &n);
    int getWritePosition();
    void setWritePosition(const int &n);
    void clear();
    void close();
    inline bool isOpen() { return _f->is_open(); }
    inline std::filesystem::path getPath() { return _path; }

private:
    std::shared_ptr<std::fstream> _f;
    std::filesystem::path _path;
    enum class lastOperationType
    {
        read,
        write,
        openOrClose
    };
    lastOperationType lastOperation;
    void readGuard();
    void writeGuard();
};

#endif /* !FILE_H_ */
