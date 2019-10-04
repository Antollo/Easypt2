#include "statement.h"
#include "nobject.h"
#include "runtime.h"
#include "common.h"
#include "console.h"
#include "defines.h"
#include "core.h"

#pragma warning(disable : 4100)

void runtime::init(stack *st)
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
    addOperatorL("="_n, {
        object::objectPtr &a = args[0].resolve(st);
        *a = *args[1].resolve(st);
        a->setConst(false);
        return a;
    });
    addOperatorL("let"_n, {
        return insertObject(args[0].getName(), nullptr);
    });
    addOperatorL("function"_n, {
        //args[0].arity special meaning: index
        return makeObject(compoundStatement::get(args[0].getArity()));
    });
    addOperatorL("return"_n, {
        throw args[0].resolve(st);
    });

    object::objectPrototype = makeUndefined();

    object::arrayPrototype = makeUndefined();
    (*object::arrayPrototype)["prototype"_n] = object::objectPrototype;

    object::stringPrototype = makeUndefined();
    (*object::stringPrototype)["prototype"_n] = object::objectPrototype;

    object::numberPrototype = makeUndefined();
    (*object::numberPrototype)["prototype"_n] = object::objectPrototype;

    object::booleanPrototype = makeUndefined();
    (*object::booleanPrototype)["prototype"_n] = object::objectPrototype;

    object::functionPrototype = makeUndefined();
    (*object::functionPrototype)["prototype"_n] = object::objectPrototype;


    addFunctionL(object::arrayPrototype, "readOperator"_n, {
        argsConvertibleGuard<number>(args);
        return thisObj->get<const object::arrayType>().at(static_cast<int>(args[0]->getConverted<number>()));
    });

    addFunctionL(object::stringPrototype, "readOperator"_n, {
        argsConvertibleGuard<number>(args);
        return makeObject(std::string(1, thisObj->get<const std::string>().at(static_cast<int>(args[0]->getConverted<number>()))));
    });

    addFunctionL(object::objectPrototype, "readOperator"_n, {
        argsConvertibleGuard<std::string>(args);
        return (*thisObj)[static_cast<name>(args[0]->getConverted<std::string>())];
    });

    insertObject("true"_n, true);
    insertObject("false"_n, false);

    insertObject("import"_n, import);

    auto Array = insertObject("Array"_n, nullptr);

    addFunctionL(Array, "readOperator"_n, {
        return makeObject(args);
    });

    auto console = insertObject("console"_n, nullptr);

    addFunctionL(console, "write"_n, {
        for (auto &el : args)
            console::write(el->getConverted<std::string>());
        return makeUndefined();
    });
    addFunctionL(console, "writeLine"_n, {
        for (auto &el : args)
            console::write(el->getConverted<std::string>());
        console::newLine();
        return makeUndefined();
    });
    addFunctionL(console, "newLine"_n, {
        console::newLine();
        return makeUndefined();
    });
    addFunctionL(console, "read"_n, {
        std::string temp;
        console::read(temp);
        return makeObject(temp);
    });
    addFunctionL(console, "readLine"_n, {
        std::string temp;
        console::readLine(temp);
        return makeObject(temp);
    });
}

void runtime::fini(stack *st)
{
    st->clear();
}