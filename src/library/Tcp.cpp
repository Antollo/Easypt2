#include "../library.h"
#include "../tcp.h"

void Tcp::init(stack *st)
{
    object::objectPtr TcpClient = insertObject("TcpClient"_n, constructorCaller);
    object::objectPtr tcpClientPrototype = object::makeEmptyObject();

    (*TcpClient)[n::classPrototype] = tcpClientPrototype;

    tcpClientPrototype->addFunctionL(n::constructor, {
        thisObj->setType<std::shared_ptr<tcpClient>>();
        thisObj->get<std::shared_ptr<tcpClient>>() = std::make_shared<tcpClient>();
        return thisObj;
    });

    tcpClientPrototype->addFunctionL("connect"_n, {
        argsConvertibleGuard<std::string, number>(args);

        std::string adress = args[0]->getConverted<object::type::String>();
        int port = static_cast<int>(args[1]->getConverted<object::type::Number>());

        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([adress, port, thisObj]() {
            auto t = thisObj->get<std::shared_ptr<tcpClient>>();
            auto f = std::async(std::launch::async, [t, &adress, port]() {
                coroutineEvent post;
                t->connect(adress, port);
            });
            await f;
            return thisObj;
        }));
    });

    tcpClientPrototype->addFunctionL("receive"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj]() {
            auto t = thisObj->get<std::shared_ptr<tcpClient>>();
            std::string str;
            auto f = std::async(std::launch::async, [t, &str]() {
                coroutineEvent post;
                t->receive(str);
            });
            await f;
            return object::makeObject(str);
        }));
    });

    tcpClientPrototype->addFunctionL("send"_n, {
        argsConvertibleGuard<std::string>(args);

        std::string str = args[0]->getConverted<object::type::String>();

        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([str, thisObj]() {
            auto t = thisObj->get<std::shared_ptr<tcpClient>>();
            auto f = std::async(std::launch::async, [t, &str]() {
                coroutineEvent post;
                t->send(str);
            });
            await f;
            return thisObj;
        }));
    });

    tcpClientPrototype->addFunctionL("close"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj]() {
            auto t = thisObj->get<std::shared_ptr<tcpClient>>();
            auto f = std::async(std::launch::async, [t]() {
                coroutineEvent post;
                t->close();
            });
            await f;
            return thisObj;
        }));
    });

    object::objectPtr SslClient = insertObject("SslClient"_n, constructorCaller);
    object::objectPtr sslClientPrototype = object::makeEmptyObject();
    (*sslClientPrototype)[n::prototype] = tcpClientPrototype;
    (*SslClient)[n::classPrototype] = sslClientPrototype;

    sslClientPrototype->addFunctionL(n::constructor, {
        thisObj->setType<std::shared_ptr<tcpClient>>();
        thisObj->get<std::shared_ptr<tcpClient>>() = std::make_shared<sslClient>();
        return thisObj;
    });

    object::objectPtr TcpServer = insertObject("TcpServer"_n, constructorCaller);
    object::objectPtr tcpServerPrototype = object::makeEmptyObject();

    (*TcpServer)[n::classPrototype] = tcpServerPrototype;

    tcpServerPrototype->addFunctionL(n::constructor, {
        thisObj->setType<std::shared_ptr<tcpServer>>();
        thisObj->get<std::shared_ptr<tcpServer>>() = std::make_shared<tcpServer>();
        return thisObj;
    });

    tcpServerPrototype->addFunctionL("bind"_n, {
        argsConvertibleGuard<number>(args);

        int port = static_cast<int>(args[0]->getConverted<object::type::Number>());

        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([port, thisObj]() {
            auto t = thisObj->get<std::shared_ptr<tcpServer>>();
            auto f = std::async(std::launch::async, [t, port]() {
                coroutineEvent post;
                t->bind(port);
            });
            await f;
            return thisObj;
        }));
    });

    
    (*tcpServerPrototype)[n::__client] = tcpClientPrototype;
    tcpServerPrototype->addFunctionL("listen"_n, {
        auto tcpClientPrototype = (*(*thisObj)[n::prototype])[n::__client];

        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([tcpClientPrototype, thisObj]() {
            auto t = thisObj->get<std::shared_ptr<tcpServer>>();
            std::shared_ptr<tcpClient> c;
            auto f = std::async(std::launch::async, [t, &c]() {
                coroutineEvent post;
                c = t->listen();
            });
            await f;
            auto tcpClientObject = object::makeEmptyObject();
            tcpClientObject->setType<std::shared_ptr<tcpClient>>();
            tcpClientObject->get<std::shared_ptr<tcpClient>>() = c;
            (*tcpClientObject)[n::prototype] = tcpClientPrototype;
            return tcpClientObject;
        }));
    });

    tcpServerPrototype->addFunctionL("close"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj]() {
            auto t = thisObj->get<std::shared_ptr<tcpServer>>();
            auto f = std::async(std::launch::async, [t]() {
                coroutineEvent post;
                t->close();
            });
            await f;
            return thisObj;
        }));
    });


    object::objectPtr SslServer = insertObject("SslServer"_n, constructorCaller);
    object::objectPtr sslServerPrototype = object::makeEmptyObject();
    (*sslServerPrototype)[n::prototype] = tcpServerPrototype;
    (*SslServer)[n::classPrototype] = sslServerPrototype;
    (*sslServerPrototype)[n::__client] = sslClientPrototype;

    sslServerPrototype->addFunctionL(n::constructor, {
        argsConvertibleGuard<std::string, std::string>(args);

        thisObj->setType<std::shared_ptr<tcpServer>>();
        thisObj->get<std::shared_ptr<tcpServer>>() = std::make_shared<sslServer>(args[0]->getConverted<object::type::String>(), args[1]->getConverted<object::type::String>(), modules::getImportPaths());
        return thisObj;
    });
}