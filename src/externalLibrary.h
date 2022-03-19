#ifndef E7F3FEB9_1A76_4558_A240_2D432717131E
#define E7F3FEB9_1A76_4558_A240_2D432717131E

#include <regex>
#include <type_traits>
#include <cctype>
#include <iterator>
#include <utility>
#include "externalFunction.h"
#include "osDependent.h"

class externalLibrary
{
public:
    static object::objectPtr load(const std::filesystem::path &name)
    {
        static_assert(std::is_same_v<registerFunctionType, decltype(&registerFunction)>);
        static_assert(std::is_same_v<registerIntegerConstantType, decltype(&registerIntegerConstant)>);
        static_assert(std::is_same_v<registerDoubleConstantType, decltype(&registerDoubleConstant)>);

        auto library = osDependant::dynamicLibrary::loadLibrary(name.string());
        auto initialize = reinterpret_cast<initializeType>(osDependant::dynamicLibrary::getFunction(library, "initialize"));

        temp = object::makeEmptyObject();

        try
        {
            initialize(&registerFunction, &registerIntegerConstant, &registerDoubleConstant);
        }
        catch (...)
        {
            temp = nullptr;
            throw;
        }
        auto copy = std::move(temp);
        temp = nullptr;
        return copy;
    }

private:
    static std::pair<std::string, std::string> splitName(std::string name)
    {
        static std::regex camelCase("([a-z]+)([A-Z].*)");
        static std::regex pythonCase("([a-zA-Z]+)_(.*)");
        std::smatch m;

        std::regex_search(name, m, camelCase);
        if (m.size() == 3)
        {
            name[std::distance(m[0].first, m[2].first)] = std::tolower(*m[2].first);
            return {std::string(m[1].first, m[1].second), std::string(m[2].first, m[2].second)};
        }
        std::regex_search(name, m, pythonCase);
        if (m.size() == 3)
            return {std::string(m[1].first, m[1].second), std::string(m[2].first, m[2].second)};
        return {name, ""};
    }
    static void registerFunction(const char *str, void *pointer, FunctionTypeType type)
    {
        auto names = splitName(str);
        auto obj = object::makeObject(externalFunction(type, pointer));
        (*temp)[static_cast<name>(str)] = obj;
        (*(*temp)[static_cast<name>(names.first)])[static_cast<name>(names.second)] = obj;
    }
    static void registerIntegerConstant(const char *str, int64_t value)
    {
        auto names = splitName(str);
        auto obj = object::makeObject(static_cast<number>(value));
        (*temp)[static_cast<name>(str)] = obj;
        (*(*temp)[static_cast<name>(names.first)])[static_cast<name>(names.second)] = obj;
    }
    static void registerDoubleConstant(const char *str, double value)
    {
        auto names = splitName(str);
        auto obj = object::makeObject(static_cast<number>(value));
        (*temp)[static_cast<name>(str)] = obj;
        (*(*temp)[static_cast<name>(names.first)])[static_cast<name>(names.second)] = obj;
    }

    static inline object::objectPtr temp;
};

#endif /* E7F3FEB9_1A76_4558_A240_2D432717131E */
