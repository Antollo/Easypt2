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
    Node(int line, std::string file) : _token(0), _line(line), _fileIndex(getFileIndex(file)), _optimizations(static_cast<uint8_t>(optimizations::notInitialized))
    {
    }
    Node(const Node &node) = delete;
    Node(Node &&node) = default;

    Node &operator=(const Node &node) = delete;
    Node &operator=(Node &&node) = default;

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
        _names.push_back(name(n));
    }
    std::vector<name> &names()
    {
        return _names;
    }
    const std::vector<name> &names() const
    {
        return _names;
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
        _line = line;
        _fileIndex = getFileIndex(file);
    }
    bool shouldHaveStack() const
    {
        switch (static_cast<Node::optimizations>(_optimizations))
        {
        case optimizations::notInitialized:
        {
            for (const auto &child : _children)
                if (child._shouldHaveStack())
                {
                    _optimizations = static_cast<uint_fast8_t>(optimizations::hasStack);
                    return true;
                }
            _optimizations = static_cast<uint_fast8_t>(optimizations::hasNoStack);
            return false;
        }
        case optimizations::hasNoStack:
            return false;
        default:
            return true;
        }
    }

private:
    friend class console;

    struct breakType
    {
    };

    int _token, _line, _fileIndex;
    mutable int_fast8_t _optimizations;
    enum class optimizations
    {
        notInitialized = 0,
        hasStack,
        hasNoStack
    };
    name _text;
    std::vector<Node> _children;
    std::vector<name> _names;
    objectPtrImpl _value;

    std::string toName() const
    {
        if (_token == IDENTIFIER)
            return static_cast<std::string>(_text);
        return tokenToName(_token);
    }
    bool _shouldHaveStack() const
    {
        if (_token == COMPOUND_STATEMENT)
            return false;
        else if (_token == LET)
            return true;
        for (const auto &child : _children)
            if (child._shouldHaveStack())
                return true;
        return false;
    }
    static int getFileIndex(const std::string &file)
    {
        int fileIndex;
        auto fileIterator = std::find_if(files.begin(), files.end(), [&file](const auto &entry) {
            return entry == file;
        });
        if (fileIterator == files.end())
        {
            fileIndex = files.size();
            files.push_back(file);
        }
        else
            fileIndex = std::distance(files.begin(), fileIterator);
        return fileIndex;
    }
    inline bool numberAssignmentOptimization(objectPtrImpl &a, objectPtrImpl &b, stack &st) const;
    void exception() const
    {
        stackTrace.push_back("exception at " + files[_fileIndex] + ":" + std::to_string(_line));
    }
    void exception(const std::string &str) const
    {
        stackTrace.push_back("exception at " + files[_fileIndex] + ":" + std::to_string(_line) + " (" + str + ")");
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
    static inline std::vector<std::string> files;
};

#endif /* !NODE_H_ */
