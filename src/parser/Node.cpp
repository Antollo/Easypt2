#include "Node.h"
#include "nobject.h"
#include "console.h"
#include <cassert>
#include <exception>

std::vector<std::string> Node::stackTrace;

std::string parseString(const std::string &source)
{
    std::string ret;
    size_t length = source.size() - 1;
    for (size_t i = 1; i < length; i++)
    {
        if (source[i] != '\\')
            ret.push_back(source[i]);
        else
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
                ret.push_back((source[i] - '0') * 100 + (source[i + 1] - '0') * 10 + (source[i + 2] - '0'));
                // TODO throw if too short
                i += 2;
                break;
            }
    }
    return ret;
}

void Node::text(const std::string &t)
{
    switch (_token)
    {
    case NUMBER_LITERAL:
        if (t.find('.') == std::string::npos)
            data->value = object::makeObject(static_cast<number>(std::stoi(t)));
        else
            data->value = object::makeObject(static_cast<number>(std::stod(t)));
        data->value->setConst();
        break;

    case STRING_LITERAL:
        data->value = object::makeObject(parseString(t));
        data->value->setConst();
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
        arg.names() = names();
        names().clear();
        // object::makeObject(Node)
        data->value = object::makeObject(arg);
        data->value->setConst();
        break;
    }
    default:
        _children.push_back(arg);
        break;
    }
}

