#include "Node.h"
#include "nobject.h"
#include "console.h"
#include <cassert>
#include <exception>

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
        if (t.find('.') == std::string::npos)
            _value = object::makeObject(static_cast<number>(std::stoi(t, 0, 0)));
        else
            _value = object::makeObject(static_cast<number>(std::stod(t)));
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
        _value = object::makeObject(object::type::Function(node));
        _value->setConst();
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
                    a->get<number>() += _children[1]._children[1].evaluateNumber(st);
                    break;
                case SUBTRACTION:
                    a->get<number>() -= _children[1]._children[1].evaluateNumber(st);
                    break;
                case MULTIPLICATION:
                    a->get<number>() *= _children[1]._children[1].evaluateNumber(st);
                    break;
                case DIVISION:
                    a->get<number>() /= _children[1]._children[1].evaluateNumber(st);
                    break;
                case MODULUS:
                    a->get<number>() %= _children[1]._children[1].evaluateNumber(st);
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

    case CALL_OPERATOR:
    {
        assert(_children.size() >= 1);
        auto a = _children[0].evaluate(st);
        object::type::Array args(_children.size() - 1);
        for (size_t i = 1; i < _children.size(); i++)
            args[i - 1] = _children[i].evaluate(st);

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
    }

    case READ_OPERATOR:
    {
        assert(_children.size() >= 1);
        auto a = _children[0].evaluate(st);
        object::type::Array args(_children.size() - 1);
        for (size_t i = 1; i < _children.size(); i++)
            args[i - 1] = _children[i].evaluate(st);

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
    }

    case METHOD_CALL_OPERATOR:
    {
        assert(_children.size() >= 2);
        assert(_children[1]._token == IDENTIFIER);
        auto a = _children[0].evaluate(st);
        auto b = (*a)[_children[1]._text];
        object::type::Array args(_children.size() - 2);
        for (size_t i = 2; i < _children.size(); i++)
            args[i - 2] = _children[i].evaluate(st);

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
    }

    case AWAIT:
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluate(st);
        auto b = (*a)[n::await_];
        return (*b)(a, {}, &st);
    }

    case ARRAY_LITERAL:
    {
        object::type::Array arr(_children.size());
        for (size_t i = 0; i < _children.size(); i++)
            arr[i] = _children[i].evaluate(st);
        return object::makeObject(arr);
    }

    case DOT:
    {
        assert(_children.size() == 2);
        assert(_children[1]._token == IDENTIFIER);
        return (*(_children[0].evaluate(st)))[_children[1]._text];
    }

    case LET:
        assert(_children.size() == 0);
        return st.insert(_text, object::makeEmptyObject());

    case ASSIGNMENT:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        if (a->isConst())
            throw std::runtime_error("tried to modify constant value");

        object::objectPtr b;
        if (!numberAssignmentOptimization(a, b, st))
        {
            *a = b;
            a->setConst(false);
        }
        return a;
    }

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
            return st.insert(static_cast<name>(_children[0].evaluate(st)->getConverted<std::string>()), a);
    }

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

    case RETURN:
        assert(_children.size() == 1);
        throw _children[0].evaluate(st);

    case THROW:
        assert(_children.size() == 1);
        throw objectException(_children[0].evaluate(st));

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

    case USER_OPERATOR:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        auto op = st[_text];
        return (*op)(op, {a, b}, &st);
    }

    case INSTANCEOF:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        return object::makeObject(a->hasOwnProperty(n::prototype) && b->hasOwnProperty(n::classPrototype) && (*a)[n::prototype].get() == (*b)[n::classPrototype].get());
    }

    case ADDITION:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(a->get<const number>() + b->getConverted<number>()));
        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<std::string>(a->get<const std::string>() + b->getConverted<std::string>()));
        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
        {
            // TODO: in place
            object::type::Array arr = a->get<const object::type::Array>();
            object::type::Array toAdd = b->getConverted<object::type::Array>();
            arr.insert(arr.end(), toAdd.begin(), toAdd.end());
            return object::makeObject(arr);
        }
        return (*(*a)[n::addition])(a, {b}, &st);
    }

    case MULTIPLICATION:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(a->get<const number>() * b->getConverted<number>()));
        else if (a->isOfType<std::string>() && b->isConvertible<number>())
        {
            const std::string &str = a->get<const std::string>();
            int i = static_cast<int>(b->getConverted<number>());
            std::string res;
            res.reserve(str.size() * i);
            while (i--)
                res += str;
            return object::makeObject(res);
        }
        else if (a->isOfType<object::type::Array>() && b->isConvertible<number>())
        {
            const object::type::Array &arr = a->get<const object::type::Array>();
            int i = static_cast<int>(b->getConverted<number>());
            object::type::Array res;
            res.reserve(arr.size() * i);
            while (i--)
                res.insert(res.begin(), arr.begin(), arr.end());
            return object::makeObject(res);
        }
        return (*(*a)[n::multiplication])(a, {b}, &st);
    }

    case SUBTRACTION:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(a->get<const number>() - b->getConverted<number>()));
        return (*(*a)[n::subtraction])(a, {b}, &st);
    }

    case DIVISION:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(a->get<const number>() / b->getConverted<number>()));
        return (*(*a)[n::division])(a, {b}, &st);
    }

    case MODULUS:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(a->get<const number>() % b->getConverted<number>()));
        return (*(*a)[n::modulus])(a, {b}, &st);
    }

    case AND:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        object::objectPtr b;
        if (a->isConvertible<bool>())
        {
            if (!a->getConverted<bool>())
                return object::makeObject(false);
            b = _children[1].evaluate(st);
            if (b->isConvertible<bool>())
                return object::makeObject(b->getConverted<bool>());
        }
        else
            b = _children[1].evaluate(st);
        return (*(*a)[n::and])(a, {b}, &st);
    }

    case OR:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        object::objectPtr b;
        if (a->isConvertible<bool>())
        {
            if (a->getConverted<bool>())
                return a;
            b = _children[1].evaluate(st);
            if (b->isConvertible<bool>())
                return b;
        }
        else
            b = _children[1].evaluate(st);
        return (*(*a)[n::or])(a, {b}, &st);
    }

    case BITWISE_AND:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(static_cast<int>(a->get<const number>()) & static_cast<int>(b->getConverted<number>())));
        return (*(*a)[n::bitwiseAnd])(a, {b}, &st);
    }

    case BITWISE_OR:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(static_cast<int>(a->get<const number>()) | static_cast<int>(b->getConverted<number>())));
        return (*(*a)[n::bitwiseOr])(a, {b}, &st);
    }

    case SHIFT_LEFT:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(static_cast<int>(a->get<const number>()) << static_cast<int>(b->getConverted<number>())));
        return (*(*a)[n::shiftLeft])(a, {b}, &st);
    }

    case SHIFT_RIGHT:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(static_cast<int>(a->get<const number>()) >> static_cast<int>(b->getConverted<number>())));
        return (*(*a)[n::shiftRight])(a, {b}, &st);
    }

    case EQUAL:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->get<const number>() == b->getConverted<number>()));

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->get<const std::string>() == b->getConverted<std::string>()));

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return object::makeObject(static_cast<bool>(a->get<const object::type::Array>() == b->getConverted<object::type::Array>()));

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->get<const bool>() == b->getConverted<bool>()));

        return (*(*a)[n::equal])(a, {b}, &st);
    }

    case NOT_EQUAL:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->get<const number>() != b->getConverted<number>()));

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->get<const std::string>() != b->getConverted<std::string>()));

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return object::makeObject(static_cast<bool>(a->get<const object::type::Array>() != b->getConverted<object::type::Array>()));

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->get<const bool>() != b->getConverted<bool>()));

        return (*(*a)[n::lessEqual])(a, {b}, &st);
    }

    case LESS:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->get<const number>() < b->getConverted<number>()));

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->get<const std::string>() < b->getConverted<std::string>()));

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return object::makeObject(static_cast<bool>(a->get<const object::type::Array>() < b->getConverted<object::type::Array>()));

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->get<const bool>() < b->getConverted<bool>()));

        return (*(*a)[n::less])(a, {b}, &st);
    }

    case GREATER:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->get<const number>() > b->getConverted<number>()));

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->get<const std::string>() > b->getConverted<std::string>()));

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return object::makeObject(static_cast<bool>(a->get<const object::type::Array>() > b->getConverted<object::type::Array>()));

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->get<const bool>() > b->getConverted<bool>()));

        return (*(*a)[n::greater])(a, {b}, &st);
    }

    case LESS_EQUAL:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->get<const number>() <= b->getConverted<number>()));

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->get<const std::string>() <= b->getConverted<std::string>()));

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return object::makeObject(static_cast<bool>(a->get<const object::type::Array>() <= b->getConverted<object::type::Array>()));

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->get<const bool>() <= b->getConverted<bool>()));

        return (*(*a)[n::lessEqual])(a, {b}, &st);
    }

    case GREATER_EQUAL:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->get<const number>() >= b->getConverted<number>()));

        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->get<const std::string>() >= b->getConverted<std::string>()));

        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return object::makeObject(static_cast<bool>(a->get<const object::type::Array>() >= b->getConverted<object::type::Array>()));

        else if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->get<const bool>() >= b->getConverted<bool>()));

        return (*(*a)[n::greaterEqual])(a, {b}, &st);
    }

    case INCREMENT:
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluate(st);
        if (a->isOfType<number>())
        {
            a->get<number>()++;
            return a;
        }
        return (*(*a)[n::increment])(a, {}, &st);
    }

    case DECREMENT:
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluate(st);
        if (a->isOfType<number>())
        {
            a->get<number>()--;
            return a;
        }
        return (*(*a)[n::decrement])(a, {}, &st);
    }

    case NOT:
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluate(st);
        if (a->isConvertible<bool>())
            return object::makeObject(!a->getConverted<bool>());
        return (*(*a)[n::not])(a, {}, &st);
    }

    case COMPLEMENT:
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluate(st);
        if (a->isOfType<number>())
            return object::makeObject(static_cast<number>(~static_cast<int>(a->get<const number>())));
        return (*(*a)[n::complement])(a, {}, &st);
    }

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

    case CALL_OPERATOR:
    {
        assert(_children.size() >= 1);
        auto a = _children[0].evaluate(st);
        object::type::Array args(_children.size() - 1);
        for (size_t i = 1; i < _children.size(); i++)
            args[i - 1] = _children[i].evaluate(st);

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
    }

    case READ_OPERATOR:
    {
        assert(_children.size() >= 1);
        static name readOperator = n::readOperator;
        auto a = _children[0].evaluate(st);
        object::type::Array args(_children.size() - 1);
        for (size_t i = 1; i < _children.size(); i++)
            args[i - 1] = _children[i].evaluate(st);

        try
        {
            (*(*a)[readOperator])(a, std::move(args), &st);
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
    }

    case METHOD_CALL_OPERATOR:
    {
        assert(_children.size() >= 2);
        assert(_children[1]._token == IDENTIFIER);
        auto a = _children[0].evaluate(st);
        auto b = (*a)[_children[1]._text];
        object::type::Array args(_children.size() - 2);
        for (size_t i = 2; i < _children.size(); i++)
            args[i - 2] = _children[i].evaluate(st);

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
    }

    case AWAIT:
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluate(st);
        auto b = (*a)[n::await_];
        (*b)(a, {}, &st);
        return;
    }

    case ARRAY_LITERAL:
    {
        for (size_t i = 0; i < _children.size(); i++)
            _children[i].evaluateVoid(st);
        return;
    }

    case DOT:
    {
        assert(_children.size() == 2);
        assert(_children[1]._token == IDENTIFIER);
        (*(_children[0].evaluate(st)))[_children[1]._text];
        return;
    }

    case LET:
        assert(_children.size() == 0);
        st.insert(_text, object::makeEmptyObject());
        return;

    case ASSIGNMENT:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        if (a->isConst())
            throw std::runtime_error("tried to modify constant value");
        object::objectPtr b;
        if (!numberAssignmentOptimization(a, b, st))
        {
            *a = b;
            a->setConst(false);
        }
        return;
    }

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
            st.insert(static_cast<name>(_children[0].evaluate(st)->getConverted<std::string>()), a);
        return;
    }

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

    case RETURN:
        assert(_children.size() == 1);
        throw _children[0].evaluate(st);

    case THROW:
        assert(_children.size() == 1);
        throw objectException(_children[0].evaluate(st));

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

    case USER_OPERATOR:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        auto op = st[_text];
        (*op)(op, {a, b}, &st);
        return;
    }

    case INSTANCEOF:
        return;

    case ADDITION:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        else if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return;
        else if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return;
        (*(*a)[n::addition])(a, {b}, &st);
        return;
    }

    case MULTIPLICATION:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        else if (a->isOfType<std::string>() && b->isConvertible<number>())
            return;
        else if (a->isOfType<object::type::Array>() && b->isConvertible<number>())
            return;
        (*(*a)[n::multiplication])(a, {b}, &st);
        return;
    }

    case SUBTRACTION:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        (*(*a)[n::subtraction])(a, {b}, &st);
        return;
    }

    case DIVISION:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        (*(*a)[n::division])(a, {b}, &st);
        return;
    }

    case MODULUS:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        (*(*a)[n::modulus])(a, {b}, &st);
        return;
    }

    case AND:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        object::objectPtr b;
        if (a->isConvertible<bool>())
        {
            if (!a->getConverted<bool>())
                return;
            b = _children[1].evaluate(st);
            if (b->isConvertible<bool>())
                return;
        }
        else
            b = _children[1].evaluate(st);
        (*(*a)[n::and])(a, {b}, &st);
        return;
    }

    case OR:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        object::objectPtr b;
        if (a->isConvertible<bool>())
        {
            if (a->getConverted<bool>())
                return;
            b = _children[1].evaluate(st);
            if (b->isConvertible<bool>())
                return;
        }
        else
            b = _children[1].evaluate(st);
        (*(*a)[n::or])(a, {b}, &st);
        return;
    }

    case BITWISE_AND:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        (*(*a)[n::bitwiseAnd])(a, {b}, &st);
        return;
    }

    case BITWISE_OR:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        (*(*a)[n::bitwiseOr])(a, {b}, &st);
        return;
    }

    case SHIFT_LEFT:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        (*(*a)[n::shiftLeft])(a, {b}, &st);
        return;
    }

    case SHIFT_RIGHT:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return;
        (*(*a)[n::shiftRight])(a, {b}, &st);
        return;
    }

    case EQUAL:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
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
    }

    case NOT_EQUAL:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
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
    }

    case LESS:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
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
    }

    case GREATER:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
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
    }

    case LESS_EQUAL:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
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
    }

    case GREATER_EQUAL:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
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
    }

    case INCREMENT:
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluate(st);
        if (a->isOfType<number>())
        {
            a->get<number>()++;
            return;
        }
        (*(*a)[n::increment])(a, {}, &st);
        return;
    }

    case DECREMENT:
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluate(st);
        if (a->isOfType<number>())
        {
            a->get<number>()--;
            return;
        }
        (*(*a)[n::decrement])(a, {}, &st);
        return;
    }

    case NOT:
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluate(st);
        if (a->isConvertible<bool>())
            return;
        (*(*a)[n::not])(a, {}, &st);
        return;
    }

    case COMPLEMENT:
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluate(st);
        if (a->isOfType<number>())
            return;
        (*(*a)[n::complement])(a, {}, &st);
        return;
    }

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
        return st[_text]->getConverted<number>();
    case ADDITION:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluateNumber(st);
        auto b = _children[1].evaluateNumber(st);
        return a + b;
    }

    case MULTIPLICATION:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluateNumber(st);
        auto b = _children[1].evaluateNumber(st);
        return a * b;
    }

    case SUBTRACTION:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluateNumber(st);
        auto b = _children[1].evaluateNumber(st);
        return a - b;
    }

    case DIVISION:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluateNumber(st);
        auto b = _children[1].evaluateNumber(st);
        return a / b;
    }

    case MODULUS:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluateNumber(st);
        auto b = _children[1].evaluateNumber(st);
        return a % b;
    }

    case BITWISE_AND:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluateNumber(st);
        auto b = _children[1].evaluateNumber(st);
        return static_cast<number>(static_cast<int>(a) & static_cast<int>(b));
    }

    case BITWISE_OR:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluateNumber(st);
        auto b = _children[1].evaluateNumber(st);
        return static_cast<number>(static_cast<int>(a) & static_cast<int>(b));
    }

    case SHIFT_LEFT:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluateNumber(st);
        auto b = _children[1].evaluateNumber(st);
        return static_cast<number>(static_cast<int>(a) << static_cast<int>(b));
    }

    case SHIFT_RIGHT:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluateNumber(st);
        auto b = _children[1].evaluateNumber(st);
        return static_cast<number>(static_cast<int>(a) >> static_cast<int>(b));
    }

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

    case COMPLEMENT:
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluateNumber(st);
        return static_cast<number>(~static_cast<int>(a));
    }

    case NUMBER_LITERAL:
        assert(_value != nullptr);
        return _value->get<const number>();

    case CONDITIONAL:
        assert(_children.size() == 3);
        return _children[0].evaluateBoolean(st) ? _children[1].evaluateNumber(st) : _children[2].evaluateNumber(st);
    }
    return evaluate(st)->getConverted<number>();
}

