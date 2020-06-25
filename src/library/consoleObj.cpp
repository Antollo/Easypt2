#include "../library.h"

void consoleObj::init(stack *st)
{
    object::objectPtr consoleObj = insertObject("console"_n, nullptr);

    addFunctionL(consoleObj, "write"_n, {
        for (auto &el : args)
            console::write(el->getConverted<std::string>());
        return thisObj;
    });
    addFunctionL(consoleObj, "debug"_n, {
        std::string temp;
        for (auto &el : args)
            temp += el->getConverted<std::string>();
        console::debug(temp);
        return thisObj;
    });
    addFunctionL(consoleObj, "log"_n, {
        std::string temp;
        for (auto &el : args)
            temp += el->getConverted<std::string>();
        console::log(temp);
        return thisObj;
    });
    addFunctionL(consoleObj, "warn"_n, {
        std::string temp;
        for (auto &el : args)
            temp += el->getConverted<std::string>();
        console::warn(temp);
        return thisObj;
    });
    addFunctionL(consoleObj, "error"_n, {
        std::string temp;
        for (auto &el : args)
            temp += el->getConverted<std::string>();
        console::error(temp);
        return thisObj;
    });
    addFunctionL(consoleObj, "writeLine"_n, {
        for (auto &el : args)
            console::write(el->getConverted<std::string>());
        console::newLine();
        return thisObj;
    });
    addFunctionL(consoleObj, "newLine"_n, {
        console::newLine();
        return thisObj;
    });
    addFunctionL(consoleObj, "read"_n, {
        std::string temp;
        console::read(temp);
        return object::makeObject(temp);
    });
    addFunctionL(consoleObj, "readAsync"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([]() {
            std::string temp;
            auto f =  std::async(std::launch::async, [&temp](){
                console::read(temp);
            });
            await f;
            return object::makeObject(temp);
        }));
    });
    addFunctionL(consoleObj, "readLine"_n, {
        std::string temp;
        console::readLine(temp);
        return object::makeObject(temp);
    });
    addFunctionL(consoleObj, "readLine"_n, {
        std::string temp;
        console::readLine(temp);
        return object::makeObject(temp);
    });
    addFunctionL(consoleObj, "controlSequence"_n, {
        argsConvertibleGuard<std::string>(args);
        console::controlSequence(args[0]->getConverted<std::string>());
        return thisObj;
    });
    addFunctionL(consoleObj, "getOutput"_n, {
        argsGuard<std::nullptr_t>(args);
        return object::makeObject(console::getOutput([&args, &st]() {
            (*args[0])(args[0], object::arrayType(args.begin() + 1, args.end()), st);
        }));
    });
}