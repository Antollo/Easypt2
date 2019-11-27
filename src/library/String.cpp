#include "../library.h"

void String::init(stack *st)
{
    object::objectPtr String = insertObject("String"_n, constructorCaller);

    addFunctionL(String, "readOperator"_n, {
        return makeObject(args);
    });

    (*String)["prototype"_n] = object::stringPrototype;

    addFunctionL(object::stringPrototype, "constructor"_n, {
        thisObj->setType<std::string>();
        return thisObj;
    });

    addFunctionL(object::stringPrototype, "readOperator"_n, {
        argsConvertibleGuard<number>(args);
        int pos = static_cast<int>(args[0]->getConverted<number>());
        const std::string& me = thisObj->get<const std::string>();
        assert<std::greater_equal>(pos, 0);
        assert<std::less>(pos, me.size());
        return makeObject(std::string(1, me[pos]));
    });

    addFunctionL(object::stringPrototype, "getByteAt"_n, {
        argsConvertibleGuard<number>(args);
        int pos = static_cast<int>(args[0]->getConverted<number>());
        const std::string& me = thisObj->get<const std::string>();
        assert<std::greater_equal>(pos, 0);
        assert<std::less>(pos, me.size());
        return makeObject(number(static_cast<int>(static_cast<unsigned char>(me[pos]))));
    });

    addFunctionL(object::stringPrototype, "setByteAt"_n, {
        argsConvertibleGuard<number, number>(args);
        int pos = static_cast<int>(args[0]->getConverted<number>());
        int byte = static_cast<int>(args[0]->getConverted<number>());
        std::string& me = thisObj->get<std::string>();
        assert<std::greater_equal>(pos, 0);
        assert<std::less>(pos, me.size());
        me[pos] = static_cast<unsigned char>(byte);
        return makeUndefined();
    });

    addFunctionL(object::stringPrototype, "length"_n, {
        return makeObject(static_cast<number>(thisObj->get<const std::string>().size()));
    });

    addFunctionL(object::stringPrototype, "insertFrom"_n, {
        argsConvertibleGuard<number, std::string, number, number>(args);
        argsGuard<nullptr_t, std::string>(args);
        std::string &dest = thisObj->get<std::string>();
        const std::string &src = args[1]->get<const std::string>();
        int destPos = static_cast<int>(args[0]->getConverted<number>());
        int srcPos = static_cast<int>(args[2]->getConverted<number>());
        int srcLength = static_cast<int>(args[3]->getConverted<number>());
        assert<std::greater_equal>(destPos, 0);
        assert<std::less_equal>(destPos, dest.size());
        assert<std::greater_equal>(srcPos, 0);
        assert<std::greater_equal>(srcLength, 0);
        assert<std::less_equal>(srcPos + srcLength, src.size());
        dest.insert(dest.begin() + destPos, src.begin() + srcPos, src.begin() + srcPos + srcLength);
        return thisObj;
    });

    addFunctionL(object::stringPrototype, "substring"_n, {
        argsConvertibleGuard<number, number>(args);
        const std::string &me = thisObj->get<const std::string>();
        int pos = static_cast<int>(args[0]->getConverted<number>());
        int length = static_cast<int>(args[1]->getConverted<number>());
        assert<std::greater_equal>(pos, 0);
        assert<std::greater_equal>(length, 0);
        assert<std::less_equal>(pos + length, me.size());
        return makeObject(std::string(me.begin() + pos, me.begin() + pos + length));
    });

    addFunctionL(object::stringPrototype, "erase"_n, {
        argsConvertibleGuard<number, number>(args);
        std::string &me = thisObj->get<std::string>();
        int pos = static_cast<int>(args[0]->getConverted<number>());
        int length = static_cast<int>(args[1]->getConverted<number>());
        assert<std::greater_equal>(pos, 0);
        assert<std::greater_equal>(length, 0);
        assert<std::less_equal>(pos + length, me.size());
        me.erase(me.begin() + pos, me.begin() + pos + length);
        return thisObj;
    });
}