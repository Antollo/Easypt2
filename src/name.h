#ifndef NAME_H
#define NAME_H

#include <unordered_map>
#include <utility>
#include <string>
#include <stdexcept>

using namespace std::string_literals;

class name
{
public:
    name() : code(0) {}
    explicit name(std::string &&textName);
    explicit name(const std::string &textName);
    explicit operator std::string() const;
    explicit operator int() const;
    bool operator==(const name &x) const;
    bool isEmpty() const { return code == 0; }

    const static name prototype, args, thisObj;

    static void init()
    {
        codeToNameMap[prototypeCode] = "prototype"s;
        nameToCodeMap["prototype"s] = prototypeCode;

        codeToNameMap[argsCode] = "args"s;
        nameToCodeMap["args"s] = argsCode;

        codeToNameMap[thisObjCode] = "this"s;
        nameToCodeMap["this"s] = thisObjCode;
    }

private:
    unsigned int code;
    name(unsigned int i) : code(i) {}
    static std::unordered_map<unsigned int, std::string> codeToNameMap;
    static std::unordered_map<std::string, unsigned int> nameToCodeMap;
    static constexpr unsigned int start = 10;
    static constexpr unsigned int prototypeCode = 1;
    static constexpr unsigned int argsCode = 2;
    static constexpr unsigned int thisObjCode = 3;
    friend struct std::hash<name>;
};

name operator"" _n(const char *str, size_t c);

namespace std
{
template <>
struct hash<name>
{
    size_t operator()(const name &x) const
    {
        return hash<int>()(x.code);
    }
};
} // namespace std

#endif // NAME_H
