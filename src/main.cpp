#include "core.h"
#include "console.h"
#include "runtime.h"
#include "defines.h"
#include "osDependent.h"

#include "treeParser.h"

inline bool isFlag(std::string a, std::string f)
{
    return (a[0] == '-' || a[0] == '/') && a.find(f) == 1;
}

int main(int argc, char **argv)
{
    stack globalStack;
    try
    {
        initialize();
        runtime::init(&globalStack);

        auto st = &globalStack;
        insertObject("argv"_n, object::arrayType());
        insertObject("argc"_n, 0_n);

        globalStack["argv"_n]->get<object::arrayType>().resize(argc);
        globalStack["argc"_n]->get<number>() = argc;

        for (int i = 0; i < argc; i++)
            globalStack["argv"_n]->get<object::arrayType>()[i] = makeObject(std::string(argv[i]));

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
        }
        runtime::fini(&globalStack);
    }
    catch (objectException &e)
    {
        auto obj = e.getPtr();
        console::stackTrace();
        if (obj->isConvertible<std::string>())
            console::error(obj->getConverted<std::string>());
        else
            console::error((std::string)e.what());
        runtime::fini(&globalStack);
    }
    catch (std::exception &e)
    {
        console::stackTrace();
        console::error((std::string)e.what());
        runtime::fini(&globalStack);
    }
    return 0;
}