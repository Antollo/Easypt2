#include "token.h"
#include "nobject.h"

std::string parseString(const std::string &source)
{
    std::string ret;
    size_t length = source.size() - 1;
    ret.reserve(length - 2);
    for (size_t i = 1; i < length; i++)
    {
        if (source[i] != '\\')
            ret.push_back(source[i]);
        else
        {
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
                i += 2;
                break;
            }
        }
    }
    return ret;
}

token::token(const std::string &n, tokenType t) : name(n), arity(-1), type(t), value(nullptr)
{
    switch (type)
    {
    case tokenType::NumberLiteral:
        type = tokenType::Value;
        if (n.find('.') == std::string::npos)
            value = makeObject(static_cast<number>(std::stoi(n)));
        else
            value = makeObject(static_cast<number>(std::stod(n)));
        value->setConst();
        break;

    case tokenType::StringLiteral:
        type = tokenType::Value;
        value = makeObject(parseString(n));
        value->setConst();
        break;

    default:
        break;
    }
};

object::objectPtr& token::resolve(stack *st) const
{
    switch (type)
    {
    case token::tokenType::Name:
    case token::tokenType::CallOperator:
        return (*st)[name];
        break;

    case token::tokenType::Value:
        return value;
        break;

    default:
        throw std::runtime_error("tried to resolve wrong token: " + static_cast<std::string>(name));
        break;
    }
}