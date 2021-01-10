#include "../library.h"
#include "../tcp.h"

void Tcp::init(stack *st)
{
    object::objectPtr TcpClient = insertObject("TcpClient"_n, constructorCaller);
    object::objectPtr tcpClientPrototype = object::makeEmptyObject();

    (*TcpClient)["classPrototype"_n] = tcpClientPrototype;

    addFunctionL(tcpClientPrototype, "constructor"_n, {
        thisObj->setType<std::shared_ptr<tcpClient>>();
        thisObj->get<std::shared_ptr<tcpClient>>() = std::make_shared<tcpClient>();
        return thisObj;
    });

    addFunctionL(tcpClientPrototype, "connect"_n, {
        argsConvertibleGuard<std::string, number>(args);

        std::string adress = args[0]->getConverted<std::string>();
        int port = static_cast<int>(args[1]->getConverted<number>());

        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([adress, port, thisObj]() {
            auto t = thisObj->get<std::shared_ptr<tcpClient>>();
            auto f = std::async(std::launch::async, [t, &adress, port]() {
                t->connect(adress, port);
            });
            await f;
            return thisObj;
        }));
    });

    addFunctionL(tcpClientPrototype, "receive"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj]() {
            auto t = thisObj->get<std::shared_ptr<tcpClient>>();
            std::string str;
            auto f = std::async(std::launch::async, [t, &str]() {
                t->receive(str);
            });
            await f;
            return object::makeObject(str);
        }));
    });

    addFunctionL(tcpClientPrototype, "send"_n, {
        argsConvertibleGuard<std::string>(args);

        std::string str = args[0]->getConverted<std::string>();

        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([str, thisObj]() {
            auto t = thisObj->get<std::shared_ptr<tcpClient>>();
            auto f = std::async(std::launch::async, [t, &str]() {
                t->send(str);
            });
            await f;
            return thisObj;
        }));
    });

    addFunctionL(tcpClientPrototype, "close"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj]() {
            auto t = thisObj->get<std::shared_ptr<tcpClient>>();
            auto f = std::async(std::launch::async, [t]() {
                t->close();
            });
            await f;
            return thisObj;
        }));
    });

    object::objectPtr TcpServer = insertObject("TcpServer"_n, constructorCaller);
    object::objectPtr tcpServerPrototype = object::makeEmptyObject();

    (*TcpServer)["classPrototype"_n] = tcpServerPrototype;

    addFunctionL(tcpServerPrototype, "constructor"_n, {
        thisObj->setType<std::shared_ptr<tcpServer>>();
        thisObj->get<std::shared_ptr<tcpServer>>() = std::make_shared<tcpServer>();
        return thisObj;
    });

    addFunctionL(tcpServerPrototype, "bind"_n, {
        argsConvertibleGuard<number>(args);

        int port = static_cast<int>(args[0]->getConverted<number>());

        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([port, thisObj]() {
            auto t = thisObj->get<std::shared_ptr<tcpServer>>();
            auto f = std::async(std::launch::async, [t, port]() {
                t->bind(port);
            });
            await f;
            return thisObj;
        }));
    });

    addFunctionL(tcpServerPrototype, "listen"_n, {
        auto tcpClientPrototype = (*(*st)["TcpClient"_n])["classPrototype"_n];

        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([tcpClientPrototype, thisObj]() {
            auto t = thisObj->get<std::shared_ptr<tcpServer>>();
            std::shared_ptr<tcpClient> c;
            auto f = std::async(std::launch::async, [t, &c]() {
                c = t->listen();
            });
			await f;
            auto tcpClientObject = object::makeEmptyObject();
            tcpClientObject->setType<std::shared_ptr<tcpClient>>();
            tcpClientObject->get<std::shared_ptr<tcpClient>>() = c;
            (*tcpClientObject)[name::prototype] = tcpClientPrototype;
            return tcpClientObject;
        }));
    });

    addFunctionL(tcpServerPrototype, "close"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj]() {
            auto t = thisObj->get<std::shared_ptr<tcpServer>>();
            auto f = std::async(std::launch::async, [t]() {
                t->close();
            });
            await f;
            return thisObj;
        }));
    });
}