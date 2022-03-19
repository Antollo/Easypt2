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
    bool operator<(const name &n) const { return code < n.code; }
    bool operator<(name &&n) const { return code < n.code; }

    bool isEmpty() const { return code == emptyCode; }

    static void init()
    {
        codeToNameMap[prototypeCode] = "prototype"s;
        codeToNameMap[argsCode] = "args"s;
        codeToNameMap[thisObjCode] = "this"s;
        codeToNameMap[readOperatorCode] = "readOperator"s;
        codeToNameMap[classPrototypeCode] = "classPrototype"s;
        codeToNameMap[ClassCode] = "Class"s;
        codeToNameMap[constructorCode] = "constructor"s;
        codeToNameMap[superCode] = "super"s;
        codeToNameMap[destructorCode] = "destructor"s;
        codeToNameMap[exceptionCode] = "exception"s;
        codeToNameMap[awaitCode] = "await"s;
        codeToNameMap[__stackCode] = "__stack"s;
        codeToNameMap[__parentCode] = "__parent"s;
        codeToNameMap[setCode] = "set"s;
        codeToNameMap[getCode] = "get"s;
        codeToNameMap[exportsCode] = "exports"s;
        codeToNameMap[moduleCode] = "module"s;
        codeToNameMap[nameCode] = "name"s;
        codeToNameMap[filenameCode] = "filename"s;
        codeToNameMap[pathCode] = "path"s;
        codeToNameMap[argvCode] = "argv"s;
        codeToNameMap[argcCode] = "argc"s;
        codeToNameMap[importCode] = "import"s;
        codeToNameMap[executeCode] = "execute"s;
        codeToNameMap[helpCode] = "help"s;
        codeToNameMap[additionCode] = "+"s;
        codeToNameMap[multiplicationCode] = "*"s;
        codeToNameMap[subtractionCode] = "-"s;
        codeToNameMap[divisionCode] = "/"s;
        codeToNameMap[modulusCode] = "%"s;
        codeToNameMap[andCode] = "&&"s;
        codeToNameMap[orCode] = "||"s;
        codeToNameMap[bitwiseAndCode] = "&"s;
        codeToNameMap[bitwiseOrCode] = "|"s;
        codeToNameMap[shiftLeftCode] = "<<"s;
        codeToNameMap[shiftRightCode] = ">>"s;
        codeToNameMap[equalCode] = "=="s;
        codeToNameMap[notEqualCode] = "!="s;
        codeToNameMap[lessCode] = "<"s;
        codeToNameMap[greaterCode] = ">"s;
        codeToNameMap[lessEqualCode] = "<="s;
        codeToNameMap[greaterEqualCode] = ">="s;
        codeToNameMap[incrementCode] = "++"s;
        codeToNameMap[decrementCode] = "--"s;
        codeToNameMap[notCode] = "!"s;
        codeToNameMap[complementCode] = "~"s;
        codeToNameMap[toNumberCode] = "toNumber"s;
        codeToNameMap[toStringCode] = "toString"s;
        codeToNameMap[toArrayCode] = "toArray"s;
        codeToNameMap[toBooleanCode] = "toBoolean"s;
        codeToNameMap[BufferCode] = "Buffer"s;
        codeToNameMap[__clientCode] = "__client"s;
        
        nameToCodeMap["prototype"s] = prototypeCode;
        nameToCodeMap["args"s] = argsCode;
        nameToCodeMap["this"s] = thisObjCode;
        nameToCodeMap["readOperator"s] = readOperatorCode;
        nameToCodeMap["classPrototype"s] = classPrototypeCode;
        nameToCodeMap["Class"s] = ClassCode;
        nameToCodeMap["constructor"s] = constructorCode;
        nameToCodeMap["super"s] = superCode;
        nameToCodeMap["destructor"s] = destructorCode;
        nameToCodeMap["exception"s] = exceptionCode;
        nameToCodeMap["await"s] = awaitCode;
        nameToCodeMap["__stack"s] = __stackCode;
        nameToCodeMap["__parent"s] = __parentCode;
        nameToCodeMap["set"s] = setCode;
        nameToCodeMap["get"s] = getCode;
        nameToCodeMap["exports"s] = exportsCode;
        nameToCodeMap["module"s] = moduleCode;
        nameToCodeMap["name"s] = nameCode;
        nameToCodeMap["filename"s] = filenameCode;
        nameToCodeMap["path"s] = pathCode;
        nameToCodeMap["argv"s] = argvCode;
        nameToCodeMap["argc"s] = argcCode;
        nameToCodeMap["import"s] = importCode;
        nameToCodeMap["execute"s] = executeCode;
        nameToCodeMap["help"s] = helpCode;
        nameToCodeMap["+"s] = additionCode;
        nameToCodeMap["*"s] = multiplicationCode;
        nameToCodeMap["-"s] = subtractionCode;
        nameToCodeMap["/"s] = divisionCode;
        nameToCodeMap["%"s] = modulusCode;
        nameToCodeMap["&&"s] = andCode;
        nameToCodeMap["||"s] = orCode;
        nameToCodeMap["&"s] = bitwiseAndCode;
        nameToCodeMap["|"s] = bitwiseOrCode;
        nameToCodeMap["<<"s] = shiftLeftCode;
        nameToCodeMap[">>"s] = shiftRightCode;
        nameToCodeMap["=="s] = equalCode;
        nameToCodeMap["!="s] = notEqualCode;
        nameToCodeMap["<"s] = lessCode;
        nameToCodeMap[">"s] = greaterCode;
        nameToCodeMap["<="s] = lessEqualCode;
        nameToCodeMap[">="s] = greaterEqualCode;
        nameToCodeMap["++"s] = incrementCode;
        nameToCodeMap["--"s] = decrementCode;
        nameToCodeMap["!"s] = notCode;
        nameToCodeMap["~"s] = complementCode;
        nameToCodeMap["toNumber"s] = toNumberCode;
        nameToCodeMap["toString"s] = toStringCode;
        nameToCodeMap["toArray"s] = toArrayCode;
        nameToCodeMap["toBoolean"s] = toBooleanCode;
        nameToCodeMap["Buffer"s] = BufferCode;
        nameToCodeMap["__client"s] = __clientCode;
    }

