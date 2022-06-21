#include "../library.h"
#include "../childProcess.h"
#include "../file.h"

void ChildProcess::init(stack *st)
{
    object::objectPtr ChildProcess = insertObject("ChildProcess"_n, constructorCaller);
    object::objectPtr childProcessPrototype = object::makeEmptyObject();

    (*ChildProcess)[n::classPrototype] = childProcessPrototype;

    childProcessPrototype->addFunctionL(n::constructor, {
        argsConvertibleGuard<object::type::String, object::type::Array>(args);
        std::string cwd;

        if (args.size() > 2 && args[2]->isConvertible<object::type::String>())
            cwd = args[2]->getConverted<object::type::String>();
        else
            cwd = std::filesystem::absolute(std::filesystem::current_path()).string();

        object::type::Array argsIn = args[1]->getConverted<object::type::Array>();
        std::vector<std::string> argsV(argsIn.size());

        for (size_t i = 0; i < argsIn.size(); i++)
            argsV[i] = argsIn[i]->getConverted<object::type::String>();
        

        thisObj->setType<object::type::ChildProcess>();
        auto &_childProcess = (thisObj->get<object::type::ChildProcess>() = std::make_shared<childProcess>(
            args[0]->getConverted<object::type::String>(),
            argsV,
            cwd
        ));

        auto &_File = (*st)["File"_n];
        auto &_stdin = (*thisObj)["stdin"_n]  = (*_File)(_File, {}, st);
        auto &_stdout = (*thisObj)["stdout"_n]  = (*_File)(_File, {}, st);
        auto &_stderr = (*thisObj)["stderr"_n] = (*_File)(_File, {}, st);

        _stdin->get<object::type::File>()->f = _childProcess->getInput();
        _stdout->get<object::type::File>()->f = _childProcess->getOutput();
        _stderr->get<object::type::File>()->f = _childProcess->getError();

        return thisObj;
    });

    childProcessPrototype->addFunctionL("running"_n, {
        object::type::ChildProcess &me = thisObj->get<object::type::ChildProcess>();
        return object::makeObject(me->running());
    });

    childProcessPrototype->addFunctionL("kill"_n, {
        object::type::ChildProcess &me = thisObj->get<object::type::ChildProcess>();
        return object::makeObject(me->kill());
    });

    childProcessPrototype->addFunctionL("wait"_n, {
        return object::makeObject(coroutine<object::objectPtr>::makeCoroutine([thisObj, args]() mutable {
            object::type::ChildProcess &me = thisObj->get<object::type::ChildProcess>();
            auto f = std::async(std::launch::async, [&me]() {
                coroutineEvent post;
                return me->wait();
            });
            return object::makeObject(await f);
        }));
    });
}