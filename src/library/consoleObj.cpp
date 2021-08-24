#include "../library.h"

void consoleObj::init(stack *st)
{
    object::objectPtr consoleObj = insertObject("console"_n, nullptr);

    addFunctionL(consoleObj, "write"_n, {
        for (auto &el : args)
            console::write(el->getConverted<object::type::String>());
        return thisObj;
    });
    addFunctionL(consoleObj, "writeAsync"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj, args]() mutable {
            auto f = std::async(std::launch::async, [&args]() mutable {
                coroutineEvent post;
                for (auto &el : args)
                    console::write(el->getConverted<object::type::String>());
            });
            await f;
            return thisObj;
        }));
    });
    addFunctionL(consoleObj, "debug"_n, {
        std::string temp;
        for (auto &el : args)
            temp += el->getConverted<object::type::String>();
        console::debug(temp);
        return thisObj;
    });
    addFunctionL(consoleObj, "log"_n, {
        std::string temp;
        for (auto &el : args)
            temp += el->getConverted<object::type::String>();
        console::log(temp);
        return thisObj;
    });
    addFunctionL(consoleObj, "warn"_n, {
        std::string temp;
        for (auto &el : args)
            temp += el->getConverted<object::type::String>();
        console::warn(temp);
        return thisObj;
    });
    addFunctionL(consoleObj, "error"_n, {
        std::string temp;
        for (auto &el : args)
            temp += el->getConverted<object::type::String>();
        console::error(temp);
        return thisObj;
    });
    addFunctionL(consoleObj, "writeLine"_n, {
        for (auto &el : args)
            console::write(el->getConverted<object::type::String>());
        console::newLine();
        return thisObj;
    });
    addFunctionL(consoleObj, "writeLineAsync"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj, args]() mutable {
            auto f = std::async(std::launch::async, [&args]() mutable {
                coroutineEvent post;
                for (auto &el : args)
                    console::write(el->getConverted<object::type::String>());
                console::newLine();
            });
            await f;
            return thisObj;
        }));
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
            auto f = std::async(std::launch::async, []() {
                coroutineEvent post;
                std::string temp;
                console::read(temp);
                return temp;
            });
            return object::makeObject(await f);
        }));
    });
    addFunctionL(consoleObj, "readLine"_n, {
        std::string temp;
        console::readLine(temp);
        return object::makeObject(temp);
    });
    addFunctionL(consoleObj, "readLineAsync"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([]() {
            auto f = std::async(std::launch::async, []() {
                coroutineEvent post;
                std::string temp;
                console::readLine(temp);
                return temp;
            });
            return object::makeObject(await f);
        }));
    });
    addFunctionL(consoleObj, "controlSequence"_n, {
        argsConvertibleGuard<std::string>(args);
        console::controlSequence(args[0]->getConverted<object::type::String>());
        return thisObj;
    });
    addFunctionL(consoleObj, "getOutput"_n, {
        argsGuard<std::nullptr_t>(args);
        return object::makeObject(console::getOutput([&args, &st]() {
            (*args[0])(args[0], object::type::Array(args.begin() + 1, args.end()), st);
        }));
    });
    addFunctionL(consoleObj, "setInput"_n, {
        argsGuard<std::string, std::nullptr_t>(args);
        console::setInput(args[0]->getConverted<object::type::String>(), [&args, &st]() {
            (*args[1])(args[1], object::type::Array(args.begin() + 2, args.end()), st);
        });
        return thisObj;
    });
    addFunctionL(consoleObj, "stackTrace"_n, {
        console::stackTrace();
        return thisObj;
    });
}