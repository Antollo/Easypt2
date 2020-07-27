#include "core.h"
#include "console.h"
#include "runtime.h"
#include "defines.h"
#include "osDependent.h"
#include "treeParser.h"

inline bool isFlag(std::string a, std::string f)
{
    return a.size() >= 2 && (((a[0] == '-' || a[0] == '/') && a.find(f) == 1) || (a[0] == '-' && a[1] == '-' && a.find(f) == 2));
}

int main(int argc, char **argv)
{
    name::init();
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

        auto import = globalStack["import"_n];

        for (int i = 0; i < argc; i++)
            globalStack["argv"_n]->get<object::arrayType>()[i] = object::makeObject(std::string(argv[i]));

        for (int i = 0; i < argc; i++)
        {
            if (isFlag(argv[i], "file") && i != argc - 1)
            {
                (*import)(import, {object::makeObject(std::string(argv[++i]))}, &globalStack);
            }
            else if (isFlag(argv[i], "repl"))
            {
                (*import)(import, {object::makeObject("../library/repl.ez"s)}, &globalStack);
            }
            else if (isFlag(argv[i], "doc") && i != argc - 1)
            {
                auto help = (*(*import)(import, {object::makeObject("../library/docs.ez"s)}, &globalStack))["help"_n];
                (*help)(help, {
                    execute(globalStack["execute"_n],
                    { object::makeObject("return " + std::string(argv[++i]) + ";") },
                    &globalStack)
                }, &globalStack);
            }
            else if (isFlag(argv[i], "version"))
            {
                console::writeLine("Version from "s + __DATE__);
            }
            else if (isFlag(argv[i], "help"))
            {
                console::writeLine("Easypt interpreter\n\n"
                                   "Usage\n\n"
                                   "    easypt [options]\n\n"
                                   "Options\n\n"
                                   "    -file <file-path>   Import file.\n"
                                   "    -repl               Starts interactive mode.\n"
                                   "    -help               Print help.\n"
                                   "    -doc <expression>   Print help on specified object.\n"
                                   "    -version            Print version.\n\n"
                                   "More help at: "
                                   "https://github.com/Antollo/Easypt2");
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