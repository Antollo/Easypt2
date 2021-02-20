#define _REGEX_MAX_STACK_COUNT 100000
#include <regex>
#include <iterator>
#include "../library.h"

void String::init(stack *st)
{
    object::objectPtr String = insertObject("String"_n, constructorCaller);

    (*String)[n::classPrototype] = object::stringPrototype;

    addFunctionL(object::stringPrototype, n::constructor, {
        thisObj->setType<std::string>();
        return thisObj;
    });

    addFunctionL(object::stringPrototype, n::readOperator, {
        argsConvertibleGuard<number>(args);
        int pos = static_cast<int>(args[0]->getConverted<number>());
        const std::string &me = thisObj->get<const std::string>();
        assert<std::greater_equal>(pos, 0);
        assert<std::less>(pos, me.size());
        return object::makeObject(std::string(1, me[pos]));
    });

    addFunctionL(object::stringPrototype, "getByteAt"_n, {
        argsConvertibleGuard<number>(args);
        int pos = static_cast<int>(args[0]->getConverted<number>());
        const std::string &me = thisObj->get<const std::string>();
        assert<std::greater_equal>(pos, 0);
        assert<std::less>(pos, me.size());
        return object::makeObject(number(static_cast<int>(static_cast<unsigned char>(me[pos]))));
    });

    addFunctionL(object::stringPrototype, "setByteAt"_n, {
        argsConvertibleGuard<number, number>(args);
        int pos = static_cast<int>(args[0]->getConverted<number>());
        int byte = static_cast<int>(args[1]->getConverted<number>());
        std::string &me = thisObj->get<std::string>();
        assert<std::greater_equal>(pos, 0);
        assert<std::less>(pos, me.size());
        me[pos] = static_cast<unsigned char>(byte);
        return thisObj;
    });

    addFunctionL(object::stringPrototype, "length"_n, {
        return object::makeObject(static_cast<number>(thisObj->get<const std::string>().size()));
    });

    addFunctionL(object::stringPrototype, "insertFrom"_n, {
        argsConvertibleGuard<number, std::string>(args);
        std::string &dest = thisObj->get<std::string>();
        const std::string &src = args[1]->get<const std::string>();
        int destPos = static_cast<int>(args[0]->getConverted<number>());
        int srcPos = 0;
        int srcLength = src.size();
        if (args.size() > 2)
        {
            argsConvertibleGuard<nullptr_t, nullptr_t, number>(args);
            srcPos = static_cast<int>(args[2]->getConverted<number>());
            srcLength = srcLength - srcPos;
            if (args.size() > 3)
            {
                argsConvertibleGuard<nullptr_t, nullptr_t, number, number>(args);
                srcLength = std::min(srcLength, static_cast<int>(args[3]->getConverted<number>()));
            }
        }
        assert<std::greater_equal>(destPos, 0);
        assert<std::less_equal>(destPos, dest.size());
        assert<std::greater_equal>(srcPos, 0);
        assert<std::greater_equal>(srcLength, 0);
        assert<std::less_equal>(srcPos + srcLength, src.size());
        dest.insert(dest.begin() + destPos, src.begin() + srcPos, src.begin() + srcPos + srcLength);
        return thisObj;
    });

    addFunctionL(object::stringPrototype, "append"_n, {
        argsConvertibleGuard<std::string>(args);
        std::string &dest = thisObj->get<std::string>();
        const std::string &src = args[0]->get<const std::string>();
        int srcPos = 0;
        int srcLength = src.size();
        if (args.size() > 1)
        {
            argsConvertibleGuard<nullptr_t, number>(args);
            srcPos = static_cast<int>(args[1]->getConverted<number>());
            srcLength = srcLength - srcPos;
            if (args.size() > 2)
            {
                argsConvertibleGuard<nullptr_t, number, number>(args);
                srcLength = std::min(srcLength, static_cast<int>(args[2]->getConverted<number>()));
            }
        }
        assert<std::greater_equal>(srcPos, 0);
        assert<std::greater_equal>(srcLength, 0);
        assert<std::less_equal>(srcPos + srcLength, src.size());
        dest.insert(dest.end(), src.begin() + srcPos, src.begin() + srcPos + srcLength);
        return thisObj;
    });

    addFunctionL(object::stringPrototype, "substring"_n, {
        argsConvertibleGuard<number>(args);
        const std::string &me = thisObj->get<const std::string>();
        int pos = static_cast<int>(args[0]->getConverted<number>());
        int length = me.size() - pos;
        if (args.size() > 1)
        {
            argsConvertibleGuard<nullptr_t, number>(args);
            length = std::min(length, static_cast<int>(args[1]->getConverted<number>()));
        }
        assert<std::greater_equal>(pos, 0);
        assert<std::greater_equal>(length, 0);
        assert<std::less_equal>(pos + length, me.size());
        return object::makeObject(std::string(me.begin() + pos, me.begin() + pos + length));
    });

    addFunctionL(object::stringPrototype, "erase"_n, {
        argsConvertibleGuard<number>(args);
        std::string &me = thisObj->get<std::string>();
        int pos = static_cast<int>(args[0]->getConverted<number>());
        int length = me.size() - pos;
        if (args.size() > 1)
        {
            argsConvertibleGuard<nullptr_t, number>(args);
            length = std::min(length, static_cast<int>(args[1]->getConverted<number>()));
        }
        assert<std::greater_equal>(pos, 0);
        assert<std::greater_equal>(length, 0);
        assert<std::less_equal>(pos + length, me.size());
        me.erase(me.begin() + pos, me.begin() + pos + length);
        return thisObj;
    });

    addFunctionL(object::stringPrototype, "split"_n, {
        argsConvertibleGuard<std::string>(args);
        const std::string &me = thisObj->get<const std::string>();
        const std::string &delim = args[0]->getConverted<const std::string>();
        int begin = 0, end, delimSize = delim.size();

        object::type::Array res;

        while ((end = me.find (delim, begin)) != std::string::npos) {
            res.push_back(object::makeObject(me.substr(begin, end - begin)));
            begin = end + delimSize;
        }
        res.push_back(object::makeObject(me.substr(begin)));

        return object::makeObject(res);
    });

    addFunctionL(object::stringPrototype, "indexOf"_n, {
        argsConvertibleGuard<std::string>(args);
        std::string &me = thisObj->get<std::string>();
        std::string key = args[0]->getConverted<std::string>();
        int pos = 0;
        if (args.size() >= 2)
            pos = static_cast<int>(args[1]->getConverted<number>());
        assert<std::greater_equal>(pos, 0);
        assert<std::less_equal>(pos, me.size());
        return object::makeObject(static_cast<number>(me.find(key, pos)));
    });

    addFunctionL(object::stringPrototype, "search"_n, {
        argsConvertibleGuard<std::string>(args);
        const std::string &me = thisObj->get<const std::string>();
        std::regex regex(args[0]->getConverted<const std::string>());
        std::smatch matchResults;
        int pos = 0;
        int length = me.size();

        if (args.size() >= 2)
            pos = static_cast<int>(args[1]->getConverted<number>());
        if (args.size() >= 3)
            length = static_cast<int>(args[2]->getConverted<number>());

        std::regex_search(me.begin() + pos, me.begin() + pos + length, matchResults, regex);
        object::type::Array array(matchResults.size());
        for (size_t i = 0; i < matchResults.size(); i++)
        {
            array[i] = object::makeEmptyObject();
            array[i]->addProperty("position"_n, object::makeObject(static_cast<number>(static_cast<int>(std::distance(me.begin(), matchResults[i].first)))));
            array[i]->addProperty("length"_n, object::makeObject(static_cast<number>(static_cast<int>(std::distance(matchResults[i].first, matchResults[i].second)))));
        }
        return object::makeObject(array);
    });

    addFunctionL(object::stringPrototype, "searchAll"_n, {
        argsConvertibleGuard<std::string>(args);
        const std::string &me = thisObj->get<const std::string>();
        std::regex regex(args[0]->getConverted<const std::string>());
        int pos = 0;
        int length = me.size();

        if (args.size() >= 2)
            pos = static_cast<int>(args[1]->getConverted<number>());
        if (args.size() >= 3)
            length = static_cast<int>(args[2]->getConverted<number>());

        auto begin = std::sregex_iterator(me.begin() + pos, me.begin() + pos + length, regex);
        auto end = std::sregex_iterator();

        object::type::Array array(std::distance(begin, end));

        size_t i = 0;
        for (auto it = begin; it != end; it++)
        {
            array[i] = object::makeEmptyObject();
            array[i]->addProperty("position"_n, object::makeObject(static_cast<number>(static_cast<int>(it->position()))));
            array[i]->addProperty("length"_n, object::makeObject(static_cast<number>(static_cast<int>(it->length()))));
            i++;
        }

        return object::makeObject(array);
    });
}