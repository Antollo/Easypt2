#include <fstream>
#include <algorithm>
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
        out.erase(std::remove(out.begin(), out.end(), '\r'), out.end());

        for (int i = 0; i < argc; i++)
        {
            if (isFlag(argv[i], "compare") && i != argc - 1)
            {
                std::ifstream f(argv[++i]);
                std::string str((std::istreambuf_iterator<char>(f)),
                                std::istreambuf_iterator<char>());
                str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
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
    catch (const objectException &e)
    {
        auto obj = e.getPtr();
        console::printStackTrace();
        if (obj->isConvertible<std::string>())
            console::error(obj->getConverted<object::type::String>());
        else
            console::error((std::string)e.what());
        runtime::fini(&globalStack);
        return 1;
    }
    catch (const std::exception &e)
    {
        console::printStackTrace();
        console::error((std::string)e.what());
        runtime::fini(&globalStack);
        return 1;
    }
    return 0;
}