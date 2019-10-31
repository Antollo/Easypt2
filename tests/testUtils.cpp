#include "utils.h"

int main()
{
    assert("abc" == console::getOutput([]() {
               console::write("abc");
           }));

    console::setInput("xyz", []() {
        std::string input;
        console::read(input);
        assert(input == "xyz");
    });

    return 0;
}