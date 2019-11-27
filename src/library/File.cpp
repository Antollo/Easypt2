#include "../library.h"
#include "../file.h"

void File::init(stack *st)
{
    object::objectPtr File = insertObject("File"_n, constructorCaller);
    object::objectPtr filePrototype = makeUndefined();

    (*File)["prototype"_n] = filePrototype;

    addFunctionL(filePrototype, "constructor"_n, {
        thisObj->setType<file>();
        return thisObj;
    });

    addFunctionL(filePrototype, "read"_n, {
        return makeObject(thisObj->get<file>().read());
    });

    addFunctionL(filePrototype, "readLine"_n, {
        return makeObject(thisObj->get<file>().readLine());
    });

    addFunctionL(filePrototype, "readBytes"_n, {
        argsConvertibleGuard<number>(args);
        return makeObject(thisObj->get<file>().readBytes(static_cast<int>(args[0]->getConverted<number>())));
    });

    addFunctionL(filePrototype, "readTo"_n, {
        argsConvertibleGuard<std::string>(args);
        std::string c = args[0]->getConverted<std::string>();
        assert(c.size(), "string empty");
        return makeObject(thisObj->get<file>().readTo(c[0]));
    });

    addFunctionL(filePrototype, "write"_n, {
        for (auto &el : args)
            thisObj->get<file>().write(el->getConverted<std::string>());
        return thisObj;
    });

    addFunctionL(filePrototype, "writeLine"_n, {
        for (auto &el : args)
            thisObj->get<file>().write(el->getConverted<std::string>());
        thisObj->get<file>().newLine();
        return thisObj;
    });

    addFunctionL(filePrototype, "open"_n, {
        argsConvertibleGuard<std::string>(args);
        thisObj->get<file>().open(args[0]->getConverted<std::string>());
        return thisObj;
    });

    addFunctionL(filePrototype, "create"_n, {
        argsConvertibleGuard<std::string>(args);
        thisObj->get<file>().create(args[0]->getConverted<std::string>());
        return thisObj;
    });

    addFunctionL(filePrototype, "remove"_n, {
        thisObj->get<file>().remove();
        return thisObj;
    });

    addFunctionL(filePrototype, "close"_n, {
        thisObj->get<file>().close();
        return thisObj;
    });
}