#ifndef NODE_H_
#define NODE_H_

#include <vector>
#include <memory>
#include <set>
#include "gramUtility.h"
#include "name.h"
#include "objectPtrImpl.h"
#include "stack.h"
#include "console.h"
#include "number.h"
#include "smallVector.h"

#define _evaluate(i) _children[i].evaluate(st)
#define _evaluateIdentifier(i) st[_children[i]._text]

#define compareObjects(a, b, opName, op, objectReturn, valueReturn)                                                  \
    switch (a->getTypeIndex())                                                                                       \
    {                                                                                                                \
    case object::typeIndex::Number:                                                                                  \
        if (b->isConvertible<object::type::Number>())                                                                \
            valueReturn(a->uncheckedGet<const object::type::Number>() op b->getConverted<object::type::Number>());   \
        else                                                                                                         \
            break;                                                                                                   \
    case object::typeIndex::String:                                                                                  \
        if (b->isConvertible<object::type::String>())                                                                \
            valueReturn(a->uncheckedGet<const object::type::String>() op b->getConverted<object::type::String>());   \
        else                                                                                                         \
            break;                                                                                                   \
    case object::typeIndex::Array:                                                                                   \
        if (b->isConvertible<object::type::Array>())                                                                 \
            valueReturn(a->uncheckedGet<const object::type::Array>() op b->getConverted<object::type::Array>());     \
        else                                                                                                         \
            break;                                                                                                   \
    case object::typeIndex::Boolean:                                                                                 \
        if (b->isConvertible<object::type::Boolean>())                                                               \
            valueReturn(a->uncheckedGet<const object::type::Boolean>() op b->getConverted<object::type::Boolean>()); \
        else                                                                                                         \
            break;                                                                                                   \
    }                                                                                                                \
    objectReturn((*(*a)[opName])(a, {b}, &st));

#define forCompareOperator(macro)        \
    macro(EQUAL, n::equal, ==);          \
    macro(NOT_EQUAL, n::notEqual, !=);   \
    macro(LESS, n::less, <);             \
    macro(LESS_EQUAL, n::lessEqual, <=); \
    macro(GREATER, n::greater, >);       \
    macro(GREATER_EQUAL, n::greaterEqual, >=);

#define compareNode(nodeName, opName, op)                                       \
    caseBinary(nodeName,                                                        \
               {                                                                \
                   assert(_children.size() == 2);                               \
                   compareObjects(a, b, opName, op, objectReturn, valueReturn); \
               })

#define caseUnary(TOKEN, ...)             \
    case TOKEN:                           \
    {                                     \
        auto a = _evaluate(0);            \
        __VA_ARGS__                       \
    }                                     \
    case TOKEN | A_IDENTIFIER:            \
    {                                     \
        auto &a = _evaluateIdentifier(0); \
        __VA_ARGS__                       \
    }

#define caseBinary(TOKEN, ...)            \
    case TOKEN:                           \
    {                                     \
        auto a = _evaluate(0);            \
        auto b = _evaluate(1);            \
        __VA_ARGS__                       \
    }                                     \
    case TOKEN | A_IDENTIFIER:            \
    {                                     \
        auto &a = _evaluateIdentifier(0); \
        auto b = _evaluate(1);            \
        __VA_ARGS__                       \
    }                                     \
    case TOKEN | B_IDENTIFIER:            \
    {                                     \
        auto a = _evaluate(0);            \
        auto &b = _evaluateIdentifier(1); \
        __VA_ARGS__                       \
    }                                     \
    case TOKEN | AB_IDENTIFIER:           \
    {                                     \
        auto &a = _evaluateIdentifier(0); \
        auto &b = _evaluateIdentifier(1); \
        __VA_ARGS__                       \
    }

#define allCases(TOKEN)        \
    case TOKEN:                \
    case TOKEN | A_IDENTIFIER: \
    case TOKEN | B_IDENTIFIER: \
    case TOKEN | AB_IDENTIFIER:

class Node
{
public:
    Node(size_t line = 0, const std::string &file = "invalid") : _token(0), _optimizations(0)
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

