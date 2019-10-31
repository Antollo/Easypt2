#include "utils.h"

int main()
{
    staticAssert("abc" == out([]() {
               console::write("abc");
           }));

    in("xyz", []() {
        std::string input;
        console::read(input);
        staticAssert(input == "xyz");
    });

    return 0;
}