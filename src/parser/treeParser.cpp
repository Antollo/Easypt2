#include <fstream>
#include <sstream>
#include "treeParser.h"
#include "console.h"
#include "osDependent.h"

std::string treeParser::file;
std::string treeParser::text;
Node *treeParser::root = nullptr;
std::istream *treeParser::stream = nullptr;

void treeParser::parseFile(const std::string &filename, Node &node)
{
    file = filename;
    std::ifstream stream(filename, std::ifstream::in);
    parseStream(stream, node);
}

void treeParser::parseString(const std::string &str, Node &node)
{
    if (str.size() > 8)
        file = "\"" + str.substr(0, 8) + "...\"";
    else
        file = "\"" + str + "\"";
    std::stringstream stream(str, std::stringstream::in);
    parseStream(stream, node);
}
void treeParser::parseStream(std::istream &_stream, Node &node)
{
    node = Node(0, file);
    root = &node;
    stream = &_stream;
    parse();
    stream = nullptr;
    extern int yylineno;
    yylineno = 1;
}

void treeParser::read(char *buffer, int *bytesRead, int maxBytesToRead)
{
    stream->read(buffer, maxBytesToRead);
    *bytesRead = stream->gcount();
}