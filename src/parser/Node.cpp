#include "Node.h"
#include "nobject.h"
#include "console.h"
#include <cassert>
#include <exception>

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

#define caseBinary(TOKEN, ...)                \
    case TOKEN:                               \
    {                                         \
        auto a = _evaluate(0);                \
        auto b = _evaluate(1);                \
        __VA_ARGS__                           \
    }                                         \
    case TOKEN | A_IDENTIFIER:                \
    {                                         \
        auto &a = _evaluateIdentifier(0);     \
        auto b = _evaluate(1);                \
        __VA_ARGS__                           \
    }                                         \
    case TOKEN | B_IDENTIFIER:                \
    {                                         \
        auto a = _evaluate(0);                \
        auto &b = _evaluateIdentifier(1);     \
        __VA_ARGS__                           \
    }                                         \
    case TOKEN | AB_IDENTIFIER: \
    {                                         \
        auto &a = _evaluateIdentifier(0);     \
        auto &b = _evaluateIdentifier(1);     \
        __VA_ARGS__                           \
    }
#define allCases(TOKEN)                       \
    case TOKEN:                               \
    case TOKEN | A_IDENTIFIER:                \
    case TOKEN | B_IDENTIFIER:                \
    case TOKEN | AB_IDENTIFIER:               \

std::string Node::parseString(const std::string &source)
{
    std::string ret;
    size_t length = source.size() - 1;
    ret.reserve(length);
    for (size_t i = 1; i < length; i++)
    {
        if (source[i] != '\\')
            ret.push_back(source[i]);
        else if (i + 1 < length)
            switch (source[++i])
            {
            case '"':
                ret.push_back('"');
                break;
            case '\\':
                ret.push_back('\\');
                break;
            case 'a':
                ret.push_back('\a');
                break;
            case 'b':
                ret.push_back('\b');
                break;
            case 'f':
                ret.push_back('\f');
                break;
            case 'n':
                ret.push_back('\n');
                break;
            case 'r':
                ret.push_back('\r');
                break;
            case 't':
                ret.push_back('\t');
                break;
            case 'v':
                ret.push_back('\v');
                break;
            case '\n':
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (i + 2 > length)
                    throw std::runtime_error("wrong escape sequence on position " + std::to_string(i) + " in \"" + source + "\"");
                ret.push_back((source[i] - '0') * 100 + (source[i + 1] - '0') * 10 + (source[i + 2] - '0'));
                i += 2;
                break;
            }
        else
            throw std::runtime_error("wrong escape sequence on position " + std::to_string(i) + " in \"" + source + "\"");
    }
    return ret;
}

void Node::text(const std::string &t)
{
    switch (_token)
    {
    case NUMBER_LITERAL:
        _value = object::makeObject(static_cast<number>(t));
        _value->setConst();
        break;

    case STRING_LITERAL:
        _value = object::makeObject(parseString(t));
        _value->setConst();
        break;

    default:
        _text = name(t);
        break;
    }
}

void Node::addChild(Node &arg)
{
    switch (_token)
    {
    case FUNCTION:
    {
        arg.names() = std::move(names());
        names().clear();
        Node *node = new Node(std::move(arg));
        node->optimize();
        _value = object::makeObject(object::type::Function(node, nullptr));
        break;
    }
    default:
        _children.push_back(std::move(arg));
        break;
    }
}

bool Node::numberAssignmentOptimization(object::objectPtr &a, object::objectPtr &b, stack &st) const
{
    if (_optimizations >= 50)
    {
        try
        {
            if (_children[1]._children.size() == 2 && _children[0] == _children[1]._children[0])
            {
                switch (_children[1]._token)
                {
                case ADDITION:
                case ADDITION | A_IDENTIFIER:
                    a->get<number>() += _children[1]._children[1].evaluateNumber(st);
                    break;
                case ADDITION | B_IDENTIFIER:
                case ADDITION | AB_IDENTIFIER:
                    a->get<number>() += st[_children[1]._children[1]._text]->getConverted<object::type::Number>();
                    break;

                case SUBTRACTION:
                case SUBTRACTION | A_IDENTIFIER:
                    a->get<number>() -= _children[1]._children[1].evaluateNumber(st);
                    break;
                case SUBTRACTION | B_IDENTIFIER:
                case SUBTRACTION | AB_IDENTIFIER:
                    a->get<number>() -= st[_children[1]._children[1]._text]->getConverted<object::type::Number>();
                    break;

                case MULTIPLICATION:
                case MULTIPLICATION | A_IDENTIFIER:
                    a->get<number>() *= _children[1]._children[1].evaluateNumber(st);
                    break;
                case MULTIPLICATION | B_IDENTIFIER:
                case MULTIPLICATION | AB_IDENTIFIER:
                    a->get<number>() *= st[_children[1]._children[1]._text]->getConverted<object::type::Number>();
                    break;

                case DIVISION:
                case DIVISION | A_IDENTIFIER:
                    a->get<number>() /= _children[1]._children[1].evaluateNumber(st);
                    break;
                case DIVISION | B_IDENTIFIER:
                case DIVISION | AB_IDENTIFIER:
                    a->get<number>() /= st[_children[1]._children[1]._text]->getConverted<object::type::Number>();
                    break;

                case MODULUS:
                case MODULUS | A_IDENTIFIER:
                    a->get<number>() %= _children[1]._children[1].evaluateNumber(st);
                    break;
                case MODULUS | B_IDENTIFIER:
                case MODULUS | AB_IDENTIFIER:
                    a->get<number>() %= st[_children[1]._children[1]._text]->getConverted<object::type::Number>();
                    break;

                default:
                    a->get<number>() = _children[1].evaluateNumber(st);
                    break;
                }
            }
            else
                a->get<number>() = _children[1].evaluateNumber(st);
            return true;
        }
        catch (...)
        {
            console::warn("bad number optimization");
            _optimizations = 0;
        }
    }
    b = _children[1].evaluate(st);
    if (a->isOfType<number>() && b->isOfType<number>())
        _optimizations = _optimizations < 100 ? _optimizations + 1 : 100;
    else
        _optimizations = _optimizations > 0 ? _optimizations - 1 : 0;

    return false;
}

