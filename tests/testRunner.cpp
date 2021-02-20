#include <fstream>
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
    stack globalStack;
    try
    {
        auto out = console::getOutput([&argc, &argv, &globalStack]() {
            name::init();
            runtime::init(&globalStack);

            auto st = &globalStack;
            auto import = globalStack[n::import];

            for (int i = 0; i < argc; i++)
            {
                if (isFlag(argv[i], "file") && i != argc - 1)
                {
                    (*import)(import, {object::makeObject(std::string(argv[++i]))}, st);
                }
            }

            runtime::fini(&globalStack);
        });

        for (int i = 0; i < argc; i++)
        {
            if (isFlag(argv[i], "compare") && i != argc - 1)
            {
                std::ifstream f(argv[++i]);
                std::string str((std::istreambuf_iterator<char>(f)),
                                std::istreambuf_iterator<char>());
                if (str == out)
                {
                    console::write("ok");
                    return 0;
                }
                else
                {
                    console::error(out);
                    return 1;
                }
            }
        }
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
        return 1;
    }
    catch (std::exception &e)
    {
        console::stackTrace();
        console::error((std::string)e.what());
        runtime::fini(&globalStack);
        return 1;
    }
    return 0;
}