        debugInfo[this] = debugInfo[&node];
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

        debugInfo[this] = debugInfo[&node];
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
    void init(size_t line, std::string file)
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

    void optimize(bool topLevel = true)
    {
        switch (_token)
        {
        case COMPOUND_STATEMENT:
            if (!shouldHaveStack())
                _token = COMPOUND_STATEMENT_STACKLESS;
            if (topLevel && !_children.empty() && _children.back()._token == RETURN)
                _children.back()._token = RETURN_LAST;
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

        case AWAIT:
        case DOT:
        case ASSIGNMENT:
        case SPREAD_OPERATOR:
        case AND:
        case OR:
        case INCREMENT:
        case DECREMENT:
        case NOT:
        case COMPLEMENT:
        case RETURN:
        case THROW:
        case CALL_OPERATOR:
        case READ_OPERATOR:
        case METHOD_CALL_OPERATOR:

            if (_children[0]._token == IDENTIFIER)
                _token |= A_IDENTIFIER;
            break;

        case ADDITION:
        case SUBTRACTION:
        case MULTIPLICATION:
        case DIVISION:
        case MODULUS:
        case USER_OPERATOR:
        case INSTANCEOF:
        case BITWISE_AND:
        case BITWISE_OR:
        case SHIFT_LEFT:
        case SHIFT_RIGHT:
        case EQUAL:
        case NOT_EQUAL:
        case LESS:
        case GREATER:
        case LESS_EQUAL:
        case GREATER_EQUAL:
            if (_children[0]._token == IDENTIFIER)
                _token |= A_IDENTIFIER;
            if (_children[1]._token == IDENTIFIER)
                _token |= B_IDENTIFIER;
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
            child.optimize(false);
        }
    }

    std::string explain(size_t indentation = 0) const
    {
        std::stringstream result;
        result << std::string(indentation, '\t') << toName() << '\n';
        for (const auto &child : _children)
            result << child.explain(indentation + 1);
        return result.str();
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

    static constexpr int A_IDENTIFIER = 1 << 13;
    static constexpr int B_IDENTIFIER = 1 << 14;
    static constexpr int AB_IDENTIFIER = A_IDENTIFIER | B_IDENTIFIER;
    static constexpr int AB_MASK = 0xFFF;

    int _token;
    mutable int8_t _optimizations;
    name _text;
    smallVector<Node> _children;
    smallVector<name> _names;
    objectPtrImpl _value;

    static std::string getSymbol(int token)
    {
        const auto symbol = LLgetSymbol(token);
        if (symbol)
            return symbol;
        else
            return std::to_string(token);
    }

    std::string toName() const
    {
        std::stringstream result;
        if (_token == IDENTIFIER)
        {
            result << getSymbol(_token) << " \"" << static_cast<std::string>(_text) << "\"";
            return result.str();
        }
        result << getSymbol(_token & AB_MASK);
        if (_token & A_IDENTIFIER)
            result << " a";
        if (_token & B_IDENTIFIER)
            result << " b";
        const auto tokenLeftPart = _token >> 16;
        if (tokenLeftPart != 0)
        {
            result << " | " << getSymbol(tokenLeftPart & AB_MASK);
            if (tokenLeftPart & A_IDENTIFIER)
                result << " a";
            if (tokenLeftPart & B_IDENTIFIER)
                result << " b";
        }
        return result.str();
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
    static size_t getFileIndex(const std::string &file)
    {
        size_t fileIndex;
        auto fileIterator = std::find_if(files.begin(), files.end(), [&file](const auto &entry)
                                         { return entry == file; });
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
        for (int64_t i = stackTrace.size() - 1; i >= 0; i--)
            console::error(stackTrace[i]);
        stackTrace.clear();
    }
    static std::string parseString(const std::string &source);

    static inline std::vector<std::string> stackTrace;
    static inline std::vector<std::string> files;
    class debugInfoType
    {
    public:
        size_t _line, _fileIndex;
    };
    static inline std::unordered_map<const Node *, debugInfoType> debugInfo;
};

#endif /* !NODE_H_ */
