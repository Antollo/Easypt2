#include "name.h"

std::unordered_map<name::codeType, std::string> name::codeToNameMap;
std::unordered_map<std::string, name::codeType> name::nameToCodeMap;

name operator"" _n(const char *str, size_t c)
{
    return name(std::string(str, c));
}

name::name(std::string &&textName)
{
    if (textName.size() == 0)
        throw std::runtime_error("terrible parser error");
    if (nameToCodeMap.count(textName) == 0)
    {
        code = nameToCodeMap.size() + start;
        nameToCodeMap[textName] = code;
        codeToNameMap[code] = textName;
    }
    else
    {
        code = nameToCodeMap[textName];
    }
}

name::name(const std::string &textName)
{
    if (textName.size() == 0)
        throw std::runtime_error("terrible parser error");
    if (nameToCodeMap.count(textName) == 0)
    {
        code = nameToCodeMap.size() + start;
        nameToCodeMap[textName] = code;
        codeToNameMap[code] = textName;
    }
    else
    {
        code = nameToCodeMap[textName];
    }
}