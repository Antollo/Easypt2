#include "utils.h"
#include "core.h"
#include "console.h"
#include "runtime.h"
#include "statement.h"
#include "defines.h"

inline bool isFlag(std::string a, std::string f)
{
    return (a[0] == '-' || a[0] == '/') && a.find(f) == 1;
}

int main(int argc, char **argv)
{
    try
    {
        staticAssert(
R"(x
a is good
b is not good
constructor
12
_g
_g
_g
a
0 1
2 1
4 1
destructor_g
)"s == console::getOutput([&argc, &argv]() {
                   stack globalStack;
                   runtime::init(&globalStack);

                   auto st = &globalStack;

                   for (int i = 0; i < argc; i++)
                   {
                       if (isFlag(argv[i], "file") && i != argc - 1)
                       {
                           import(nullptr, {makeObject(std::string(argv[++i]))}, st);
                       }
                   }
               }));
    }
    catch (const std::exception &e)
    {
        console::error((std::string)e.what());
        return 1;
    }
    return 0;
}