#ifndef NODE_H_
#define NODE_H_

#include <vector>
#include <memory>
#include "gramUtility.h"
#include "name.h"
#include "objectPtrImpl.h"
#include "stack.h"
#include "console.h"
#include "number.h"
#include "smallVector.h"

class Node
{
public:
    Node(int line = 0, const std::string &file = "invalid") : _token(0), _optimizations(0)
    {
        debugInfo[this]._line = line;
        debugInfo[this]._fileIndex = getFileIndex(file);
    }
    Node(const Node &node) = delete;
    Node(Node &&node)
    {
        _token = std::move(node._token);
        _optimizations = std::move(node._optimizations);
        _text = std::move(node._text);
        _children = std::move(node._children);
        _names = std::move(node._names);
        _value = std::move(node._value);

        debugInfo[this]._line = debugInfo[&node]._line;
        debugInfo[this]._fileIndex = debugInfo[&node]._fileIndex;
        debugInfo.erase(&node);
    }

    Node &operator=(const Node &node) = delete;
    Node &operator=(Node &&node)
    {
        _token = std::move(node._token);
        _optimizations = std::move(node._optimizations);
        _text = std::move(node._text);
        _children = std::move(node._children);
        _names = std::move(node._names);
        _value = std::move(node._value);

        debugInfo[this]._line = debugInfo[&node]._line;
        debugInfo[this]._fileIndex = debugInfo[&node]._fileIndex;
        debugInfo.erase(&node);
        return *this;
    }

    ~Node()
    {
        debugInfo.erase(this);
    }

    bool operator==(const Node &node) const
    {
        if (_token == IDENTIFIER && _token == node._token)
            return _text == node._text;
        return _token == node._token && _children == node._children;
    }

    void token(int t)
    {
        if (_token != 0)
            std::runtime_error("Token type already set");
        _token = t;
    }

    void text(const std::string &t);

    objectPtrImpl evaluate(stack &st) const;
    number evaluateNumber(stack &st) const;
    bool evaluateBoolean(stack &st) const;
    void evaluateVoid(stack &st) const;

    void addChild(Node &arg);
    void addName(const std::string &n)
    {
        _names.push_back(name(n));
    }
    smallVector<name> &names()
    {
        return _names;
    }
    const smallVector<name> &names() const
    {
        return _names;
    }
    smallVector<Node> &children()
    {
        return _children;
    }
    const smallVector<Node> &children() const
    {
        return _children;
    }
    void init(int line, std::string file)
    {
        debugInfo[this]._line = line;
        debugInfo[this]._fileIndex = getFileIndex(file);
    }
    bool shouldHaveStack() const
    {
        switch (_optimizations)
        {
        case stackOptimizations::hasNoStack:
            return false;
        case stackOptimizations::hasStack:
            return true;
        case stackOptimizations::notInitialized:
        {
            for (const auto &child : _children)
            {
                if (child.shouldHaveStackHelper())
                {
                    _optimizations = stackOptimizations::hasStack;
                    return true;
                }
            }
            _optimizations = stackOptimizations::hasNoStack;
            return false;
        }
        default:
            return true;
        }
    }

    void optimize()
    {
        switch (_token)
        {
        case COMPOUND_STATEMENT:
            if (!shouldHaveStack())
                _token = COMPOUND_STATEMENT_STACKLESS;
            break;

        case WHILE:
            if (_children[1]._token == COMPOUND_STATEMENT)
            {
                if (!_children[1].shouldHaveStack())
                    _token = WHILE_COMPOUND_STATEMENT_STACKLESS;
                else
                    _token = WHILE_COMPOUND_STATEMENT;
            }
            break;

        case FOR:
            if (_children[3]._token == COMPOUND_STATEMENT)
            {
                if (!_children[3].shouldHaveStack())
                    _token = FOR_COMPOUND_STATEMENT_STACKLESS;
                else
                    _token = FOR_COMPOUND_STATEMENT;
            }
            break;

        case INIT_ASSIGNMENT:
            if (_children[0]._token == LET)
                _token = INIT_ASSIGNMENT_LET;
            else if (_children[0]._token == IDENTIFIER)
                _token = INIT_ASSIGNMENT_IDENTIFIER;
            else if (_children[0]._token == DOT && _children[0]._children[1]._token == IDENTIFIER)
                _token = INIT_ASSIGNMENT_DOT;
            else
                throw std::runtime_error("left side of init_assignment is not identifier or dot operator");
            break;

        default:
            break;
        }

        for (auto &child : _children)
        {
            if (child._token == STATEMENT)
            {
                auto temp = std::move(child._children[0]);
                child = std::move(temp);
            }
            child.optimize();
        }
    }

private:
    friend class console;

    class breakType
    {
    };

    class stackOptimizations
    {
    public:
        static constexpr int8_t notInitialized = 0;
        static constexpr int8_t hasStack = 1;
        static constexpr int8_t hasNoStack = 2;
    };
    int _token;
    mutable int8_t _optimizations;
    name _text;
    smallVector<Node> _children;
    smallVector<name> _names;
    objectPtrImpl _value;

    std::string toName() const
    {
        if (_token == IDENTIFIER)
            return static_cast<std::string>(_text);
        return tokenToName(_token);
    }
    bool shouldHaveStackHelper() const
    {
        if (_token == COMPOUND_STATEMENT)
            return false;
        else if (_token == LET)
            return true;
        for (const auto &child : _children)
            if (child.shouldHaveStackHelper())
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
        stackTrace.push_back("exception at " + files[debugInfo[this]._fileIndex] + ":" + std::to_string(debugInfo[this]._line));
    }
    void exception(const std::string &str) const
    {
        stackTrace.push_back("exception at " + files[debugInfo[this]._fileIndex] + ":" + std::to_string(debugInfo[this]._line) + " (" + str + ")");
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

    static inline std::vector<std::string> stackTrace;
    static inline std::vector<std::string> files;
    class debugInfoType
    {
    public:
        int _line, _fileIndex;
    };
    static inline std::unordered_map<const Node *, debugInfoType> debugInfo;
};

#endif /* !NODE_H_ */