#define _evaluate(i) _children[i].evaluate(st)
#define _evaluateIdentifier(i) st[_children[i]._text]

object::objectPtr Node::evaluate(stack &st) const
{

    switch (_token)
    {
    case COMPOUND_STATEMENT:
    {
        size_t i = 0;
        stack localStack(&st);
        try
        {
            for (; i < _children.size(); i++)
                _children[i].evaluateVoid(localStack);
        }
        catch (objectException &e)
        {
            _children[i].exception();
            throw e;
        }
        catch (std::exception &e)
        {
            _children[i].exception();
            throw e;
        }
        return nullptr;
    }

    case COMPOUND_STATEMENT_STACKLESS:
    {
        size_t i = 0;
        try
        {
            for (; i < _children.size(); i++)
                _children[i].evaluateVoid(st);
        }
        catch (objectException &e)
        {
            _children[i].exception();
            throw e;
        }
        catch (std::exception &e)
        {
            _children[i].exception();
            throw e;
        }
        return nullptr;
    }

    case IF:
        assert(_children.size() == 2);
        if (_children[0].evaluateBoolean(st))
            _children[1].evaluateVoid(st);
        return nullptr;

    case IF_ELSE:
        assert(_children.size() == 3);
        if (_children[0].evaluateBoolean(st))
            _children[1].evaluateVoid(st);
        else
            _children[2].evaluateVoid(st);
        return nullptr;

    case CONDITIONAL:
        assert(_children.size() == 3);
        return _children[0].evaluateBoolean(st) ? _children[1].evaluate(st) : _children[2].evaluate(st);

    case WHILE:
        assert(_children.size() == 2);
        try
        {
            while (_children[0].evaluateBoolean(st))
                _children[1].evaluateVoid(st);
        }
        catch (const breakType &e)
        {
        }
        return nullptr;

    case WHILE_COMPOUND_STATEMENT:
    {
        assert(_children.size() == 2);
        stack localStack(&st);
        try
        {
            while (_children[0].evaluateBoolean(st))
                for (auto &child : _children[1]._children)
                    child.evaluateVoid(localStack);
        }
        catch (const breakType &e)
        {
        }
        return nullptr;
    }

    case WHILE_COMPOUND_STATEMENT_STACKLESS:
        assert(_children.size() == 2);
        try
        {
            while (_children[0].evaluateBoolean(st))
                for (auto &child : _children[1]._children)
                    child.evaluateVoid(st);
        }
        catch (const breakType &e)
        {
        }
        return nullptr;

    case FOR:
        assert(_children.size() == 4);
        _children[0].evaluateVoid(st);
        try
        {
            while (_children[1].evaluateBoolean(st))
            {
                _children[3].evaluateVoid(st);
                _children[2].evaluateVoid(st);
            }
        }
        catch (const breakType &e)
        {
        }
        return nullptr;

    case FOR_COMPOUND_STATEMENT:
    {
        assert(_children.size() == 4);
        stack localStack(&st);
        _children[0].evaluateVoid(st);
        try
        {
            while (_children[1].evaluateBoolean(st))
            {
                for (auto &child : _children[3]._children)
                    child.evaluateVoid(localStack);
                _children[2].evaluateVoid(st);
            }
        }
        catch (const breakType &e)
        {
        }
        return nullptr;
    }

    case FOR_COMPOUND_STATEMENT_STACKLESS:
        assert(_children.size() == 4);
        _children[0].evaluateVoid(st);
        try
        {
            while (_children[1].evaluateBoolean(st))
            {
                for (auto &child : _children[3]._children)
                    child.evaluateVoid(st);
                _children[2].evaluateVoid(st);
            }
        }
        catch (const breakType &e)
        {
        }
        return nullptr;

    case STATEMENT:
        assert(_children.size() == 1);
        return _children[0].evaluate(st);

    caseUnary(CALL_OPERATOR,
    {
        assert(_children.size() >= 1);
        object::type::Array args(_children.size() - 1);
        for (size_t i = 1, j = 0; i < _children.size(); i++)
            if (_children[i]._token != SPREAD_OPERATOR)
                args[j++] = _children[i].evaluate(st);
            else
            {
                args.erase(args.begin() + j);
                auto arr = _children[i].evaluate(st)->getConverted<object::type::Array>();
                args.insert(args.begin() + j, arr.begin(), arr.end());
                j += arr.size();
            }

        try
        {
            return (*a)(a, std::move(args), &st);
        }
        catch (objectException &e)
        {
            _children[0].exception(_children[0].toName());
            throw e;
        }
        catch (std::exception &e)
        {
            _children[0].exception(_children[0].toName());
            throw e;
        }
    })

    caseUnary(READ_OPERATOR,
    {
        assert(_children.size() >= 1);
        object::type::Array args(_children.size() - 1);
        for (size_t i = 1, j = 0; i < _children.size(); i++)
            if (_children[i]._token != SPREAD_OPERATOR)
                args[j++] = _children[i].evaluate(st);
            else
            {
                args.erase(args.begin() + j);
                auto arr = _children[i].evaluate(st)->getConverted<object::type::Array>();
                args.insert(args.begin() + j, arr.begin(), arr.end());
                j += arr.size();
            }
        try
        {
            return (*(*a)[n::readOperator])(a, std::move(args), &st);
        }
        catch (objectException &e)
        {
            _children[0].exception(_children[0].toName() + ".readOperator");
            throw e;
        }
        catch (std::exception &e)
        {
            _children[0].exception(_children[0].toName() + ".readOperator");
            throw e;
        }
    })

    caseUnary(METHOD_CALL_OPERATOR,
    {
        assert(_children.size() >= 2);
        assert(_children[1]._token == IDENTIFIER);
        auto b = (*a)[_children[1]._text];
        object::type::Array args(_children.size() - 2);
        for (size_t i = 2, j = 0; i < _children.size(); i++)
            if (_children[i]._token != SPREAD_OPERATOR)
                args[j++] = _children[i].evaluate(st);
            else
            {
                args.erase(args.begin() + j);
                auto arr = _children[i].evaluate(st)->getConverted<object::type::Array>();
                args.insert(args.begin() + j, arr.begin(), arr.end());
                j += arr.size();
            }
        try
        {
            return (*b)(a, std::move(args), &st);
        }
        catch (objectException &e)
        {
            _children[0].exception(_children[0].toName() + "." + _children[1].toName());
            throw e;
        }
        catch (std::exception &e)
        {
            _children[0].exception(_children[0].toName() + "." + _children[1].toName());
            throw e;
        }
    })

    caseUnary(AWAIT,
    {
        assert(_children.size() == 1);
        auto b = (*a)[n::await_];
        return (*b)(a, {}, &st);
    })

    case ARRAY_LITERAL:
    {
        object::type::Array args(_children.size());
        for (size_t i = 0, j = 0; i < _children.size(); i++)
            if (_children[i]._token != SPREAD_OPERATOR)
                args[j++] = _children[i].evaluate(st);
            else
            {
                args.erase(args.begin() + j);
                auto arr = _children[i].evaluate(st)->getConverted<object::type::Array>();
                args.insert(args.begin() + j, arr.begin(), arr.end());
                j += arr.size();
            }
        return object::makeObject(args);
    }

    caseUnary(DOT,
    {
        assert(_children.size() == 2);
        assert(_children[1]._token == IDENTIFIER);
        return (*a)[_children[1]._text];
    })

    case LET:
        assert(_children.size() == 0);
        return st.insert(_text, object::makeEmptyObject());

    caseUnary(ASSIGNMENT,
    {
        assert(_children.size() == 2);
        if (a->isConst())
            throw std::runtime_error("tried to modify constant value");

        object::objectPtr b;
        if (!numberAssignmentOptimization(a, b, st))
        {
            *a = b;
            a->setConst(false);
        }
        return a;
    })

    case INIT_ASSIGNMENT_LET:
        assert(_children.size() == 2);
        return st.insert(_children[0]._text, _children[1].evaluate(st));

    case INIT_ASSIGNMENT_IDENTIFIER:
    {
        assert(_children.size() == 2);
        assert(!_children[0]._text.isEmpty());
        auto &a = st[_children[0]._text];
        if (a->isConst())
            throw std::runtime_error("tried to modify constant value");
        object::objectPtr b;
        if (!numberAssignmentOptimization(a, b, st))
            a = b;
        return a;
    }

    case INIT_ASSIGNMENT_DOT:
    {
        assert(_children[0]._children.size() == 2);
        auto &a = (*(_children[0]._children[0].evaluate(st)))[_children[0]._children[1]._text];
        if (a->isConst())
            throw std::runtime_error("tried to modify constant value");
        object::objectPtr b;
        if (!numberAssignmentOptimization(a, b, st))
            a = b;
        return a;
    }

    case JSON_ASSIGNMENT:
    {
        assert(_children.size() == 2);
        auto a = _children[1].evaluate(st);
        if (a->isConst())
        {
            auto b = object::makeEmptyObject();
            *b = *a;
            b->setConst(false);
            a = b;
        }
        if (_children[0]._token == IDENTIFIER)
            return st.insert(_children[0]._text, a);
        else
            return st.insert(static_cast<name>(_children[0].evaluate(st)->getConverted<object::type::String>()), a);
    }

    caseUnary(SPREAD_OPERATOR,
    {
        auto properties = a->getOwnPropertiesWithoutPrototype();
        for (const auto &property : properties)
            st.insert(property.first, property.second);
        return a;
    })

    case DELETE_:
        assert(_children.size() == 1);
        if (_children[0]._token == IDENTIFIER)
        {
            assert(!_children[0]._text.isEmpty());
            auto a = st[_children[0]._text];
            st.erase(_children[0]._text);
            return a;
        }
        else if (_children[0]._token == DOT && _children[0]._children[1]._token == IDENTIFIER)
        {
            assert(_children[0]._children.size() == 2);
            auto expr = _children[0]._children[0].evaluate(st);
            auto a = (*expr)[_children[0]._children[1]._text];
            expr->removeProperty(_children[0]._children[1]._text);
            return a;
        }
        throw std::runtime_error("left side of delete is not identifier or dot operator");
        break;

    case JSON:
    {
        stack localJsonStack(&st);
        for (auto &child : _children)
            child.evaluate(localJsonStack);

        auto obj = object::makeEmptyObject();
        localJsonStack.copyToObject(obj);
        return obj;
    }

    case CLASS:
    {
        stack localJsonStack(&st);
        for (auto &child : _children)
            child.evaluate(localJsonStack);

        auto obj = object::makeEmptyObject();
        localJsonStack.copyToObject(obj);

        auto Class = st[n::Class];
        if (!_children.empty() && _children.back()._token == IDENTIFIER)
        {
            auto &prototype = (*obj)[n::prototype];
            prototype = (*_children.back().evaluate(st))[n::classPrototype];
            (*prototype)[n::super] = (*prototype)[n::constructor];
        }
        obj = (*Class)(Class, {obj}, &st);
        st.insert(_text, obj);
        return obj;
    }

    caseUnary(RETURN,
    {
        assert(_children.size() == 1);
        throw a;
    })

    caseUnary(THROW,
    {
        assert(_children.size() == 1);
        throw objectException(a);
    })

    case BREAK:
        throw breakType();

    case TRY:
    {
        assert(_children.size() == 2);
        try
        {
            _children[0].evaluateVoid(st);
        }
        catch (objectException &e)
        {
            stackTrace.clear();
            stack localStack(&st);
            localStack.insert(n::exception, e.getPtr());
            _children[1].evaluateVoid(localStack);
        }
        catch (std::exception &e)
        {
            stackTrace.clear();
            stack localStack(&st);
            localStack.insert(n::exception, object::makeObject((std::string)e.what()));
            _children[1].evaluateVoid(localStack);
        }
        return nullptr;
    }

    caseBinary(USER_OPERATOR,
    {
        assert(_children.size() == 2);
        auto op = st[_text];
        return (*op)(op, {a, b}, &st);
    })

    caseBinary(INSTANCEOF,
    {
        assert(_children.size() == 2);
        return object::makeObject(a->hasOwnProperty(n::prototype) && b->hasOwnProperty(n::classPrototype) && (*a)[n::prototype].get() == (*b)[n::classPrototype].get());
    })

    caseBinary(ADDITION,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(a->uncheckedGet<const number>() + b->getConverted<object::type::Number>()));
        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<std::string>(a->uncheckedGet<const object::type::String>() + b->getConverted<object::type::String>()));
        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
        {
            object::type::Array arr = a->uncheckedGet<const object::type::Array>();
            object::type::Array toAdd = b->getConverted<object::type::Array>();
            arr.insert(arr.end(), toAdd.begin(), toAdd.end());
            return object::makeObject(arr);
        }
        return (*(*a)[n::addition])(a, {b}, &st);
    })

    caseBinary(MULTIPLICATION,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(a->uncheckedGet<const number>() * b->getConverted<object::type::Number>()));
        else if (a->isOfType<std::string>() && b->isConvertible<number>())
        {
            const std::string &str = a->uncheckedGet<const std::string>();
            size_t i = static_cast<size_t>(b->getConverted<object::type::Number>());
            std::string res;
            res.reserve(str.size() * i);
            while (i--)
                res.insert(res.begin(), str.begin(), str.end());
            return object::makeObject(res);
        }
        else if (a->isOfType<object::type::Array>() && b->isConvertible<number>())
        {
            const object::type::Array &arr = a->uncheckedGet<const object::type::Array>();
            size_t i = static_cast<size_t>(b->getConverted<object::type::Number>());
            object::type::Array res;
            res.reserve(arr.size() * i);
            while (i--)
                res.insert(res.begin(), arr.begin(), arr.end());
            for (auto &el : res)
            {
                *el = el;
                el->setConst(false);
            }
            return object::makeObject(res);
        }
        return (*(*a)[n::multiplication])(a, {b}, &st);
    })

    caseBinary(SUBTRACTION,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(a->uncheckedGet<const number>() - b->getConverted<object::type::Number>()));
        return (*(*a)[n::subtraction])(a, {b}, &st);
    })

    caseBinary(DIVISION,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(a->uncheckedGet<const number>() / b->getConverted<object::type::Number>()));
        return (*(*a)[n::division])(a, {b}, &st);
    })

    caseBinary(MODULUS,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(a->uncheckedGet<const number>() % b->getConverted<object::type::Number>()));
        return (*(*a)[n::modulus])(a, {b}, &st);
    })

    caseUnary(AND,
    {
        assert(_children.size() == 2);
        object::objectPtr b;
        if (a->isConvertible<bool>())
        {
            if (!a->getConverted<object::type::Boolean>())
                return object::makeObject(false);
            b = _children[1].evaluate(st);
            if (b->isConvertible<bool>())
                return object::makeObject(b->getConverted<object::type::Boolean>());
        }
        else
            b = _children[1].evaluate(st);
        return (*(*a)[n::andOp])(a, {b}, &st);
    })

    caseUnary(OR,
    {
        assert(_children.size() == 2);
        object::objectPtr b;
        if (a->isConvertible<bool>())
        {
            if (a->getConverted<object::type::Boolean>())
                return a;
            b = _children[1].evaluate(st);
            if (b->isConvertible<bool>())
                return b;
        }
        else
            b = _children[1].evaluate(st);
        return (*(*a)[n::orOp])(a, {b}, &st);
    })

    caseBinary(BITWISE_AND,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(static_cast<number::intType>(a->uncheckedGet<const number>()) & static_cast<number::intType>(b->getConverted<object::type::Number>())));
        return (*(*a)[n::bitwiseAnd])(a, {b}, &st);
    })

    caseBinary(BITWISE_OR,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(static_cast<number::intType>(a->uncheckedGet<const number>()) | static_cast<number::intType>(b->getConverted<object::type::Number>())));
        return (*(*a)[n::bitwiseOr])(a, {b}, &st);
    })

    caseBinary(SHIFT_LEFT,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(static_cast<number::intType>(a->uncheckedGet<const number>()) << static_cast<number::intType>(b->getConverted<object::type::Number>())));
        return (*(*a)[n::shiftLeft])(a, {b}, &st);
    })

    caseBinary(SHIFT_RIGHT,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(static_cast<number::intType>(a->uncheckedGet<const number>()) >> static_cast<number::intType>(b->getConverted<object::type::Number>())));
        return (*(*a)[n::shiftRight])(a, {b}, &st);
    })

    caseBinary(EQUAL,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const number>() == b->getConverted<object::type::Number>()));

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const std::string>() == b->getConverted<object::type::String>()));

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const object::type::Array>() == b->getConverted<object::type::Array>()));

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const bool>() == b->getConverted<object::type::Boolean>()));

        else if (a->isOfType<nullptr_t>() && b->isOfType<nullptr_t>())
            return object::makeObject(*a == *b);

        return (*(*a)[n::equal])(a, {b}, &st);
    })

    caseBinary(NOT_EQUAL,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const number>() != b->getConverted<object::type::Number>()));

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const std::string>() != b->getConverted<object::type::String>()));

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const object::type::Array>() != b->getConverted<object::type::Array>()));

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const bool>() != b->getConverted<object::type::Boolean>()));

        return (*(*a)[n::lessEqual])(a, {b}, &st);
    })

    caseBinary(LESS,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const number>() < b->getConverted<object::type::Number>()));

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const std::string>() < b->getConverted<object::type::String>()));

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const object::type::Array>() < b->getConverted<object::type::Array>()));

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const bool>() < b->getConverted<object::type::Boolean>()));

        return (*(*a)[n::less])(a, {b}, &st);
    })

    caseBinary(GREATER,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const number>() > b->getConverted<object::type::Number>()));

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const std::string>() > b->getConverted<object::type::String>()));

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const object::type::Array>() > b->getConverted<object::type::Array>()));

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const bool>() > b->getConverted<object::type::Boolean>()));

        return (*(*a)[n::greater])(a, {b}, &st);
    })

    caseBinary(LESS_EQUAL,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const number>() <= b->getConverted<object::type::Number>()));

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const std::string>() <= b->getConverted<object::type::String>()));

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const object::type::Array>() <= b->getConverted<object::type::Array>()));

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const bool>() <= b->getConverted<object::type::Boolean>()));

        return (*(*a)[n::lessEqual])(a, {b}, &st);
    })

    caseBinary(GREATER_EQUAL,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const number>() >= b->getConverted<object::type::Number>()));

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const std::string>() >= b->getConverted<object::type::String>()));

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const object::type::Array>() >= b->getConverted<object::type::Array>()));

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->uncheckedGet<const bool>() >= b->getConverted<object::type::Boolean>()));

        return (*(*a)[n::greaterEqual])(a, {b}, &st);
    })

    caseUnary(INCREMENT,
    {
        assert(_children.size() == 1);
        if (a->isOfType<number>())
        {
            a->uncheckedGet<number>()++;
            return a;
        }
        return (*(*a)[n::increment])(a, {}, &st);
    })
        

    caseUnary(DECREMENT,
    {
        assert(_children.size() == 1);
        if (a->isOfType<number>())
        {
            a->uncheckedGet<number>()--;
            return a;
        }
        return (*(*a)[n::decrement])(a, {}, &st);
    })

    caseUnary(NOT,
    {
        assert(_children.size() == 1);
        if (a->isConvertible<bool>())
            return object::makeObject(!a->getConverted<object::type::Boolean>());
        return (*(*a)[n::notOp])(a, {}, &st);
    })

    caseUnary(COMPLEMENT,
    {
        assert(_children.size() == 1);
        if (a->isOfType<number>())
            return object::makeObject(static_cast<number>(~static_cast<number::intType>(a->uncheckedGet<const number>())));
        return (*(*a)[n::complement])(a, {}, &st);
    })

    case IDENTIFIER:
        assert(!_text.isEmpty());
        return st[_text];

    case FUNCTION:
        if (!_text.isEmpty())
            st.insert(_text, _value);
        const_cast<object &>(*_value).captureStack(st.copyToFlatStack());
    case NUMBER_LITERAL:
    case STRING_LITERAL:
        assert(_value != nullptr);
        return _value;

    default:
        for (auto &child : _children)
            console::warn(LLgetSymbol(child._token));
        break;
    }
    return object::makeEmptyObject();
}

