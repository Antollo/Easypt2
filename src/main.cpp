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

        //auto opengl = dynamicLibrary::loadLibrary("./opengl.so");
        //auto fun = reinterpret_cast<void(*)()>(dynamicLibrary::getFunction(opengl, "fun"));
        //fun();

        auto st = &globalStack;
        insertObject(n::argv, object::type::Array());
        insertObject(n::argc, 0_n);

        globalStack[n::argv]->get<object::type::Array>().resize(argc);
        globalStack[n::argc]->get<number>() = argc;

        for (int i = 0; i < argc; i++)
            globalStack[n::argv]->get<object::type::Array>()[i] = object::makeObject(std::string(argv[i]));

        for (int i = 0; i < argc; i++)
        {
            auto import = globalStack[n::import];
            auto execute = globalStack[n::execute];

            if (isFlag(argv[i], "file") && i != argc - 1)
            {
                (*import)(import, {object::makeObject(std::string(argv[++i]))}, &globalStack);
            }
            else if (isFlag(argv[i], "repl"))
            {
                (*import)(import, {object::makeObject("../library/repl.ez"s)}, &globalStack);
            }
            else if (isFlag(argv[i], "execute"))
            {
                (*execute)(execute, {object::makeObject(std::string(argv[++i]))}, &globalStack);
            }
            else if (isFlag(argv[i], "doc") && i != argc - 1)
            {
                auto help = (*(*import)(import, {object::makeObject("../library/docs.ez"s)}, &globalStack))[n::help];
                std::mutex m;
                std::condition_variable cv;
                bool done = false;
                auto abort = std::async(std::launch::async, [&m, &cv, &done]() {
                    std::unique_lock<std::mutex> lk(m);
                    cv.wait_for(lk, std::chrono::milliseconds(500), [&done]() { return done; });
                    if (!done)
                        std::abort();
                });
                (*help)(help, {(*execute)(execute, {object::makeObject("return " + std::string(argv[++i]) + ";")}, &globalStack)}, &globalStack);
                {
                    std::unique_lock<std::mutex> lk(m);
                    done = true;
                }
                cv.notify_one();
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
                                   "https://antollo.github.io/Easypt2/");
            }
        }
        runtime::fini(&globalStack);
    }
    catch (objectException &e)
    {
        auto obj = e.getPtr();
        console::stackTrace();
        if (obj->isConvertible<std::string>())
            console::error(obj.getConverted<std::string>());
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