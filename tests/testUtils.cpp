#include "utils.h"

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

    return 0;
}