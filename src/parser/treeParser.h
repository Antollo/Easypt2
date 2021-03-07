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
    static void throwLater(const std::string &str);


    static inline std::string file;
    static inline std::string text;
    static inline Node* root = nullptr;

private:
    static void parseStream(std::istream &stream, Node &node);
    static inline std::istream* stream = nullptr;
    static inline std::string exception;
    static inline bool exceptionToThrow = false;
};

#endif /* !TREEPARSER_H_ */
