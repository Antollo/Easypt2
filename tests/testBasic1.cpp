#include "utils.h"
#include "core.h"
#include "console.h"
#include "runtime.h"
#include "Node.h"
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
R"(a
b
3
3
1
ab
3.500000
1
11
2
7
function
true
true
s
10987654321
property
return function 2
return method 1
this property: property
return function 3
log from second file
return value from second file
12
Something not important.
1
false
[2]
1
3
0
56721
2
zz
122
122
mobile
)"s == console::getOutput([&argc, &argv]() {
                    name::init();
                    stack globalStack;
                    runtime::init(&globalStack);

                    auto st = &globalStack;
                    auto import = globalStack["import"_n];

                    for (int i = 0; i < argc; i++)
                    {
                        if (isFlag(argv[i], "file") && i != argc - 1)
                        {
                            (*import)(import, {object::makeObject(std::string(argv[++i]))}, st);
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