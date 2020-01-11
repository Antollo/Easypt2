#include "../library.h"

void operators::init(stack *st)
{
addOperatorL("+"_n, {
        object::objectPtr &a = args[0].resolve(st);
        object::objectPtr &b = args[1].resolve(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return token(makeObject(static_cast<number>(a->get<const number>() + b->getConverted<number>())));
        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return token(makeObject(static_cast<std::string>(a->get<const std::string>() + b->getConverted<std::string>())));
        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
        {
            object::arrayType arr = a->get<const object::arrayType>();
            object::arrayType toAdd = b->getConverted<object::arrayType>();
            arr.insert(arr.end(), toAdd.begin(), toAdd.end());
            return token(makeObject(arr));
        }
        return token((*(*a)["+"_n])(a, {b}, st));
    });
    addOperatorL("*"_n, {
        object::objectPtr &a = args[0].resolve(st);
        object::objectPtr &b = args[1].resolve(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return token(makeObject(static_cast<number>(a->get<const number>() * b->getConverted<number>())));
        if (a->isOfType<std::string>() && b->isConvertible<number>())
        {
            const std::string &str = a->get<const std::string>();
            int i = static_cast<int>(b->getConverted<number>());
            std::string res;
            res.reserve(str.size() * i);
            while (i--)
                res += str;
            return token(makeObject(res));
        }
        if (a->isOfType<object::arrayType>() && b->isConvertible<number>())
        {
            const object::arrayType &arr = a->get<const object::arrayType>();
            int i = static_cast<int>(b->getConverted<number>());
            object::arrayType res;
            res.reserve(arr.size() * i);
            while (i--)
                res.insert(res.begin(), arr.begin(), arr.end());
            return token(makeObject(res));
        }
        return token((*(*a)["*"_n])(a, {b}, st));
    });
    addOperatorL("-"_n, {
        object::objectPtr &a = args[0].resolve(st);
        object::objectPtr &b = args[1].resolve(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return token(makeObject(static_cast<number>(a->get<const number>() - b->getConverted<number>())));
        return token((*(*a)["-"_n])(a, {b}, st));
    });
    addOperatorL("/"_n, {
        object::objectPtr &a = args[0].resolve(st);
        object::objectPtr &b = args[1].resolve(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return token(makeObject(static_cast<number>(a->get<const number>() / b->getConverted<number>())));
        return token((*(*a)["/"_n])(a, {b}, st));
    });
    addOperatorL("%"_n, {
        object::objectPtr &a = args[0].resolve(st);
        object::objectPtr &b = args[1].resolve(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return token(makeObject(static_cast<number>(a->get<const number>() % b->getConverted<number>())));
        return token((*(*a)["%"_n])(a, {b}, st));
    });
    addOperatorL("&&"_n, {
        object::objectPtr &a = args[0].resolve(st);
        object::objectPtr &b = args[1].resolve(st);
        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return token(makeObject(static_cast<bool>(a->get<const bool>() && b->getConverted<bool>())));
        return token((*(*a)["&&"_n])(a, {b}, st));
    });
    addOperatorL("=="_n, {
        object::objectPtr &a = args[0].resolve(st);
        object::objectPtr &b = args[1].resolve(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return token(makeObject(static_cast<bool>(a->get<const number>() == b->getConverted<number>())));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return token(makeObject(static_cast<bool>(a->get<const std::string>() == b->getConverted<std::string>())));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return token(makeObject(static_cast<bool>(a->get<const object::arrayType>() == b->getConverted<object::arrayType>())));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return token(makeObject(static_cast<bool>(a->get<const bool>() == b->getConverted<bool>())));

        return token((*(*a)["=="_n])(a, {b}, st));
    });
    addOperatorL("!="_n, {
        object::objectPtr &a = args[0].resolve(st);
        object::objectPtr &b = args[1].resolve(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return token(makeObject(static_cast<bool>(a->get<const number>() != b->getConverted<number>())));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return token(makeObject(static_cast<bool>(a->get<const std::string>() != b->getConverted<std::string>())));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return token(makeObject(static_cast<bool>(a->get<const object::arrayType>() != b->getConverted<object::arrayType>())));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return token(makeObject(static_cast<bool>(a->get<const bool>() != b->getConverted<bool>())));

        return token((*(*a)["!="_n])(a, {b}, st));
    });
    addOperatorL("<"_n, {
        object::objectPtr &a = args[0].resolve(st);
        object::objectPtr &b = args[1].resolve(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return token(makeObject(static_cast<bool>(a->get<const number>() < b->getConverted<number>())));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return token(makeObject(static_cast<bool>(a->get<const std::string>() < b->getConverted<std::string>())));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return token(makeObject(static_cast<bool>(a->get<const object::arrayType>() < b->getConverted<object::arrayType>())));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return token(makeObject(static_cast<bool>(a->get<const bool>() < b->getConverted<bool>())));

        return token((*(*a)["<"_n])(a, {b}, st));
    });
    addOperatorL(">"_n, {
        object::objectPtr &a = args[0].resolve(st);
        object::objectPtr &b = args[1].resolve(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return token(makeObject(static_cast<bool>(a->get<const number>() > b->getConverted<number>())));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return token(makeObject(static_cast<bool>(a->get<const std::string>() > b->getConverted<std::string>())));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return token(makeObject(static_cast<bool>(a->get<const object::arrayType>() > b->getConverted<object::arrayType>())));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return token(makeObject(static_cast<bool>(a->get<const bool>() > b->getConverted<bool>())));

        return token((*(*a)[">"_n])(a, {b}, st));
    });
    addOperatorL("<="_n, {
        object::objectPtr &a = args[0].resolve(st);
        object::objectPtr &b = args[1].resolve(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return token(makeObject(static_cast<bool>(a->get<const number>() <= b->getConverted<number>())));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return token(makeObject(static_cast<bool>(a->get<const std::string>() <= b->getConverted<std::string>())));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return token(makeObject(static_cast<bool>(a->get<const object::arrayType>() <= b->getConverted<object::arrayType>())));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return token(makeObject(static_cast<bool>(a->get<const bool>() <= b->getConverted<bool>())));

        return token((*(*a)["<="_n])(a, {b}, st));
    });
    addOperatorL(">="_n, {
        object::objectPtr &a = args[0].resolve(st);
        object::objectPtr &b = args[1].resolve(st);
        if (a->isOfType<number>() && b->isConvertible<number>())
            return token(makeObject(static_cast<bool>(a->get<const number>() >= b->getConverted<number>())));

        if (a->isOfType<std::string>() && b->isConvertible<std::string>())
            return token(makeObject(static_cast<bool>(a->get<const std::string>() >= b->getConverted<std::string>())));

        if (a->isOfType<object::arrayType>() && b->isConvertible<object::arrayType>())
            return token(makeObject(static_cast<bool>(a->get<const object::arrayType>() >= b->getConverted<object::arrayType>())));

        if (a->isOfType<bool>() && b->isConvertible<bool>())
            return token(makeObject(static_cast<bool>(a->get<const bool>() >= b->getConverted<bool>())));

        return token((*(*a)[">="_n])(a, {b}, st));
    });
    addOperatorL("++"_n, {
        object::objectPtr &a = args[0].resolve(st);
        if (a->isOfType<number>())
        {
            a->get<number>()++;
            return token(a);
        }
        return token((*(*a)["++"_n])(a, {}, st));
    });
    addOperatorL("--"_n, {
        object::objectPtr &a = args[0].resolve(st);
        if (a->isOfType<number>())
        {
            a->get<number>()--;
            return token(a);
        }
        return token((*(*a)["--"_n])(a, {}, st));
    });
    addOperatorL("-u"_n, {
        object::objectPtr &a = args[0].resolve(st);
        if (a->isOfType<number>())
            return token(makeObject(0_n-static_cast<number>(a->get<const number>())));
        return token((*(*a)["-"_n])(a, {}, st));
    });
    addOperatorL("!"_n, {
        object::objectPtr &a = args[0].resolve(st);
        if (a->isConvertible<bool>())
            return token(makeObject(!a->getConverted<bool>()));
        return token((*(*a)["-"_n])(a, {}, st));
    });
    addOperatorL("="_n, {
        object::objectPtr &a = args[0].resolve(st);
        if (a->isConst())
            throw std::runtime_error("tried to modify constant value");
        *a = *args[1].resolve(st);
        a->setConst(false);
        return a;
    });
    addOperatorL("let"_n, {
        return insertObject(args[0].getName(), nullptr);
    });
    addOperatorL(":"_n, {
        /*if (args[0].getType() == token::tokenType::StringLiteral)
        {
            console::log((int)args[0].getType());
        }*/
        if (args[0].getType() == token::tokenType::Name)
            return st->insert(args[0].getName(), args[1].resolve(st));
        else
            return st->insert(static_cast<name>(args[0].resolve(st)->getConverted<std::string>()), args[1].resolve(st));
        throw std::runtime_error("Invalid use of : operator");
    });
    addOperatorL("function"_n, {
        //args[0].arity special meaning: index
        if (args[0].getType() != token::tokenType::CompoundStatement)
            throw std::runtime_error("wrong operand for function operator");
        return makeObject(compoundStatement::get(args[0].getCompoundStatementIndex()));
    });
    addOperatorL("json"_n, {
        if (args[0].getType() != token::tokenType::CompoundStatement)
            throw std::runtime_error("wrong operand for json operator");
        stack localJsonStack(st);
        compoundStatement::get(args[0].getCompoundStatementIndex())(localJsonStack);
        
        auto res = makeObject(nullptr);
        for (auto x : localJsonStack)
            res->addProperty(x.first, x.second);
        return res;
    });
    addOperatorL("return"_n, {
        throw args[0].resolve(st);
    });
    addOperatorL("throw"_n, {
        throw objectException(args[0].resolve(st));
    });
}