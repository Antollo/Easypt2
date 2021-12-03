#include "../library.h"
#include "../file.h"

void File::init(stack *st)
{
    object::objectPtr File = insertObject("File"_n, constructorCaller);
    object::objectPtr filePrototype = object::makeEmptyObject();

    (*File)[n::classPrototype] = filePrototype;

    filePrototype->addFunctionL(n::constructor, {
        thisObj->setType<std::shared_ptr<file>>();
        thisObj->get<std::shared_ptr<file>>() = std::make_shared<file>();

        if (args.size() > 1 && args[0]->isConvertible<std::string>())
            thisObj->get<std::shared_ptr<file>>()->open(args[0]->getConverted<object::type::String>());
        return thisObj;
    });

    filePrototype->addFunctionL("read"_n, {
        return object::makeObject(thisObj->get<std::shared_ptr<file>>()->read());
    });

    filePrototype->addFunctionL("readAsync"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj]() {
            auto f = std::async(std::launch::async, [&thisObj]() {
                coroutineEvent post;
                return thisObj->get<std::shared_ptr<file>>()->read();
            });
            return object::makeObject(await f);
        }));
    });

    filePrototype->addFunctionL("readLine"_n, {
        return object::makeObject(thisObj->get<std::shared_ptr<file>>()->readLine());
    });

    filePrototype->addFunctionL("readLineAsync"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj]() {
            auto f = std::async(std::launch::async, [&thisObj]() {
                coroutineEvent post;
                return thisObj->get<std::shared_ptr<file>>()->readLine();
            });
            return object::makeObject(await f);
        }));
    });

    filePrototype->addFunctionL("readBytes"_n, {
        argsConvertibleGuard<number>(args);
        return object::makeObject(thisObj->get<std::shared_ptr<file>>()->readBytes(static_cast<size_t>(args[0]->getConverted<object::type::Number>())));
    });

    filePrototype->addFunctionL("readBytesAsync"_n, {
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

    filePrototype->addFunctionL("readTo"_n, {
        argsConvertibleGuard<std::string>(args);
        std::string c = args[0]->getConverted<object::type::String>();
        assert(c.size(), "string empty");
        return object::makeObject(thisObj->get<std::shared_ptr<file>>()->readTo(c[0]));
    });

    filePrototype->addFunctionL("readToAsync"_n, {
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

    filePrototype->addFunctionL("write"_n, {
        for (auto &el : args)
            thisObj->get<std::shared_ptr<file>>()->write(el->getConverted<object::type::String>());
        return thisObj;
    });

    filePrototype->addFunctionL("writeAsync"_n, {
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

    filePrototype->addFunctionL("writeLine"_n, {
        for (auto &el : args)
            thisObj->get<std::shared_ptr<file>>()->write(el->getConverted<object::type::String>());
        thisObj->get<std::shared_ptr<file>>()->newLine();
        return thisObj;
    });

    filePrototype->addFunctionL("writeLineAsync"_n, {
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

    filePrototype->addFunctionL("open"_n, {
        argsConvertibleGuard<std::string>(args);
        thisObj->get<std::shared_ptr<file>>()->open(args[0]->getConverted<object::type::String>());
        return thisObj;
    });

    filePrototype->addFunctionL("create"_n, {
        argsConvertibleGuard<std::string>(args);
        thisObj->get<std::shared_ptr<file>>()->create(args[0]->getConverted<object::type::String>());
        return thisObj;
    });

    filePrototype->addFunctionL("remove"_n, {
        thisObj->get<std::shared_ptr<file>>()->remove();
        return thisObj;
    });

    filePrototype->addFunctionL("close"_n, {
        thisObj->get<std::shared_ptr<file>>()->close();
        return thisObj;
    });

    filePrototype->addFunctionL("size"_n, {
        return object::makeObject(static_cast<number>(thisObj->get<std::shared_ptr<file>>()->size()));
    });

    filePrototype->addFunctionL("flush"_n, {
        thisObj->get<std::shared_ptr<file>>()->flush();
        return thisObj;
    });

    filePrototype->addFunctionL("setReadPosition"_n, {
        argsConvertibleGuard<number>(args);
        thisObj->get<std::shared_ptr<file>>()->setReadPosition(static_cast<size_t>(args[0]->getConverted<object::type::Number>()));
        return thisObj;
    });

    filePrototype->addFunctionL("setWritePosition"_n, {
        argsConvertibleGuard<number>(args);
        thisObj->get<std::shared_ptr<file>>()->setWritePosition(static_cast<size_t>(args[0]->getConverted<object::type::Number>()));
        return thisObj;
    });

    filePrototype->addFunctionL("getReadPosition"_n, {
        return object::makeObject(static_cast<number>(thisObj->get<std::shared_ptr<file>>()->getReadPosition()));
    });

    filePrototype->addFunctionL("getWritePosition"_n, {
        return object::makeObject(static_cast<number>(thisObj->get<std::shared_ptr<file>>()->getWritePosition()));
    });
}