#ifndef TREEPARSER_H_
#define TREEPARSER_H_

#include <string>
#include <istream>
#include "Node.h"

void parse();

class treeParser
{
public:
    static void parseFile(const std::string &filename, Node &node);
    static void parseString(const std::string &str, Node &node);
    static void read(char *buffer, int *bytesRead, int maxBytesToRead);

    static std::string file;
    static std::string text;
    static Node* root;

private:
    static void parseStream(std::istream &stream, Node &node);
    static std::istream* stream;
};

#endif /* !TREEPARSER_H_ */
