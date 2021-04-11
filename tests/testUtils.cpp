#include "utils.h"
#include <variant>
#include <string>

int main()
{
    staticAssert("abc" == console::getOutput([]() {
                     console::write("abc");
                 }));

    console::setInput("xyz", []() {
        std::string input;
        console::read(input);
        staticAssert(input == "xyz");
    });

    std::variant<int, float, char, std::string> variant;
    void *v = reinterpret_cast<void *>(&variant);

    void *a = reinterpret_cast<void *>(&std::get<int>(variant));
    staticAssert(a == v);

    variant = 1.f;
    void *b = reinterpret_cast<void *>(&std::get<float>(variant));
    staticAssert(b == v);

    variant = 'a';
    void *c = reinterpret_cast<void *>(&std::get<char>(variant));
    staticAssert(c == v);

    variant = "abc";
    void *d = reinterpret_cast<void *>(&std::get<std::string>(variant));
    staticAssert(d == v);

    return 0;
}