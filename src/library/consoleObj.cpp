#include <sstream>
#include "../library.h"

void consoleObj::init(stack *st)
{
    object::objectPtr consoleObj = insertObject("console"_n, nullptr);

    consoleObj->addFunctionL("write"_n, {
        for (auto &el : args)
            console::write(el->getConverted<object::type::String>());
        return thisObj;
    });
    consoleObj->addFunctionL("writeAsync"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj, args]() mutable {
            std::stringstream temp;
            for (auto &el : args)
                temp << el->getConverted<object::type::String>();
            auto f = std::async(std::launch::async, [&temp]() mutable {
                coroutineEvent post;
                console::write(temp.str());
            });
            await f;
            return thisObj;
        }));
    });
    consoleObj->addFunctionL("debug"_n, {
        std::stringstream temp;
        for (auto &el : args)
            temp << el->getConverted<object::type::String>();
        console::debug(temp.str());
        return thisObj;
    });
    consoleObj->addFunctionL("log"_n, {
        std::stringstream temp;
        for (auto &el : args)
            temp << el->getConverted<object::type::String>();
        console::log(temp.str());
        return thisObj;
    });
    consoleObj->addFunctionL("ok"_n, {
        std::stringstream temp;
        for (auto &el : args)
            temp << el->getConverted<object::type::String>();
        console::ok(temp.str());
        return thisObj;
    });
    consoleObj->addFunctionL("warn"_n, {
        std::stringstream temp;
        for (auto &el : args)
            temp << el->getConverted<object::type::String>();
        console::warn(temp.str());
        return thisObj;
    });
    consoleObj->addFunctionL("error"_n, {
        std::stringstream temp;
        for (auto &el : args)
            temp << el->getConverted<object::type::String>();
        console::error(temp.str());
        return thisObj;
    });
    consoleObj->addFunctionL("writeLine"_n, {
        for (auto &el : args)
            console::write(el->getConverted<object::type::String>());
        console::newLine();
        return thisObj;
    });
    consoleObj->addFunctionL("writeLineAsync"_n, {
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
    consoleObj->addFunctionL("newLine"_n, {
        console::newLine();
        return thisObj;
    });
    consoleObj->addFunctionL("read"_n, {
        std::string temp;
        console::read(temp);
        return object::makeObject(temp);
    });
    consoleObj->addFunctionL("readAsync"_n, {
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
    consoleObj->addFunctionL("readLine"_n, {
        std::string temp;
        console::readLine(temp);
        return object::makeObject(temp);
    });
    consoleObj->addFunctionL("readLineAsync"_n, {
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
    consoleObj->addFunctionL("controlSequence"_n, {
        argsConvertibleGuard<std::string>(args);
        console::controlSequence(args[0]->getConverted<object::type::String>());
        return thisObj;
    });
    consoleObj->addFunctionL("getOutput"_n, {
        argsGuard<std::nullptr_t>(args);
        return object::makeObject(console::getOutput([&args, &st]() {
            (*args[0])(args[0], object::type::Array(args.begin() + 1, args.end()), st);
        }));
    });
    consoleObj->addFunctionL("setInput"_n, {
        argsGuard<std::string, std::nullptr_t>(args);
        console::setInput(args[0]->getConverted<object::type::String>(), [&args, &st]() {
            (*args[1])(args[1], object::type::Array(args.begin() + 2, args.end()), st);
        });
        return thisObj;
    });
    consoleObj->addFunctionL("stackTrace"_n, {
        console::printStackTrace();
        return thisObj;
    });
}