bool Node::evaluateBoolean(stack &st) const
{
    switch (_token)
    {
    case IDENTIFIER:
        return st[_text]->getConverted<bool>();
    case INSTANCEOF:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        return a->hasOwnProperty(n::prototype) && b->hasOwnProperty(n::classPrototype) && (*a)[n::prototype].get() == (*b)[n::classPrototype].get();
    }

    case AND:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        object::objectPtr b;
        if (a->isConvertible<bool>())
        {
            if (!a->getConverted<bool>())
                return false;
            b = _children[1].evaluate(st);
            if (b->isConvertible<bool>())
                return b->getConverted<bool>();
        }
        else
            b = _children[1].evaluate(st);
        return (*(*a)[n::and])(a, {b}, &st)->getConverted<bool>();
    }

    case OR:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        object::objectPtr b;
        if (a->isConvertible<bool>())
        {
            if (a->getConverted<bool>())
                return true;
            b = _children[1].evaluate(st);
            if (b->isConvertible<bool>())
                return b->getConverted<bool>();
        }
        else
            b = _children[1].evaluate(st);
        return (*(*a)[n::or])(a, {b}, &st)->getConverted<bool>();
    }

    case EQUAL:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return a->get<const number>() == b->getConverted<number>();

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return a->get<const std::string>() == b->getConverted<std::string>();

        if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return a->get<const object::type::Array>() == b->getConverted<object::type::Array>();

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return a->get<const bool>() == b->getConverted<bool>();

        return (*(*a)[n::equal])(a, {b}, &st)->getConverted<bool>();
    }

    case NOT_EQUAL:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return a->get<const number>() != b->getConverted<number>();

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return a->get<const std::string>() != b->getConverted<std::string>();

        if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return a->get<const object::type::Array>() != b->getConverted<object::type::Array>();

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return a->get<const bool>() != b->getConverted<bool>();

        return (*(*a)[n::lessEqual])(a, {b}, &st)->getConverted<bool>();
    }

    case LESS:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return a->get<const number>() < b->getConverted<number>();

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return a->get<const std::string>() < b->getConverted<std::string>();

        if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return a->get<const object::type::Array>() < b->getConverted<object::type::Array>();

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return a->get<const bool>() < b->getConverted<bool>();

        return (*(*a)[n::less])(a, {b}, &st)->getConverted<bool>();
    }

    case GREATER:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return a->get<const number>() > b->getConverted<number>();

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return a->get<const std::string>() > b->getConverted<std::string>();

        if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return a->get<const object::type::Array>() > b->getConverted<object::type::Array>();

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return a->get<const bool>() > b->getConverted<bool>();

        return (*(*a)[n::greater])(a, {b}, &st)->getConverted<bool>();
    }

    case LESS_EQUAL:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return a->get<const number>() <= b->getConverted<number>();

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return a->get<const std::string>() <= b->getConverted<std::string>();

        if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return a->get<const object::type::Array>() <= b->getConverted<object::type::Array>();

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return a->get<const bool>() <= b->getConverted<bool>();

        return (*(*a)[n::lessEqual])(a, {b}, &st)->getConverted<bool>();
    }

    case GREATER_EQUAL:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return a->get<const number>() >= b->getConverted<number>();

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return a->get<const std::string>() >= b->getConverted<std::string>();

        if (a->isOfType<object::type::Array>() && b->isConvertible<object::type::Array>())
            return a->get<const object::type::Array>() >= b->getConverted<object::type::Array>();

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return a->get<const bool>() >= b->getConverted<bool>();

        return (*(*a)[n::greaterEqual])(a, {b}, &st)->getConverted<bool>();
    }

    case NOT:
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluate(st);
        if (a->isConvertible<bool>())
            return !a->getConverted<bool>();
        return (*(*a)[n::not])(a, {}, &st)->getConverted<bool>();
    }

    case CONDITIONAL:
        assert(_children.size() == 3);
        return _children[0].evaluateBoolean(st) ? _children[1].evaluateBoolean(st) : _children[2].evaluateBoolean(st);
    }
    return evaluate(st)->getConverted<bool>();
}