void Node::evaluateVoid(stack &st) const
{

    switch (_token)
    {
    case COMPOUND_STATEMENT:
    {
        size_t i = 0;
        stack localStack(&st);
        try
        {
            for (; i < _children.size(); i++)
                _children[i].evaluateVoid(localStack);
        }
        catch (objectException &e)
        {
            _children[i].exception();
            throw e;
        }
        catch (std::exception &e)
        {
            _children[i].exception();
            throw e;
        }
        return;
    }

    case COMPOUND_STATEMENT_STACKLESS:
    {
        size_t i = 0;
        try
        {
            for (; i < _children.size(); i++)
                _children[i].evaluateVoid(st);
        }
        catch (objectException &e)
        {
            _children[i].exception();
            throw e;
        }
        catch (std::exception &e)
        {
            _children[i].exception();
            throw e;
        }
        return;
    }
    case IF:
        assert(_children.size() == 2);
        if (_children[0].evaluateBoolean(st))
            _children[1].evaluateVoid(st);
        return;

    case IF_ELSE:
        assert(_children.size() == 3);
        if (_children[0].evaluateBoolean(st))
            _children[1].evaluateVoid(st);
        else
            _children[2].evaluateVoid(st);
        return;

    case CONDITIONAL:
        assert(_children.size() == 3);
        _children[0].evaluateBoolean(st) ? _children[1].evaluateVoid(st) : _children[2].evaluateVoid(st);
        return;

    case WHILE:
        assert(_children.size() == 2);
        try
        {
            while (_children[0].evaluateBoolean(st))
                _children[1].evaluateVoid(st);
        }
        catch (const breakType &e)
        {
        }
        return;

    case WHILE_COMPOUND_STATEMENT:
    {
        assert(_children.size() == 2);
        stack localStack(&st);
        try
        {
            while (_children[0].evaluateBoolean(st))
                for (auto &child : _children[1]._children)
                    child.evaluateVoid(localStack);
        }
        catch (const breakType &e)
        {
        }
        return;
    }

    case WHILE_COMPOUND_STATEMENT_STACKLESS:
        assert(_children.size() == 2);
        try
        {
            while (_children[0].evaluateBoolean(st))
                for (auto &child : _children[1]._children)
                    child.evaluateVoid(st);
        }
        catch (const breakType &e)
        {
        }
        return;

    case FOR:
        assert(_children.size() == 4);
        _children[0].evaluateVoid(st);
        try
        {
            while (_children[1].evaluateBoolean(st))
            {
                _children[3].evaluateVoid(st);
                _children[2].evaluateVoid(st);
            }
        }
        catch (const breakType &e)
        {
        }
        return;

    case FOR_COMPOUND_STATEMENT:
    {
        assert(_children.size() == 4);
        stack localStack(&st);
        _children[0].evaluateVoid(st);
        try
        {
            while (_children[1].evaluateBoolean(st))
            {
                for (auto &child : _children[3]._children)
                    child.evaluateVoid(localStack);
                _children[2].evaluateVoid(st);
            }
        }
        catch (const breakType &e)
        {
        }
        return;
    }

    case FOR_COMPOUND_STATEMENT_STACKLESS:
        assert(_children.size() == 4);
        _children[0].evaluateVoid(st);
        try
        {
            while (_children[1].evaluateBoolean(st))
            {
                for (auto &child : _children[3]._children)
                    child.evaluateVoid(st);
                _children[2].evaluateVoid(st);
            }
        }
        catch (const breakType &e)
        {
        }
        return;

    case STATEMENT:
        assert(_children.size() == 1);
        _children[0].evaluateVoid(st);
        return;

    caseUnary(CALL_OPERATOR,
    {
        assert(_children.size() >= 1);
        object::type::Array args(_children.size() - 1);
        for (size_t i = 1, j = 0; i < _children.size(); i++)
            if (_children[i]._token != SPREAD_OPERATOR)
                args[j++] = _children[i].evaluate(st);
            else
            {
                args.erase(args.begin() + j);
                auto arr = _children[i].evaluate(st)->getConverted<object::type::Array>();
                args.insert(args.begin() + j, arr.begin(), arr.end());
                j += arr.size();
            }
        try
        {
            (*a)(a, std::move(args), &st);
            return;
        }
        catch (objectException &e)
        {
            _children[0].exception(_children[0].toName());
            throw e;
        }
        catch (std::exception &e)
        {
            _children[0].exception(_children[0].toName());
            throw e;
        }
    })

    caseUnary(READ_OPERATOR,
    {
        assert(_children.size() >= 1);
        object::type::Array args(_children.size() - 1);
        for (size_t i = 1, j = 0; i < _children.size(); i++)
            if (_children[i]._token != SPREAD_OPERATOR)
                args[j++] = _children[i].evaluate(st);
            else
            {
                args.erase(args.begin() + j);
                auto arr = _children[i].evaluate(st)->getConverted<object::type::Array>();
                args.insert(args.begin() + j, arr.begin(), arr.end());
                j += arr.size();
            }
        try
        {
            (*(*a)[n::readOperator])(a, std::move(args), &st);
            return;
        }
        catch (objectException &e)
        {
            _children[0].exception(_children[0].toName() + ".readOperator");
            throw e;
        }
        catch (std::exception &e)
        {
            _children[0].exception(_children[0].toName() + ".readOperator");
            throw e;
        }
    })

    caseUnary(METHOD_CALL_OPERATOR,
    {
        assert(_children.size() >= 2);
        assert(_children[1]._token == IDENTIFIER);
        auto b = (*a)[_children[1]._text];
        object::type::Array args(_children.size() - 2);
        for (size_t i = 2, j = 0; i < _children.size(); i++)
            if (_children[i]._token != SPREAD_OPERATOR)
                args[j++] = _children[i].evaluate(st);
            else
            {
                args.erase(args.begin() + j);
                auto arr = _children[i].evaluate(st)->getConverted<object::type::Array>();
                args.insert(args.begin() + j, arr.begin(), arr.end());
                j += arr.size();
            }
        try
        {
            (*b)(a, std::move(args), &st);
            return;
        }
        catch (objectException &e)
        {
            _children[0].exception(_children[0].toName() + "." + _children[1].toName());
            throw e;
        }
        catch (std::exception &e)
        {
            _children[0].exception(_children[0].toName() + "." + _children[1].toName());
            throw e;
        }
    })

    caseUnary(AWAIT,
    {
        assert(_children.size() == 1);
        auto b = (*a)[n::await_];
        (*b)(a, {}, &st);
        return;
    })

    case ARRAY_LITERAL:
    {
        for (size_t i = 0; i < _children.size(); i++)
            _children[i].evaluateVoid(st);
        return;
    }

    caseUnary(DOT,
    {
        assert(_children.size() == 2);
        assert(_children[1]._token == IDENTIFIER);
        (*a)[_children[1]._text];
        return;
    })

    case LET:
        assert(_children.size() == 0);
        st.insert(_text, object::makeEmptyObject());
        return;

    caseUnary(ASSIGNMENT,
    {
        assert(_children.size() == 2);
        if (a->isConst())
            throw std::runtime_error("tried to modify constant value");
        object::objectPtr b;
        if (!numberAssignmentOptimization(a, b, st))
        {
            *a = b;
            a->setConst(false);
        }
        return;
    })

    case INIT_ASSIGNMENT_LET:
        assert(_children.size() == 2);
        st.insert(_children[0]._text, _children[1].evaluate(st));
        return;

    case INIT_ASSIGNMENT_IDENTIFIER:
    {
        assert(_children.size() == 2);
        assert(!_children[0]._text.isEmpty());
        auto &a = st[_children[0]._text];
        if (a->isConst())
            throw std::runtime_error("tried to modify constant value");
        object::objectPtr b;
        if (!numberAssignmentOptimization(a, b, st))
            a = b;
        return;
    }

    case INIT_ASSIGNMENT_DOT:
    {
        assert(_children[0]._children.size() == 2);
        auto &a = (*(_children[0]._children[0].evaluate(st)))[_children[0]._children[1]._text];
        if (a->isConst())
            throw std::runtime_error("tried to modify constant value");
        object::objectPtr b;
        if (!numberAssignmentOptimization(a, b, st))
            a = b;
        return;
    }

    case JSON_ASSIGNMENT:
    {
        assert(_children.size() == 2);
        auto a = _children[1].evaluate(st);
        if (a->isConst())
        {
            auto b = object::makeEmptyObject();
            *b = *a;
            b->setConst(false);
            a = b;
        }
        if (_children[0]._token == IDENTIFIER)
            st.insert(_children[0]._text, a);
        else
            st.insert(static_cast<name>(_children[0].evaluate(st)->getConverted<object::type::String>()), a);
        return;
    }

    caseUnary(SPREAD_OPERATOR,
    {
        auto properties = a->getOwnPropertiesWithoutPrototype();
        for (const auto &property : properties)
            st.insert(property.first, property.second);
        return;
    })

    case DELETE_:
        assert(_children.size() == 1);
        if (_children[0]._token == IDENTIFIER)
        {
            assert(!_children[0]._text.isEmpty());
            auto a = st[_children[0]._text];
            st.erase(_children[0]._text);
            return;
        }
        else if (_children[0]._token == DOT && _children[0]._children[1]._token == IDENTIFIER)
        {
            assert(_children[0]._children.size() == 2);
            auto expr = _children[0]._children[0].evaluate(st);
            auto a = (*expr)[_children[0]._children[1]._text];
            expr->removeProperty(_children[0]._children[1]._text);
            return;
        }
        throw std::runtime_error("left side of delete is not identifier or dot operator");
        break;

    case JSON:
    {
        stack localJsonStack(&st);
        for (auto &child : _children)
            child.evaluate(localJsonStack);

        auto obj = object::makeEmptyObject();
        localJsonStack.copyToObject(obj);
        return;
    }

    case CLASS:
    {
        stack localJsonStack(&st);
        for (auto &child : _children)
            child.evaluateVoid(localJsonStack);

        auto obj = object::makeEmptyObject();
        localJsonStack.copyToObject(obj);

        auto Class = st[n::Class];
        if (!_children.empty() && _children.back()._token == IDENTIFIER)
        {
            auto &prototype = (*obj)[n::prototype];
            prototype = (*_children.back().evaluate(st))[n::classPrototype];
            (*prototype)[n::super] = (*prototype)[n::constructor];
        }
        obj = (*Class)(Class, {obj}, &st);
        st.insert(_text, obj);
        return;
    }

    caseUnary(RETURN,
    {
        assert(_children.size() == 1);
        throw a;
    })

    caseUnary(THROW,
    {
        assert(_children.size() == 1);
        throw objectException(a);
    })

    case BREAK:
        throw breakType();

    case TRY:
    {
        assert(_children.size() == 2);
        try
        {
            _children[0].evaluateVoid(st);
        }
        catch (objectException &e)
        {
            stackTrace.clear();
            stack localStack(&st);
            localStack.insert(n::exception, e.getPtr());
            _children[1].evaluateVoid(localStack);
        }
        catch (std::exception &e)
        {
            stackTrace.clear();
            stack localStack(&st);
            localStack.insert(n::exception, object::makeObject((std::string)e.what()));
            _children[1].evaluateVoid(localStack);
        }
        return;
    }

    caseBinary(USER_OPERATOR,
    {
        assert(_children.size() == 2);
        auto op = st[_text];
        (*op)(op, {a, b}, &st);
        return;
    })

    allCases(INSTANCEOF)
        return;

    caseBinary(ADDITION,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return;
        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return;
        (*(*a)[n::addition])(a, {b}, &st);
        return;
    })

    caseBinary(MULTIPLICATION,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        else if (a->isOfType<std::string>() && b->isConvertible<number>())
            return;
        else if (a->isOfType<object::type::Array>() && b->isConvertible<number>())
            return;
        (*(*a)[n::multiplication])(a, {b}, &st);
        return;
    })

    caseBinary(SUBTRACTION,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        (*(*a)[n::subtraction])(a, {b}, &st);
        return;
    })

    caseBinary(DIVISION,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        (*(*a)[n::division])(a, {b}, &st);
        return;
    })

    caseBinary(MODULUS,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        (*(*a)[n::modulus])(a, {b}, &st);
        return;
    })

    caseUnary(AND,
    {
        assert(_children.size() == 2);
        object::objectPtr b;
        if (a->isConvertible<bool>())
        {
            if (!a->getConverted<object::type::Boolean>())
                return;
            b = _children[1].evaluate(st);
            if (b->isConvertible<bool>())
                return;
        }
        else
            b = _children[1].evaluate(st);
        (*(*a)[n::andOp])(a, {b}, &st);
        return;
    })

    caseUnary(OR,
    {
        assert(_children.size() == 2);
        object::objectPtr b;
        if (a->isConvertible<bool>())
        {
            if (a->getConverted<object::type::Boolean>())
                return;
            b = _children[1].evaluate(st);
            if (b->isConvertible<bool>())
                return;
        }
        else
            b = _children[1].evaluate(st);
        (*(*a)[n::orOp])(a, {b}, &st);
        return;
    })

    caseBinary(BITWISE_AND,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        (*(*a)[n::bitwiseAnd])(a, {b}, &st);
        return;
    })

    caseBinary(BITWISE_OR,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        (*(*a)[n::bitwiseOr])(a, {b}, &st);
        return;
    })

    caseBinary(SHIFT_LEFT,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        (*(*a)[n::shiftLeft])(a, {b}, &st);
        return;
    })

    caseBinary(SHIFT_RIGHT,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        (*(*a)[n::shiftRight])(a, {b}, &st);
        return;
    })

    caseBinary(EQUAL,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return;
        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return;
        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return;
        (*(*a)[n::equal])(a, {b}, &st);
        return;
    })

    caseBinary(NOT_EQUAL,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return;
        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return;
        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return;
        (*(*a)[n::lessEqual])(a, {b}, &st);
        return;
    })

    caseBinary(LESS,
    {
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return;
        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return;
        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return;
        (*(*a)[n::less])(a, {b}, &st);
        return;
    })

    caseBinary(GREATER,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return;
        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return;
        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return;
        (*(*a)[n::greater])(a, {b}, &st);
        return;
    })

    caseBinary(LESS_EQUAL,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return;
        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return;
        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return;
        (*(*a)[n::lessEqual])(a, {b}, &st);
        return;
    })

    caseBinary(GREATER_EQUAL,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return;
        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return;
        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return;
        (*(*a)[n::greaterEqual])(a, {b}, &st);
        return;
    })

    caseUnary(INCREMENT,
    {
        assert(_children.size() == 1);
        if (a->isOfType<number>())
        {
            a->uncheckedGet<number>()++;
            return;
        }
        (*(*a)[n::increment])(a, {}, &st);
        return;
    })

    caseUnary(DECREMENT,
    {
        assert(_children.size() == 1);
        if (a->isOfType<number>())
        {
            a->uncheckedGet<number>()--;
            return;
        }
        (*(*a)[n::decrement])(a, {}, &st);
        return;
    })

    caseUnary(NOT,
    {
        assert(_children.size() == 1);
        if (a->isConvertible<bool>())
            return;
        (*(*a)[n::notOp])(a, {}, &st);
        return;
    })

    caseUnary(COMPLEMENT,
    {
        assert(_children.size() == 1);
        if (a->isOfType<number>())
            return;
        (*(*a)[n::complement])(a, {}, &st);
        return;
    })

    case IDENTIFIER:
        return;

    case FUNCTION:
        if (!_text.isEmpty())
            st.insert(_text, _value);
        const_cast<object &>(*_value).captureStack(st.copyToFlatStack());
    case NUMBER_LITERAL:
    case STRING_LITERAL:
        return;

    default:
        for (auto &child : _children)
            console::warn(LLgetSymbol(child._token));
        break;
    }
}

