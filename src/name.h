#ifndef NAME_H
#define NAME_H

#include <unordered_map>
#include <utility>
#include <string>
#include <stdexcept>
#include <cstdint>

using namespace std::string_literals;

class name
{
public:
    using codeType = uint_fast16_t;
    name() : code(emptyCode) {}
    explicit name(std::string &&textName);
    explicit name(const std::string &textName);

    inline name(const name &n) = default;
    inline name(name &&n) = default;

    inline name &operator=(const name &n) = default;
    inline name &operator=(name &&) = default;

    explicit operator std::string() const { return codeToNameMap[code]; }
    explicit operator name::codeType() const { return code; }

    bool operator==(const name &n) const { return code == n.code; }
    bool operator==(name &&n) const { return code == n.code; }

    bool isEmpty() const { return code == emptyCode; }

    const static name empty;
    const static name prototype;
    const static name args;
    const static name thisObj;

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
    codeType code;
    name(codeType i) : code(i) {}
    static std::unordered_map<codeType, std::string> codeToNameMap;
    static std::unordered_map<std::string, codeType> nameToCodeMap;
    static constexpr codeType start = 10;
    static constexpr codeType emptyCode = 0;
    static constexpr codeType prototypeCode = 1;
    static constexpr codeType argsCode = 2;
    static constexpr codeType thisObjCode = 3;
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
            return hash<name::codeType>()(x.code);
        }
    };
} // namespace std

#endif // NAME_H