object::objectPtr Node::evaluate(stack &st) const
{
    switch (_token)
    {
    case COMPOUND_STATEMENT:
    {
        stack localStack(&st);
        size_t i = 0;
        try
        {
            for (; i < _children.size(); i++)
                _children[i].evaluate(localStack);
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
        if (_children[0].evaluate(st)->getConverted<bool>())
            _children[1].evaluate(st);
        return nullptr;

    case IF_ELSE:
        assert(_children.size() == 3);
        if (_children[0].evaluate(st)->getConverted<bool>())
            _children[1].evaluate(st);
        else
            _children[2].evaluate(st);
        return nullptr;

    case WHILE:
        assert(_children.size() == 2);
        while (_children[0].evaluate(st)->getConverted<bool>())
            _children[1].evaluate(st);
        return nullptr;

    case FOR:
        assert(_children.size() == 4);
        _children[0].evaluate(st);
        while (_children[1].evaluate(st)->getConverted<bool>())
        {
            _children[3].evaluate(st);
            _children[2].evaluate(st);
        }
        return nullptr;

    case STATEMENT:
        assert(_children.size() == 1);
        _children[0].evaluate(st);
        return nullptr;

    case CALL_OPERATOR:
    {
        assert(_children.size() >= 1);
        auto a = _children[0].evaluate(st);
        object::arrayType args(_children.size() - 1);
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
        static name readOperator = "readOperator"_n;
        auto a = _children[0].evaluate(st);
        object::arrayType args(_children.size() - 1);
        for (size_t i = 1; i < _children.size(); i++)
            args[i - 1] = _children[i].evaluate(st);

        try
        {
            return (*(*a)[readOperator])(a, std::move(args), &st);
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
        object::arrayType args(_children.size() - 2);
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

    case ARRAY_LITERAL:
    {
        object::arrayType arr(_children.size());
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
        auto b = _children[1].evaluate(st);
        *a = *b;
        a->setConst(false);
        return a;
    }

    case INIT_ASSIGNMENT:
        assert(_children.size() == 2);
        if (_children[0]._token == LET)
        {
            return st.insert(_children[0]._text, _children[1].evaluate(st));
        }
        else if (_children[0]._token == IDENTIFIER)
        {
            assert(!_children[0]._text.isEmpty());
            auto &a = st[_children[0]._text];
            if (a->isConst())
                throw std::runtime_error("tried to modify constant value");
            a = _children[1].evaluate(st);
            return a;
        }
        else if (_children[0]._token == DOT && _children[0]._children[1]._token == IDENTIFIER)
        {
            assert(_children[0]._children.size() == 2);
            auto &a = (*(_children[0].evaluate(st)))[_children[1]._text];
            if (a->isConst())
                throw std::runtime_error("tried to modify constant value");
            a = _children[1].evaluate(st);
            return a;
        }
        throw std::runtime_error("left side of init_assignment is not identifier or dot operator");
        break;

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

    case JSON:
    {
        stack localJsonStack(&st);
        for (auto &child : _children)
            child.evaluate(localJsonStack);

        auto obj = object::makeEmptyObject();
        for (auto x : localJsonStack)
            obj->addProperty(x.first, x.second);
        return obj;
    }

    case CLASS:
    {
        stack localJsonStack(&st);
        for (auto &child : _children)
            child.evaluate(localJsonStack);

        auto obj = object::makeEmptyObject();
        for (auto x : localJsonStack)
            obj->addProperty(x.first, x.second);

        auto Class = st["Class"_n];
        if (!_children.empty() && _children.back()._token == IDENTIFIER)
        {
            auto &prototype = (*obj)[name::prototype];
            prototype = (*_children.back().evaluate(st))["classPrototype"_n];
            (*prototype)["super"_n] = (*prototype)["constructor"_n];
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

    case TRY:
    {
        assert(_children.size() == 2);
        try
        {
            _children[0].evaluate(st);
        }
        catch (objectException &e)
        {
            stackTrace.clear();
            stack localStack(&st);
            localStack.insert("exception"_n, e.getPtr());
            _children[1].evaluate(localStack);
        }
        catch (std::exception &e)
        {
            stackTrace.clear();
            stack localStack(&st);
            localStack.insert("exception"_n, object::makeObject((std::string)e.what()));
            _children[1].evaluate(localStack);
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
        return object::makeObject(a->hasOwnProperty(name::prototype) && b->hasOwnProperty("classPrototype"_n) && (*a)[name::prototype].get() == (*b)["classPrototype"_n].get());
    }

    case ADDITION:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(a->get<const number>() + b->getConverted<number>()));
        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<std::string>(a->get<const std::string>() + b->getConverted<std::string>()));
        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
        {
            // TODO: in place
            object::arrayType arr = a->get<const object::arrayType>();
            object::arrayType toAdd = b->getConverted<object::arrayType>();
            arr.insert(arr.end(), toAdd.begin(), toAdd.end());
            return object::makeObject(arr);
        }
        return (*(*a)["+"_n])(a, {b}, &st);
    }

    case MULTIPLICATION:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(a->get<const number>() * b->getConverted<number>()));
        if (a->isOfType<std::string>() && b->isConvertible<number>())
        {
            const std::string &str = a->get<const std::string>();
            int i = static_cast<int>(b->getConverted<number>());
            std::string res;
            res.reserve(str.size() * i);
            while (i--)
                res += str;
            return object::makeObject(res);
        }
        if (a->isOfType<object::arrayType>() && b->isConvertible<number>())
        {
            const object::arrayType &arr = a->get<const object::arrayType>();
            int i = static_cast<int>(b->getConverted<number>());
            object::arrayType res;
            res.reserve(arr.size() * i);
            while (i--)
                res.insert(res.begin(), arr.begin(), arr.end());
            return object::makeObject(res);
        }
        return (*(*a)["*"_n])(a, {b}, &st);
    }

    case SUBTRACTION:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(a->get<const number>() - b->getConverted<number>()));
        return (*(*a)["-"_n])(a, {b}, &st);
    }

    case DIVISION:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(a->get<const number>() / b->getConverted<number>()));
        return (*(*a)["/"_n])(a, {b}, &st);
    }

    case MODULUS:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(a->get<const number>() % b->getConverted<number>()));
        return (*(*a)["%"_n])(a, {b}, &st);
    }

    case AND:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        object::objectPtr b;
        if (a->isOfType<bool>())
        {
            if (a->get<const bool>() == false)
                return object::makeObject(false);
            b = _children[1].evaluate(st);
            if (b->isConvertible<bool>())
                return object::makeObject(b->getConverted<bool>());
        }
        else
            b = _children[1].evaluate(st);
        return (*(*a)["&&"_n])(a, {b}, &st);
    }

    case OR:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->get<const bool>() || b->getConverted<bool>()));
        return (*(*a)["&&"_n])(a, {b}, &st);
    }

    case BITWISE_AND:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(static_cast<int>(a->get<const number>()) & static_cast<int>(b->getConverted<number>())));
        return (*(*a)["&"_n])(a, {b}, &st);
    }

    case BITWISE_OR:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(static_cast<int>(a->get<const number>()) | static_cast<int>(b->getConverted<number>())));
        return (*(*a)["|"_n])(a, {b}, &st);
    }

    case SHIFT_LEFT:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(static_cast<int>(a->get<const number>()) << static_cast<int>(b->getConverted<number>())));
        return (*(*a)["<<"_n])(a, {b}, &st);
    }

    case SHIFT_RIGHT:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<number>(static_cast<int>(a->get<const number>()) >> static_cast<int>(b->getConverted<number>())));
        return (*(*a)[">>"_n])(a, {b}, &st);
    }

    case EQUAL:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->get<const number>() == b->getConverted<number>()));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->get<const std::string>() == b->getConverted<std::string>()));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return object::makeObject(static_cast<bool>(a->get<const object::arrayType>() == b->getConverted<object::arrayType>()));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->get<const bool>() == b->getConverted<bool>()));

        return (*(*a)["=="_n])(a, {b}, &st);
    }

    case NOT_EQUAL:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->get<const number>() != b->getConverted<number>()));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->get<const std::string>() != b->getConverted<std::string>()));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return object::makeObject(static_cast<bool>(a->get<const object::arrayType>() != b->getConverted<object::arrayType>()));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->get<const bool>() != b->getConverted<bool>()));

        return (*(*a)["!="_n])(a, {b}, &st);
    }

    case LESS:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->get<const number>() < b->getConverted<number>()));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->get<const std::string>() < b->getConverted<std::string>()));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return object::makeObject(static_cast<bool>(a->get<const object::arrayType>() < b->getConverted<object::arrayType>()));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->get<const bool>() < b->getConverted<bool>()));

        return (*(*a)["<"_n])(a, {b}, &st);
    }

    case GREATER:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->get<const number>() > b->getConverted<number>()));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->get<const std::string>() > b->getConverted<std::string>()));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return object::makeObject(static_cast<bool>(a->get<const object::arrayType>() > b->getConverted<object::arrayType>()));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->get<const bool>() > b->getConverted<bool>()));

        return (*(*a)[">"_n])(a, {b}, &st);
    }

    case LESS_EQUAL:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->get<const number>() <= b->getConverted<number>()));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->get<const std::string>() <= b->getConverted<std::string>()));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return object::makeObject(static_cast<bool>(a->get<const object::arrayType>() <= b->getConverted<object::arrayType>()));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->get<const bool>() <= b->getConverted<bool>()));

        return (*(*a)["<="_n])(a, {b}, &st);
    }

    case GREATER_EQUAL:
    {
        assert(_children.size() == 2);
        auto a = _children[0].evaluate(st);
        auto b = _children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return object::makeObject(static_cast<bool>(a->get<const number>() >= b->getConverted<number>()));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return object::makeObject(static_cast<bool>(a->get<const std::string>() >= b->getConverted<std::string>()));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return object::makeObject(static_cast<bool>(a->get<const object::arrayType>() >= b->getConverted<object::arrayType>()));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return object::makeObject(static_cast<bool>(a->get<const bool>() >= b->getConverted<bool>()));

        return (*(*a)[">="_n])(a, {b}, &st);
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
        return (*(*a)["++"_n])(a, {}, &st);
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
        return (*(*a)["--"_n])(a, {}, &st);
    }

    case NOT:
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluate(st);
        if (a->isConvertible<bool>())
            return object::makeObject(!a->getConverted<bool>());
        return (*(*a)["!"_n])(a, {}, &st);
    }

    case COMPLEMENT:
    {
        assert(_children.size() == 1);
        auto a = _children[0].evaluate(st);
        if (a->isOfType<number>())
            return object::makeObject(static_cast<number>(~static_cast<int>(a->get<const number>())));
        return (*(*a)["~"_n])(a, {}, &st);
    }

    case IDENTIFIER:
        assert(!_text.isEmpty());
        return st[_text];

    case FUNCTION:
        if (!_text.isEmpty())
            st.insert(_text, data->value);
    case NUMBER_LITERAL:
    case STRING_LITERAL:
        assert(data->value != nullptr);
        return data->value;

    default:
        for (auto &child : _children)
            console::warn(LLgetSymbol(child._token));
        break;
    }
    return object::makeEmptyObject();
}

