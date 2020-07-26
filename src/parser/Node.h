#ifndef NODE_H_
#define NODE_H_

#include <vector>
#include <memory>
#include "gramUtility.h"
#include "gram.h"
#include "name.h"
#include "objectPtrImpl.h"
#include "stack.h"
#include "console.h"
#include "number.h"

class Node
{
public:
    Node(int line, std::string file) : _token(0)
    {
        data = std::make_shared<dataType>();
        data->line = line;
        data->file = file;
    }
    void token(int t) { _token = t; }
    void text(const std::string &t);

    std::string toString() const;
    objectPtrImpl evaluate(stack &st) const;
    number evaluateNumber(stack &st) const;
    bool evaluateBoolean(stack &st) const;
    void evaluateVoid(stack &st) const;

    void addChild(Node &arg);
    void addName(const std::string &n)
    {
        data->names.push_back(name(n));
    }
    std::vector<name> &names()
    {
        return data->names;
    }
    const std::vector<name> &names() const
    {
        return data->names;
    }
    std::vector<Node> &children()
    {
        return _children;
    }
    const std::vector<Node> &children() const
    {
        return _children;
    }
    void init(int line, std::string file)
    {
        data->line = line;
        data->file = file;
    }

private:
    friend class console;

    struct dataType
    {
        int line, numeric;
        std::string file;
        std::vector<name> names;
        objectPtrImpl value;
    };

    struct breakType
    {
    };

    int _token;
    name _text;
    std::vector<Node> _children;
    std::shared_ptr<dataType> data;

    std::string toName() const
    {
        if (_token == IDENTIFIER)
            return static_cast<std::string>(_text);
        return tokenToName(_token);
    }
    void exception() const
    {
        stackTrace.push_back("exception at " + data->file + ":" + std::to_string(data->line));
    }
    void exception(const std::string &str) const
    {
        stackTrace.push_back("exception at " + data->file + ":" + std::to_string(data->line) + " (" + str + ")");
    }
    static void printStackTrace()
    {
        if (stackTrace.empty())
            return;
        for (int i = stackTrace.size() - 1; i >= 0; i--)
            console::error(stackTrace[i]);
        stackTrace.clear();
    }
    static std::string parseString(const std::string &source);

    static std::vector<std::string> stackTrace;
};

#endif /* !NODE_H_ */
