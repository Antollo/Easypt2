#include "osDependent.h"
#include "core.h"
#include "common.h"
#include "file.h"
#include "treeParser.h"

object::objectPtr import(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    static std::unordered_map<std::string, object::objectPtr> imported;
    argsConvertibleGuard<std::nullptr_t>(args);
    std::string fileNameString;
    std::filesystem::path fileName;
    object::functionType root = object::makeFunction();
    if (args[0]->isOfType<std::string>())
    {
        fileName = args[0]->get<const std::string>();
        if (fileName.extension() != ".ez"s)
            fileName += ".ez"s;

        if (imported.count(fileName.stem().string()))
            return imported[fileName.stem().string()];

        std::filesystem::path executablePath = getExecutablePath().parent_path();

        if (std::filesystem::exists(fileName))
            fileNameString = fileName.string();
        else if (std::filesystem::exists(executablePath / fileName))
            fileNameString = (executablePath / fileName).string();
        else
            throw std::runtime_error("file " + fileName.string() + " not found");
    }
    else if (args[0]->isOfType<file>())
    {
        file &sourceFile = args[0]->get<file>();
        fileName = sourceFile.getPath();

        if (!sourceFile.isOpen())
            throw std::runtime_error("file " + fileName.string() + " is not opened");

        fileNameString = sourceFile.getPath().string();
    }
    else
        throw std::runtime_error("wrong type of argument");

    treeParser::parseFile(fileNameString, root);
    object::objectPtr sourceFunction = object::makeObject(root);
    object::objectPtr result;
    {
        stack s(st);
        auto module = s.insert("module"_n, object::makeEmptyObject());
        module->addProperty("name"_n, object::makeObject(fileNameString));
        result = (*sourceFunction)(sourceFunction, std::move(args), &s);
    }
    imported.insert(std::make_pair(fileName.stem().string(), result));
    return result;
}

object::objectPtr ez_parse(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    argsConvertibleGuard<std::string>(args);
    std::string temp = args[0]->getConverted<std::string>();
    object::functionType root = object::makeFunction();
    treeParser::parseString(temp, root);
    return object::makeObject(root);
}

object::objectPtr execute(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    argsConvertibleGuard<std::string>(args);
    std::string temp = args[0]->getConverted<std::string>();
    object::functionType root = object::makeFunction();
    treeParser::parseString(temp, root);
    size_t i = 0;
    try
    {
        for (; i < root.children().size(); i++)
            root.children()[i].evaluate(*st);
    }
    catch (const object::objectPtr &ret)
    {
        return ret;
    }
    catch (objectException &e)
    {
        throw e;
    }
    catch (std::exception &e)
    {
        throw e;
    }
    return thisObj;
}

/*object::objectPtr repl(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    console::log("repl");
    useStdio stdio;
    treeParser::file = "repl";
    object::functionType root = object::makeFunction();
    treeParser::parseStream(std::cin, root);
    return object::makeObject(root);
}*/

object::objectPtr constructorCaller(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    auto newObj = object::makeEmptyObject();
    newObj->addProperty(name::prototype, (*thisObj)["classPrototype"_n]);
    if ((*newObj)[name::prototype]->hasOwnProperty("constructor"_n))
        (*(*newObj)["constructor"_n])(newObj, std::move(args), st);
    return newObj;
}

object::objectPtr getStack(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    return st->toObject();
}