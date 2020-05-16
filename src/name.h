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

private:
    int code;
    static std::unordered_map<int, std::string> codeToNameMap;
    static std::unordered_map<std::string, int> nameToCodeMap;
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
