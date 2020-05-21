#include <fstream>
#include <sstream>
#include <stdexcept>
#include "treeParser.h"
#include "console.h"
#include "osDependent.h"

void yyrestart(FILE *);

std::string treeParser::file;
std::string treeParser::text;
Node *treeParser::root = nullptr;
std::istream *treeParser::stream = nullptr;
std::string treeParser::exception;
bool treeParser::exceptionToThrow;

void treeParser::parseFile(const std::string &filename, Node &node)
{
    file = filename;
    std::ifstream fileStream(filename, std::ifstream::in);
    parseStream(fileStream, node);
}

void treeParser::parseString(const std::string &str, Node &node)
{
    if (str.size() > 8)
        file = "\"" + str.substr(0, 8) + "...\"";
    else
        file = "\"" + str + "\"";
    std::stringstream StringStream(str, std::stringstream::in);
    parseStream(StringStream, node);
}
void treeParser::parseStream(std::istream &_stream, Node &node)
{
    node = Node(0, file);
    root = &node;
    stream = &_stream;
    exceptionToThrow = false;
    exception.clear();
    extern int yylineno;
    yylineno = 0;
    //yyrestart(nullptr);
    parse();
    stream = nullptr;
    if (exceptionToThrow)
        throw std::runtime_error(exception);
}

void treeParser::read(char *buffer, int *bytesRead, int maxBytesToRead)
{
    stream->read(buffer, maxBytesToRead);
    *bytesRead = stream->gcount();
}

void treeParser::throwLater(const std::string &str)
{
    if (exceptionToThrow == false)
    {
        exceptionToThrow = true;
        exception = str;
    }
}