private:
    static constexpr codeType start = 50;
    static constexpr codeType emptyCode = 0;
    static constexpr codeType reservedCode = 1;
    static constexpr codeType prototypeCode = 2;
    static constexpr codeType argsCode = 3;
    static constexpr codeType thisObjCode = 4;
    static constexpr codeType readOperatorCode = 5;
    static constexpr codeType classPrototypeCode = 6;
    static constexpr codeType ClassCode = 7;
    static constexpr codeType constructorCode = 8;
    static constexpr codeType superCode = 9;
    static constexpr codeType destructorCode = 10;
    static constexpr codeType exceptionCode = 11;
    static constexpr codeType awaitCode = 12;
    static constexpr codeType __stackCode = 13;
    static constexpr codeType __parentCode = 14;
    static constexpr codeType setCode = 15;
    static constexpr codeType getCode = 16;
    static constexpr codeType exportsCode = 17;
    static constexpr codeType moduleCode = 18;
    static constexpr codeType nameCode = 19;
    static constexpr codeType filenameCode = 20;
    static constexpr codeType pathCode = 21;
    static constexpr codeType argvCode = 22;
    static constexpr codeType argcCode = 23;
    static constexpr codeType importCode = 24;
    static constexpr codeType executeCode = 25;
    static constexpr codeType helpCode = 26;
    static constexpr codeType additionCode = 27;
    static constexpr codeType multiplicationCode = 28;
    static constexpr codeType subtractionCode = 29;
    static constexpr codeType divisionCode = 30;
    static constexpr codeType modulusCode = 31;
    static constexpr codeType andCode = 32;
    static constexpr codeType orCode = 33;
    static constexpr codeType bitwiseAndCode = 34;
    static constexpr codeType bitwiseOrCode = 35;
    static constexpr codeType shiftLeftCode = 36;
    static constexpr codeType shiftRightCode = 37;
    static constexpr codeType equalCode = 38;
    static constexpr codeType notEqualCode = 39;
    static constexpr codeType lessCode = 40;
    static constexpr codeType greaterCode = 41;
    static constexpr codeType lessEqualCode = 42;
    static constexpr codeType greaterEqualCode = 43;
    static constexpr codeType incrementCode = 44;
    static constexpr codeType decrementCode = 45;
    static constexpr codeType notCode = 46;
    static constexpr codeType complementCode = 47;
    static constexpr codeType toNumberCode = 48;
    static constexpr codeType toStringCode = 49;
    static constexpr codeType toArrayCode = 50;
    static constexpr codeType toBooleanCode = 51;
    static constexpr codeType BufferCode = 52;
    static constexpr codeType __clientCode = 53;

    codeType code;
    constexpr name(codeType i) : code(i) {}
    static std::unordered_map<codeType, std::string> codeToNameMap;
    static std::unordered_map<std::string, codeType> nameToCodeMap;
    friend struct std::hash<name>;
    friend struct n;
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

