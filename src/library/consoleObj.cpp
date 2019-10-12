#include "../library.h"

void consoleObj::init(stack *st)
{
    auto consoleObj = insertObject("console"_n, nullptr);

    addFunctionL(consoleObj, "write"_n, {
        for (auto &el : args)
            console::write(el->getConverted<std::string>());
        return makeUndefined();
    });
    addFunctionL(consoleObj, "debug"_n, {
        std::string temp;
        for (auto &el : args)
            temp += el->getConverted<std::string>();
        console::debug(temp);
        return makeUndefined();
    });
    addFunctionL(consoleObj, "log"_n, {
        std::string temp;
        for (auto &el : args)
            temp += el->getConverted<std::string>();
        console::log(temp);
        return makeUndefined();
    });
    addFunctionL(consoleObj, "warn"_n, {
        std::string temp;
        for (auto &el : args)
            temp += el->getConverted<std::string>();
        console::warn(temp);
        return makeUndefined();
    });
    addFunctionL(consoleObj, "error"_n, {
        std::string temp;
        for (auto &el : args)
            temp += el->getConverted<std::string>();
        console::error(temp);
        return makeUndefined();
    });
    addFunctionL(consoleObj, "writeLine"_n, {
        for (auto &el : args)
            console::write(el->getConverted<std::string>());
        console::newLine();
        return makeUndefined();
    });
    addFunctionL(consoleObj, "newLine"_n, {
        console::newLine();
        return makeUndefined();
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
}