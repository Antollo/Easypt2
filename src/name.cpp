#include "name.h"

std::unordered_map<int, std::string> name::codeToNameMap;
std::unordered_map<std::string, int> name::nameToCodeMap;

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
        code = nameToCodeMap.size();
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
        code = nameToCodeMap.size();
        nameToCodeMap[textName] = code;
        codeToNameMap[code] = textName;
    }
    else
    {
        code = nameToCodeMap[textName];
    }
}

name::operator std::string() const
{
    return codeToNameMap[code];
}
name::operator int() const
{
    return code;
}
bool name::operator==(const name &x) const
{
    return code == x.code;
}