struct n
{
    static constexpr name empty = name::emptyCode;
    static constexpr name reserved = name::reservedCode;
    static constexpr name prototype = name::prototypeCode;
    static constexpr name args = name::argsCode;
    static constexpr name thisObj = name::thisObjCode;
    static constexpr name readOperator = name::readOperatorCode;
    static constexpr name classPrototype = name::classPrototypeCode;
    static constexpr name Class = name::ClassCode;
    static constexpr name constructor = name::constructorCode;
    static constexpr name super = name::superCode;
    static constexpr name destructor = name::destructorCode;
    static constexpr name exception = name::exceptionCode;
    static constexpr name await_ = name::awaitCode;
    static constexpr name __stack = name::__stackCode;
    static constexpr name __parent = name::__parentCode;
    static constexpr name set = name::setCode;
    static constexpr name get = name::getCode;
    static constexpr name exports = name::exportsCode;
    static constexpr name module = name::moduleCode;
    static constexpr name name_ = name::nameCode;
    static constexpr name filename = name::filenameCode;
    static constexpr name path = name::pathCode;
    static constexpr name argv = name::argvCode;
    static constexpr name argc = name::argcCode;
    static constexpr name import = name::importCode;
    static constexpr name execute = name::executeCode;
    static constexpr name help = name::helpCode;
    static constexpr name addition = name::additionCode;
    static constexpr name multiplication = name::multiplicationCode;
    static constexpr name subtraction = name::subtractionCode;
    static constexpr name division = name::divisionCode;
    static constexpr name modulus = name::modulusCode;
    static constexpr name andOp = name::andCode;
    static constexpr name orOp = name::orCode;
    static constexpr name bitwiseAnd = name::bitwiseAndCode;
    static constexpr name bitwiseOr = name::bitwiseOrCode;
    static constexpr name shiftLeft = name::shiftLeftCode;
    static constexpr name shiftRight = name::shiftRightCode;
    static constexpr name equal = name::equalCode;
    static constexpr name notEqual = name::notEqualCode;
    static constexpr name less = name::lessCode;
    static constexpr name greater = name::greaterCode;
    static constexpr name lessEqual = name::lessEqualCode;
    static constexpr name greaterEqual = name::greaterEqualCode;
    static constexpr name increment = name::incrementCode;
    static constexpr name decrement = name::decrementCode;
    static constexpr name notOp = name::notCode;
    static constexpr name complement = name::complementCode;
    static constexpr name toNumber = name::toNumberCode;
    static constexpr name toString = name::toStringCode;
    static constexpr name toArray = name::toArrayCode;
    static constexpr name toBoolean = name::toBooleanCode;
    static constexpr name Buffer = name::BufferCode;
    static constexpr name __client = name::__clientCode;

};

#endif // NAME_H
