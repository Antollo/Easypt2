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
            else if (isFlag(argv[i], "repl"))
            {
                import(nullptr, {makeObject(std::string("repl"))}, &globalStack);
            }
            else if (isFlag(argv[i], "help"))
            {
                console::writeLine("See project's repository (there are tutorial and language reference): https://github.com/Antollo/Easypt2"s);
            }
            else
            {
                globalStack["argv"_n]->get<object::arrayType>().push_back(makeObject(std::string(argv[i])));
                globalStack["argc"_n]->get<number>()++;
            }
        }
    }
    catch (const std::exception &e)
    {
        console::error((std::string)e.what());
    }
    return 0;
}