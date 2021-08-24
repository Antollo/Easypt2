#include "../library.h"
#include "../file.h"

void File::init(stack *st)
{
    object::objectPtr File = insertObject("File"_n, constructorCaller);
    object::objectPtr filePrototype = object::makeEmptyObject();

    (*File)[n::classPrototype] = filePrototype;

    addFunctionL(filePrototype, n::constructor, {
        thisObj->setType<std::shared_ptr<file>>();
        thisObj->get<std::shared_ptr<file>>() = std::make_shared<file>();

        if (args.size() > 1 && args[0]->isConvertible<std::string>())
            thisObj->get<std::shared_ptr<file>>()->open(args[0]->getConverted<object::type::String>());
        return thisObj;
    });

    addFunctionL(filePrototype, "read"_n, {
        return object::makeObject(thisObj->get<std::shared_ptr<file>>()->read());
    });

    addFunctionL(filePrototype, "readAsync"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj]() {
            auto f = std::async(std::launch::async, [&thisObj]() {
                coroutineEvent post;
                return thisObj->get<std::shared_ptr<file>>()->read();
            });
            return object::makeObject(await f);
        }));
    });

    addFunctionL(filePrototype, "readLine"_n, {
        return object::makeObject(thisObj->get<std::shared_ptr<file>>()->readLine());
    });

    addFunctionL(filePrototype, "readLineAsync"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj]() {
            auto f = std::async(std::launch::async, [&thisObj]() {
                coroutineEvent post;
                return thisObj->get<std::shared_ptr<file>>()->readLine();
            });
            return object::makeObject(await f);
        }));
    });

    addFunctionL(filePrototype, "readBytes"_n, {
        argsConvertibleGuard<number>(args);
        return object::makeObject(thisObj->get<std::shared_ptr<file>>()->readBytes(static_cast<size_t>(args[0]->getConverted<object::type::Number>())));
    });

    addFunctionL(filePrototype, "readBytesAsync"_n, {
        argsConvertibleGuard<number>(args);
        size_t n = static_cast<size_t>(args[0]->getConverted<object::type::Number>());
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj, n]() {
            auto f = std::async(std::launch::async, [&thisObj, n]() mutable {
                coroutineEvent post;
                return thisObj->get<std::shared_ptr<file>>()->readBytes(n);
            });
            return object::makeObject(await f);
        }));
    });

    addFunctionL(filePrototype, "readTo"_n, {
        argsConvertibleGuard<std::string>(args);
        std::string c = args[0]->getConverted<object::type::String>();
        assert(c.size(), "string empty");
        return object::makeObject(thisObj->get<std::shared_ptr<file>>()->readTo(c[0]));
    });

    addFunctionL(filePrototype, "readToAsync"_n, {
        argsConvertibleGuard<std::string>(args);
        std::string c = args[0]->getConverted<object::type::String>();
        assert(c.size(), "string empty");
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj, c]() {
            auto f = std::async(std::launch::async, [&thisObj, c(std::move(c))]() {
                coroutineEvent post;
                return thisObj->get<std::shared_ptr<file>>()->readTo(c[0]);
            });
            return object::makeObject(await f);
        }));
    });

    addFunctionL(filePrototype, "write"_n, {
        for (auto &el : args)
            thisObj->get<std::shared_ptr<file>>()->write(el->getConverted<object::type::String>());
        return thisObj;
    });

    addFunctionL(filePrototype, "writeAsync"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj, args]() mutable {
            auto f = std::async(std::launch::async, [&thisObj, &args]() {
                coroutineEvent post;
                for (auto &el : args)
                    thisObj->get<std::shared_ptr<file>>()->write(el->getConverted<object::type::String>());
            });
            await f;
            return thisObj;
        }));
    });

    addFunctionL(filePrototype, "writeLine"_n, {
        for (auto &el : args)
            thisObj->get<std::shared_ptr<file>>()->write(el->getConverted<object::type::String>());
        thisObj->get<std::shared_ptr<file>>()->newLine();
        return thisObj;
    });

    addFunctionL(filePrototype, "writeLineAsync"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj, args]() mutable {
            auto f = std::async(std::launch::async, [&thisObj, &args]() {
                coroutineEvent post;
                for (auto &el : args)
                    thisObj->get<std::shared_ptr<file>>()->write(el->getConverted<object::type::String>());
                thisObj->get<std::shared_ptr<file>>()->newLine();
            });
            await f;
            return thisObj;
        }));
    });

    addFunctionL(filePrototype, "open"_n, {
        argsConvertibleGuard<std::string>(args);
        thisObj->get<std::shared_ptr<file>>()->open(args[0]->getConverted<object::type::String>());
        return thisObj;
    });

    addFunctionL(filePrototype, "create"_n, {
        argsConvertibleGuard<std::string>(args);
        thisObj->get<std::shared_ptr<file>>()->create(args[0]->getConverted<object::type::String>());
        return thisObj;
    });

    addFunctionL(filePrototype, "remove"_n, {
        thisObj->get<std::shared_ptr<file>>()->remove();
        return thisObj;
    });

    addFunctionL(filePrototype, "close"_n, {
        thisObj->get<std::shared_ptr<file>>()->close();
        return thisObj;
    });

    addFunctionL(filePrototype, "size"_n, {
        return object::makeObject(static_cast<number>(thisObj->get<std::shared_ptr<file>>()->size()));
    });

    addFunctionL(filePrototype, "flush"_n, {
        thisObj->get<std::shared_ptr<file>>()->flush();
        return thisObj;
    });

    addFunctionL(filePrototype, "setReadPosition"_n, {
        argsConvertibleGuard<number>(args);
        thisObj->get<std::shared_ptr<file>>()->setReadPosition(static_cast<size_t>(args[0]->getConverted<object::type::Number>()));
        return thisObj;
    });

    addFunctionL(filePrototype, "setWritePosition"_n, {
        argsConvertibleGuard<number>(args);
        thisObj->get<std::shared_ptr<file>>()->setWritePosition(static_cast<size_t>(args[0]->getConverted<object::type::Number>()));
        return thisObj;
    });

    addFunctionL(filePrototype, "getReadPosition"_n, {
        return object::makeObject(static_cast<number>(thisObj->get<std::shared_ptr<file>>()->getReadPosition()));
    });

    addFunctionL(filePrototype, "getWritePosition"_n, {
        return object::makeObject(static_cast<number>(thisObj->get<std::shared_ptr<file>>()->getWritePosition()));
    });
}