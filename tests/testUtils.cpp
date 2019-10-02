#include "utils.h"

int main()
{
    assert("abc" == out([]() {
               console::write("abc");
           }));

    in("xyz", []() {
        std::string input;
        console::read(input);
        assert(input == "xyz");
    });

    return 0;
}