std::string Node::toString() const
{
    switch (_token)
    {
    case IDENTIFIER:
    case STRING_LITERAL:
        return static_cast<std::string>(_text);
    case NUMBER_LITERAL:
        return static_cast<std::string>(_text);
    case DOT:
        return "<" + _children[0].toString() + "." + _children[1].toString() + ">";
    case CALL_OPERATOR:
    {
        std::string r = "<" + _children[0].toString() + ">(";
        for (size_t i = 1; i < _children.size(); i++)
            r += _children[i].toString() + ", ";
        if (r.back() == ' ')
        {
            r.pop_back();
            r.back() = ')';
        }
        else
            r.push_back(')');
        return r;
    }
    case READ_OPERATOR:
    {
        std::string r = "<" + _children[0].toString() + ">[";
        for (size_t i = 1; i < _children.size(); i++)
            r += _children[i].toString() + ", ";
        if (r.back() == ' ')
        {
            r.pop_back();
            r.back() = ']';
        }
        else
            r.push_back(']');
        return r;
    }
    case METHOD_CALL_OPERATOR:
    {
        std::string r = "<" + _children[0].toString() + "." + _children[1].toString() + ">(";
        for (size_t i = 2; i < _children.size(); i++)
            r += _children[i].toString() + ", ";
        if (r.back() == ' ')
        {
            r.pop_back();
            r.back() = ')';
        }
        else
            r.push_back(')');
        return r;
    }
    case STATEMENT:
        return _children[0].toString();
    case COMPOUND_STATEMENT:
    {
        std::string r = "{\n";
        for (const auto &arg : _children)
            r += arg.toString() + "\n";
        r += "}";
        return r;
    }
    case ARRAY_LITERAL:
    {
        std::string r = "Array [";
        for (const auto &arg : _children)
            r += arg.toString() + ", ";
        if (r.back() == ' ')
        {
            r.pop_back();
            r.back() = ']';
        }
        else
            r.push_back(']');
        return r;
    }
    case IF:
    case IF_ELSE:
    {
        std::string r = "if (" + _children[0].toString() + ") {\n";
        for (size_t i = 1; i < _children.size(); i++)
            r += _children[i].toString() + "\n";
        r += "}";
        return r;
    }
    case WHILE:
    {
        std::string r = "while (" + _children[0].toString() + ") {\n";
        for (size_t i = 1; i < _children.size(); i++)
            r += _children[i].toString() + "\n";
        r += "}";
        return r;
    }
    case LET:
        return "let " + static_cast<std::string>(_text);
    case RETURN:
        return "return " + _children[0].toString();
    case THROW:
        return "return " + _children[0].toString();
    case TRY:
    {
        std::string r = "try ";
        r += _children[0].toString() + " catch ";
        r += _children[1].toString();
        return r;
    }
    case FUNCTION:
    {
        std::string r = "function {\n";
        for (const auto &arg : _children)
            r += arg.toString() + "\n";
        r += "}";
        return r;
    }
    case JSON:
    {
        std::string r = "json {\n";
        for (const auto &arg : _children)
            r += arg.toString() + "\n";
        r += "}";
        return r;
    }
    case CLASS:
    {
        std::string r = "class " + static_cast<std::string>(_text) + " {\n";
        for (const auto &arg : _children)
            r += arg.toString() + "\n";
        r += "}";
        return r;
    }
    default:
        switch (_children.size())
        {
        case 0:
            return "<" + toName() + ">";
        case 1:
            return "<" + toName() + " " + _children[0].toString() + ">";
        case 2:
            return "<" + _children[0].toString() + " " + toName() + " " + _children[1].toString() + ">";
        default:
        {
            std::string r = "<" + toName();
            for (const auto &arg : _children)
                r += " " + arg.toString();
            r += ">";
            return r;
        }
        }
    }
}