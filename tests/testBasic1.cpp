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
        assert("a\nb\n3\n3\n1\nab\n3.500000\n1\n11\n2\n7\nfunction\ntrue\ntrue\ns\n10987654321\nproperty" == out([&argc, &argv]() {
                   stack globalStack;
                   runtime::init(&globalStack);

                   auto st = &globalStack;
                   insertObject("argv"_n, object::arrayType());
                   insertObject("argc"_n, 0_n);

                   for (int i = 0; i < argc; i++)
                   {
                       if (isFlag(argv[i], "file") && i != argc - 1)
                       {
                           import(nullptr, {makeObject(std::string(argv[++i]))}, &globalStack);
                       }
                       else
                       {
                           globalStack["argv"_n]->get<object::arrayType>().push_back(makeObject(std::string(argv[i])));
                           globalStack["argc"_n]->get<number>()++;
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