number Node::evaluateNumber(stack &st) const
{

    switch (_token)
    {

    //case CALL_OPERATOR:
    //case METHOD_CALL_OPERATOR:
    // TODO
    // sin, cos, sqrt etc.
    case IDENTIFIER:
        return st[_text]->getConverted<object::type::Number>();
    /*case INCREMENT:
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluate(st);
        if (a->isOfType<number>())
            return ++(a->get<number>());
    }

    case DECREMENT:
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluate(st);
        if (a->isOfType<number>())
            return --(a->get<number>());
    }*/

    allCases(COMPLEMENT)
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluateNumber(st);
        return static_cast<number>(~static_cast<number::intType>(a));
    }

    case NUMBER_LITERAL:
        assert(_value != nullptr);
        return _value->uncheckedGet<const number>();

    case CONDITIONAL:
        assert(_children.size() == 3);
        return _children[0].evaluateBoolean(st) ? _children[1].evaluateNumber(st) : _children[2].evaluateNumber(st);
    }
    return evaluate(st)->getConverted<object::type::Number>();
}

bool Node::evaluateBoolean(stack &st) const
{
    switch (_token)
    {
    case IDENTIFIER:
        return st[_text]->getConverted<object::type::Boolean>();
    caseBinary(INSTANCEOF,
    {
        assert(_children.size() == 2);
        return a->hasOwnProperty(n::prototype) && b->hasOwnProperty(n::classPrototype) && (*a)[n::prototype].get() == (*b)[n::classPrototype].get();
    })

    caseUnary(AND,
    {
        assert(_children.size() == 2);
        object::objectPtr b;
        if (a->isConvertible<bool>())
        {
            if (!a->getConverted<object::type::Boolean>())
                return false;
            b = _children[1].evaluate(st);
            if (b->isConvertible<bool>())
                return b->getConverted<object::type::Boolean>();
        }
        else
            b = _children[1].evaluate(st);
        return (*(*a)[n::andOp])(a, {b}, &st)->getConverted<object::type::Boolean>();
    })

    caseUnary(OR,
    {
        assert(_children.size() == 2);
        object::objectPtr b;
        if (a->isConvertible<bool>())
        {
            if (a->getConverted<object::type::Boolean>())
                return true;
            b = _children[1].evaluate(st);
            if (b->isConvertible<bool>())
                return b->getConverted<object::type::Boolean>();
        }
        else
            b = _children[1].evaluate(st);
        return (*(*a)[n::orOp])(a, {b}, &st)->getConverted<object::type::Boolean>();
    })

    caseBinary(EQUAL,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return a->uncheckedGet<const number>() == b->getConverted<object::type::Number>();

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return a->uncheckedGet<const std::string>() == b->getConverted<object::type::String>();

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return a->uncheckedGet<const object::type::Array>() == b->getConverted<object::type::Array>();

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return a->uncheckedGet<const bool>() == b->getConverted<object::type::Boolean>();

        else if (a->isOfType<nullptr_t>() && b->isOfType<nullptr_t>())
            return object::makeObject(*a == *b);

        return (*(*a)[n::equal])(a, {b}, &st)->getConverted<object::type::Boolean>();
    })

    caseBinary(NOT_EQUAL,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return a->uncheckedGet<const number>() != b->getConverted<object::type::Number>();

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return a->uncheckedGet<const std::string>() != b->getConverted<object::type::String>();

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return a->uncheckedGet<const object::type::Array>() != b->getConverted<object::type::Array>();

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return a->uncheckedGet<const bool>() != b->getConverted<object::type::Boolean>();

        return (*(*a)[n::lessEqual])(a, {b}, &st)->getConverted<object::type::Boolean>();
    })

    caseBinary(LESS,
    {
        if (a->isOfType<number>() && b->isConvertible<number>())
            return a->uncheckedGet<const number>() < b->getConverted<object::type::Number>();

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return a->uncheckedGet<const std::string>() < b->getConverted<object::type::String>();

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return a->uncheckedGet<const object::type::Array>() < b->getConverted<object::type::Array>();

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return a->uncheckedGet<const bool>() < b->getConverted<object::type::Boolean>();

        return (*(*a)[n::less])(a, {b}, &st)->getConverted<object::type::Boolean>();
    })

    caseBinary(GREATER,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return a->uncheckedGet<const number>() > b->getConverted<object::type::Number>();

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return a->uncheckedGet<const std::string>() > b->getConverted<object::type::String>();

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return a->uncheckedGet<const object::type::Array>() > b->getConverted<object::type::Array>();

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return a->uncheckedGet<const bool>() > b->getConverted<object::type::Boolean>();

        return (*(*a)[n::greater])(a, {b}, &st)->getConverted<object::type::Boolean>();
    })

    caseBinary(LESS_EQUAL,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return a->uncheckedGet<const number>() <= b->getConverted<object::type::Number>();

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return a->uncheckedGet<const std::string>() <= b->getConverted<object::type::String>();

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return a->uncheckedGet<const object::type::Array>() <= b->getConverted<object::type::Array>();

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return a->uncheckedGet<const bool>() <= b->getConverted<object::type::Boolean>();

        return (*(*a)[n::lessEqual])(a, {b}, &st)->getConverted<object::type::Boolean>();
    })

    caseBinary(GREATER_EQUAL,
    {
        assert(_children.size() == 2);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return a->uncheckedGet<const number>() >= b->getConverted<object::type::Number>();

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return a->uncheckedGet<const std::string>() >= b->getConverted<object::type::String>();

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return a->uncheckedGet<const object::type::Array>() >= b->getConverted<object::type::Array>();

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return a->uncheckedGet<const bool>() >= b->getConverted<object::type::Boolean>();

        return (*(*a)[n::greaterEqual])(a, {b}, &st)->getConverted<object::type::Boolean>();
    })

    caseUnary(NOT,
    {
        assert(_children.size() == 1);
        if (a->isConvertible<bool>())
            return !a->getConverted<object::type::Boolean>();
        return (*(*a)[n::notOp])(a, {}, &st)->getConverted<object::type::Boolean>();
    })

    case CONDITIONAL:
        assert(_children.size() == 3);
        return _children[0].evaluateBoolean(st) ? _children[1].evaluateBoolean(st) : _children[2].evaluateBoolean(st);
    }
    return evaluate(st)->getConverted<object::type::Boolean>();
}