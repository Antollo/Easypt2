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
            data->value = makeObject(static_cast<number>(std::stoi(t)));
        else
            data->value = makeObject(static_cast<number>(std::stod(t)));
        data->value->setConst();
        break;

    case STRING_LITERAL:
        data->value = makeObject(parseString(t));
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
        // makeObject(Node)
        data->value = makeObject(arg);
        data->value->setConst();
        break;
    }
    default:
        children.push_back(arg);
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
            for (; i < children.size(); i++)
                children[i].evaluate(localStack);
        }
        catch (objectException &e)
        {
            children[i].exception();
            throw e;
        }
        catch (std::exception &e)
        {
            children[i].exception();
            throw e;
        }
        return nullptr;
    }

    case IF:
        assert(children.size() == 2);
        if (children[0].evaluate(st)->getConverted<bool>())
            children[1].evaluate(st);
        return nullptr;

    case IF_ELSE:
        assert(children.size() == 3);
        if (children[0].evaluate(st)->getConverted<bool>())
            children[1].evaluate(st);
        else
            children[2].evaluate(st);
        return nullptr;

    case WHILE:
        assert(children.size() == 2);
        while (children[0].evaluate(st)->getConverted<bool>())
            children[1].evaluate(st);
        return nullptr;

    case FOR:
        assert(children.size() == 4);
        children[0].evaluate(st);
        while (children[1].evaluate(st)->getConverted<bool>())
        {
            children[3].evaluate(st);
            children[2].evaluate(st);
        }
        return nullptr;

    case STATEMENT:
        assert(children.size() == 1);
        children[0].evaluate(st);
        return nullptr;

    case CALL_OPERATOR:
    {
        assert(children.size() >= 1);
        auto a = children[0].evaluate(st);
        object::arrayType args(children.size() - 1);
        for (size_t i = 1; i < children.size(); i++)
            args[i - 1] = children[i].evaluate(st);

        try
        {
            return (*a)(nullptr, std::move(args), &st);
        }
        catch (objectException &e)
        {
            children[0].exception(children[0].toName());
            throw e;
        }
        catch (std::exception &e)
        {
            children[0].exception(children[0].toName());
            throw e;
        }
    }

    case READ_OPERATOR:
    {
        assert(children.size() >= 1);
        static name readOperator = "readOperator"_n;
        auto a = children[0].evaluate(st);
        object::arrayType args(children.size() - 1);
        for (size_t i = 1; i < children.size(); i++)
            args[i - 1] = children[i].evaluate(st);

        try
        {
            return (*(*a)[readOperator])(a, std::move(args), &st);
        }
        catch (objectException &e)
        {
            children[0].exception(children[0].toName() + ".readOperator");
            throw e;
        }
        catch (std::exception &e)
        {
            children[0].exception(children[0].toName() + ".readOperator");
            throw e;
        }
    }

    case METHOD_CALL_OPERATOR:
    {
        assert(children.size() >= 2);
        assert(children[1]._token == IDENTIFIER);
        auto a = children[0].evaluate(st);
        auto b = (*a)[children[1]._text];
        object::arrayType args(children.size() - 2);
        for (size_t i = 2; i < children.size(); i++)
            args[i - 2] = children[i].evaluate(st);

        try
        {
            return (*b)(a, std::move(args), &st);
        }
        catch (objectException &e)
        {
            children[0].exception(children[0].toName() + "." + children[1].toName());
            throw e;
        }
        catch (std::exception &e)
        {
            children[0].exception(children[0].toName() + "." + children[1].toName());
            throw e;
        }
    }

    case ARRAY_LITERAL:
    {
        object::arrayType arr(children.size());
        for (size_t i = 0; i < children.size(); i++)
            arr[i] = children[i].evaluate(st);
        return makeObject(arr);
    }

    case DOT:
    {
        assert(children.size() == 2);
        assert(children[1]._token == IDENTIFIER);
        return (*(children[0].evaluate(st)))[children[1]._text];
    }

    case LET:
        assert(children.size() == 0);
        return st.insert(_text, makeEmptyObject());

    case ASSIGNMENT:
    {
        assert(children.size() == 2);
        auto a = children[0].evaluate(st);
        if (a->isConst())
            throw std::runtime_error("tried to modify constant value");
        auto b = children[1].evaluate(st);
        *a = *b;
        a->setConst(false);
        return a;
    }

    case INIT_ASSIGNMENT:
        assert(children.size() == 2);
        if (children[0]._token == LET)
        {
            return st.insert(children[0]._text, children[1].evaluate(st));
        }
        else if (children[0]._token == IDENTIFIER)
        {
            assert(!children[0]._text.isEmpty());
            auto &a = st[children[0]._text];
            if (a->isConst())
                throw std::runtime_error("tried to modify constant value");
            a = children[1].evaluate(st);
            return a;
        }
        else if (children[0]._token == DOT && children[0].children[1]._token == IDENTIFIER)
        {
            assert(children[0].children.size() == 2);
            auto &a = (*(children[0].evaluate(st)))[children[1]._text];
            if (a->isConst())
                throw std::runtime_error("tried to modify constant value");
            a = children[1].evaluate(st);
            return a;
        }
        throw std::runtime_error("left side of init_assignment is not identifier or dot operator");
        break;

    case JSON_ASSIGNMENT:
    {
        assert(children.size() == 2);
        auto a = children[1].evaluate(st);
        if (a->isConst())
        {
            auto b = makeEmptyObject();
            *b = *a;
            b->setConst(false);
            a = b;
        }
        if (children[0]._token == IDENTIFIER)
            return st.insert(children[0]._text, a);
        else
            return st.insert(static_cast<name>(children[0].evaluate(st)->getConverted<std::string>()), a);
    }
    case JSON:
    {
        stack localJsonStack(&st);
        for (auto &child : children)
            child.evaluate(localJsonStack);

        auto obj = makeEmptyObject();
        for (auto x : localJsonStack)
            obj->addProperty(x.first, x.second);
        return obj;
    }

    case CLASS:
    {
        stack localJsonStack(&st);
        for (auto &child : children)
            child.evaluate(localJsonStack);

        auto obj = makeEmptyObject();
        for (auto x : localJsonStack)
            obj->addProperty(x.first, x.second);

        auto Class = st["Class"_n];
        obj = (*Class)(nullptr, {obj}, &st);
        st.insert(_text, obj);
        return obj;
    }

    case RETURN:
        assert(children.size() == 1);
        throw children[0].evaluate(st);

    case THROW:
        assert(children.size() == 1);
        throw objectException(children[0].evaluate(st));

    case TRY:
    {
        assert(children.size() == 2);
        try
        {
            children[0].evaluate(st);
        }
        catch (objectException &e)
        {
            stackTrace.clear();
            stack localStack(&st);
            localStack.insert("exception"_n, e.getPtr());
            children[1].evaluate(localStack);
        }
        catch (std::exception &e)
        {
            stackTrace.clear();
            stack localStack(&st);
            localStack.insert("exception"_n, makeObject((std::string)e.what()));
            children[1].evaluate(localStack);
        }
        return nullptr;
    }

    case USER_OPERATOR:
    {
        assert(children.size() == 2);
        auto a = children[0].evaluate(st);
        auto b = children[1].evaluate(st);
        return (*st[_text])(nullptr, {a, b}, &st);
    }

    case ADDITION:
    {
        assert(children.size() == 2);
        auto a = children[0].evaluate(st);
        auto b = children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return makeObject(static_cast<number>(a->get<const number>() + b->getConverted<number>()));
        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return makeObject(static_cast<std::string>(a->get<const std::string>() + b->getConverted<std::string>()));
        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
        {
            // TODO: in place
            object::arrayType arr = a->get<const object::arrayType>();
            object::arrayType toAdd = b->getConverted<object::arrayType>();
            arr.insert(arr.end(), toAdd.begin(), toAdd.end());
            return makeObject(arr);
        }
        return (*(*a)["+"_n])(a, {b}, &st);
    }
    case MULTIPLICATION:
    {
        assert(children.size() == 2);
        auto a = children[0].evaluate(st);
        auto b = children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return makeObject(static_cast<number>(a->get<const number>() * b->getConverted<number>()));
        if (a->isOfType<std::string>() && b->isConvertible<number>())
        {
            const std::string &str = a->get<const std::string>();
            int i = static_cast<int>(b->getConverted<number>());
            std::string res;
            res.reserve(str.size() * i);
            while (i--)
                res += str;
            return makeObject(res);
        }
        if (a->isOfType<object::arrayType>() && b->isConvertible<number>())
        {
            const object::arrayType &arr = a->get<const object::arrayType>();
            int i = static_cast<int>(b->getConverted<number>());
            object::arrayType res;
            res.reserve(arr.size() * i);
            while (i--)
                res.insert(res.begin(), arr.begin(), arr.end());
            return makeObject(res);
        }
        return (*(*a)["*"_n])(a, {b}, &st);
    }
    case SUBTRACTION:
    {
        assert(children.size() == 2);
        auto a = children[0].evaluate(st);
        auto b = children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return makeObject(static_cast<number>(a->get<const number>() - b->getConverted<number>()));
        return (*(*a)["-"_n])(a, {b}, &st);
    }
    case DIVISION:
    {
        assert(children.size() == 2);
        auto a = children[0].evaluate(st);
        auto b = children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return makeObject(static_cast<number>(a->get<const number>() / b->getConverted<number>()));
        return (*(*a)["/"_n])(a, {b}, &st);
    }
    case MODULUS:
    {
        assert(children.size() == 2);
        auto a = children[0].evaluate(st);
        auto b = children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return makeObject(static_cast<number>(a->get<const number>() % b->getConverted<number>()));
        return (*(*a)["%"_n])(a, {b}, &st);
    }
    case AND:
    {
        assert(children.size() == 2);
        auto a = children[0].evaluate(st);
        auto b = children[1].evaluate(st);
        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return makeObject(static_cast<bool>(a->get<const bool>() && b->getConverted<bool>()));
        return (*(*a)["&&"_n])(a, {b}, &st);
    }
    case EQUAL:
    {
        assert(children.size() == 2);
        auto a = children[0].evaluate(st);
        auto b = children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return makeObject(static_cast<bool>(a->get<const number>() == b->getConverted<number>()));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return makeObject(static_cast<bool>(a->get<const std::string>() == b->getConverted<std::string>()));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return makeObject(static_cast<bool>(a->get<const object::arrayType>() == b->getConverted<object::arrayType>()));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return makeObject(static_cast<bool>(a->get<const bool>() == b->getConverted<bool>()));

        return (*(*a)["=="_n])(a, {b}, &st);
    }
    case NOT_EQUAL:
    {
        assert(children.size() == 2);
        auto a = children[0].evaluate(st);
        auto b = children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return makeObject(static_cast<bool>(a->get<const number>() != b->getConverted<number>()));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return makeObject(static_cast<bool>(a->get<const std::string>() != b->getConverted<std::string>()));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return makeObject(static_cast<bool>(a->get<const object::arrayType>() != b->getConverted<object::arrayType>()));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return makeObject(static_cast<bool>(a->get<const bool>() != b->getConverted<bool>()));

        return (*(*a)["!="_n])(a, {b}, &st);
    }
    case LESS:
    {
        assert(children.size() == 2);
        auto a = children[0].evaluate(st);
        auto b = children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return makeObject(static_cast<bool>(a->get<const number>() < b->getConverted<number>()));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return makeObject(static_cast<bool>(a->get<const std::string>() < b->getConverted<std::string>()));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return makeObject(static_cast<bool>(a->get<const object::arrayType>() < b->getConverted<object::arrayType>()));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return makeObject(static_cast<bool>(a->get<const bool>() < b->getConverted<bool>()));

        return (*(*a)["<"_n])(a, {b}, &st);
    }
    case GREATER:
    {
        assert(children.size() == 2);
        auto a = children[0].evaluate(st);
        auto b = children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return makeObject(static_cast<bool>(a->get<const number>() > b->getConverted<number>()));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return makeObject(static_cast<bool>(a->get<const std::string>() > b->getConverted<std::string>()));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return makeObject(static_cast<bool>(a->get<const object::arrayType>() > b->getConverted<object::arrayType>()));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return makeObject(static_cast<bool>(a->get<const bool>() > b->getConverted<bool>()));

        return (*(*a)[">"_n])(a, {b}, &st);
    }
    case LESS_EQUAL:
    {
        assert(children.size() == 2);
        auto a = children[0].evaluate(st);
        auto b = children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return makeObject(static_cast<bool>(a->get<const number>() <= b->getConverted<number>()));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return makeObject(static_cast<bool>(a->get<const std::string>() <= b->getConverted<std::string>()));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return makeObject(static_cast<bool>(a->get<const object::arrayType>() <= b->getConverted<object::arrayType>()));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return makeObject(static_cast<bool>(a->get<const bool>() <= b->getConverted<bool>()));

        return (*(*a)["<="_n])(a, {b}, &st);
    }
    case GREATER_EQUAL:
    {
        assert(children.size() == 2);
        auto a = children[0].evaluate(st);
        auto b = children[1].evaluate(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return makeObject(static_cast<bool>(a->get<const number>() >= b->getConverted<number>()));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return makeObject(static_cast<bool>(a->get<const std::string>() >= b->getConverted<std::string>()));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return makeObject(static_cast<bool>(a->get<const object::arrayType>() >= b->getConverted<object::arrayType>()));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return makeObject(static_cast<bool>(a->get<const bool>() >= b->getConverted<bool>()));

        return (*(*a)[">="_n])(a, {b}, &st);
    }
    case INCREMENT:
    {
        assert(children.size() == 1);
        auto a = children[0].evaluate(st);
        if (a->isOfType<number>())
        {
            a->get<number>()++;
            return a;
        }
        return (*(*a)["++"_n])(a, {}, &st);
    }
    case DECREMENT:
    {
        assert(children.size() == 1);
        auto a = children[0].evaluate(st);
        if (a->isOfType<number>())
        {
            a->get<number>()--;
            return a;
        }
        return (*(*a)["--"_n])(a, {}, &st);
    }

    case NOT:
    {
        assert(children.size() == 1);
        auto a = children[0].evaluate(st);
        if (a->isConvertible<bool>())
            return makeObject(!a->getConverted<bool>());
        return (*(*a)["!"_n])(a, {}, &st);
    }

    case IDENTIFIER:
        assert(!_text.isEmpty());
        return st[_text];

    case NUMBER_LITERAL:
    case STRING_LITERAL:
    case FUNCTION:
        assert(data->value != nullptr);
        return data->value;

    default:
        for (auto &child : children)
            console::warn(LLgetSymbol(child._token));
        break;
    }
    return makeEmptyObject();
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
        return "<" + children[0].toString() + "." + children[1].toString() + ">";
    case CALL_OPERATOR:
    {
        std::string r = "<" + children[0].toString() + ">(";
        for (size_t i = 1; i < children.size(); i++)
            r += children[i].toString() + ", ";
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
        std::string r = "<" + children[0].toString() + ">[";
        for (size_t i = 1; i < children.size(); i++)
            r += children[i].toString() + ", ";
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
        std::string r = "<" + children[0].toString() + "." + children[1].toString() + ">(";
        for (size_t i = 2; i < children.size(); i++)
            r += children[i].toString() + ", ";
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
        return children[0].toString();
    case COMPOUND_STATEMENT:
    {
        std::string r = "{\n";
        for (const auto &arg : children)
            r += arg.toString() + "\n";
        r += "}";
        return r;
    }
    case ARRAY_LITERAL:
    {
        std::string r = "Array [";
        for (const auto &arg : children)
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
        std::string r = "if (" + children[0].toString() + ") {\n";
        for (size_t i = 1; i < children.size(); i++)
            r += children[i].toString() + "\n";
        r += "}";
        return r;
    }
    case WHILE:
    {
        std::string r = "while (" + children[0].toString() + ") {\n";
        for (size_t i = 1; i < children.size(); i++)
            r += children[i].toString() + "\n";
        r += "}";
        return r;
    }
    case LET:
        return "let " + static_cast<std::string>(_text);
    case RETURN:
        return "return " + children[0].toString();
    case THROW:
        return "return " + children[0].toString();
    case TRY:
    {
        std::string r = "try ";
        r += children[0].toString() + " catch ";
        r += children[1].toString();
        return r;
    }
    case FUNCTION:
    {
        std::string r = "function {\n";
        for (const auto &arg : children)
            r += arg.toString() + "\n";
        r += "}";
        return r;
    }
    case JSON:
    {
        std::string r = "json {\n";
        for (const auto &arg : children)
            r += arg.toString() + "\n";
        r += "}";
        return r;
    }
    case CLASS:
    {
        std::string r = "class " + static_cast<std::string>(_text) + " {\n";
        for (const auto &arg : children)
            r += arg.toString() + "\n";
        r += "}";
        return r;
    }
    default:
        switch (children.size())
        {
        case 0:
            return "<" + toName() + ">";
        case 1:
            return "<" + toName() + " " + children[0].toString() + ">";
        case 2:
            return "<" + children[0].toString() + " " + toName() + " " + children[1].toString() + ">";
        default:
        {
            std::string r = "<" + toName();
            for (const auto &arg : children)
                r += " " + arg.toString();
            r += ">";
            return r;
        }
        }
    }
}