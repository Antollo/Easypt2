#include "../library.h"

void consoleObj::init(stack *st)
{
    object::objectPtr consoleObj = insertObject("console"_n, nullptr);

    addFunctionL(consoleObj, "write"_n, {
        for (auto &el : args)
            console::write(el->getConverted<std::string>());
        return makeEmptyObject();
    });
    addFunctionL(consoleObj, "debug"_n, {
        std::string temp;
        for (auto &el : args)
            temp += el->getConverted<std::string>();
        console::debug(temp);
        return makeEmptyObject();
    });
    addFunctionL(consoleObj, "log"_n, {
        std::string temp;
        for (auto &el : args)
            temp += el->getConverted<std::string>();
        console::log(temp);
        return makeEmptyObject();
    });
    addFunctionL(consoleObj, "warn"_n, {
        std::string temp;
        for (auto &el : args)
            temp += el->getConverted<std::string>();
        console::warn(temp);
        return makeEmptyObject();
    });
    addFunctionL(consoleObj, "error"_n, {
        std::string temp;
        for (auto &el : args)
            temp += el->getConverted<std::string>();
        console::error(temp);
        return makeEmptyObject();
    });
    addFunctionL(consoleObj, "writeLine"_n, {
        for (auto &el : args)
            console::write(el->getConverted<std::string>());
        console::newLine();
        return makeEmptyObject();
    });
    addFunctionL(consoleObj, "newLine"_n, {
        console::newLine();
        return makeEmptyObject();
    });
    addFunctionL(consoleObj, "read"_n, {
        std::string temp;
        console::read(temp);
        return makeObject(temp);
    });
    addFunctionL(consoleObj, "readLine"_n, {
        std::string temp;
        console::readLine(temp);
        return makeObject(temp);
    });
    addFunctionL(consoleObj, "readLine"_n, {
        std::string temp;
        console::readLine(temp);
        return makeObject(temp);
    });
    addFunctionL(consoleObj, "controlSequence"_n, {
        argsConvertibleGuard<std::string>(args);
        console::controlSequence(args[0]->getConverted<std::string>());
        return makeEmptyObject();
    });
    addFunctionL(consoleObj, "getOutput"_n, {
        argsGuard<std::nullptr_t>(args);
        return makeObject(console::getOutput([&args, &st]() {
            (*args[0])(nullptr, {}, st);
        }));
    });
}