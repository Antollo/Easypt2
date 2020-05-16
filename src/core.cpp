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
    object::objectPtr sourceFunction = makeObject(root);
    object::objectPtr result;
    {
        stack s(st);
        auto module = s.insert("module"_n, makeEmptyObject());
        module->addProperty("name"_n, makeObject(fileNameString));
        result = (*sourceFunction)(nullptr, std::move(args), &s);
    }
    imported.insert(std::make_pair(fileName.stem().string(), result));
    return result;
}

/*object::objectPtr import(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    static std::unordered_map<std::string, object::objectPtr> imported;
    argsConvertibleGuard<std::nullptr_t>(args);
    std::string source, fileNameString;
    std::filesystem::path fileName;
    if (args[0]->isOfType<std::string>())
    {
        fileName = args[0]->get<const std::string>();
        if (fileName.extension() != ".ez"s)
            fileName += ".ez"s;

        if (imported.count(fileName.stem().string()))
            return imported[fileName.stem().string()];

        std::filesystem::path executablePath = getExecutablePath().parent_path();
        if (std::filesystem::exists(fileName) || std::filesystem::exists(executablePath / fileName))
        {
            file sourceFile(fileName.string());
            if (!sourceFile.isOpen())
            {
                sourceFile.open((executablePath / fileName).string());
                if (!sourceFile.isOpen())
                    throw std::runtime_error("file " + fileName.string() + " cannot be opened");
            }
            source = sourceFile.readTo(EOF);
            fileNameString = sourceFile.getPath().string();
            sourceFile.close();
            if (fileName.stem().extension() == ".min"s)
                parser::registerLiterals(source);
            else
                parser::transpile(source);
        }
        else
            throw std::runtime_error("file " + fileName.string() + " not found");
    }
    else if (args[0]->isOfType<file>())
    {
        file &sourceFile = args[0]->get<file>();
        fileName = sourceFile.getPath();
        // Cache?
        //if (imported.count(sourceFile.getPath().stem().string()))
        //    return imported[sourceFile.getPath().stem().string()];

        if (!sourceFile.isOpen())
            throw std::runtime_error("file " + fileName.string() + " is not opened");
        source = sourceFile.readTo(EOF);
        fileNameString = sourceFile.getPath().string();
        sourceFile.close();
        if (fileName.stem().extension() == ".min"s)
            parser::registerLiterals(source);
        else
            parser::transpile(source);
    }
    else
        throw std::runtime_error("wrong type of argument");

    object::objectPtr sourceFunction = makeObject(compoundStatement(source));
    parser::clearCache();
    object::objectPtr result;
    {
        stack s(st);
        auto module = s.insert("module"_n, makeEmptyObject());
        module->addProperty("name"_n, makeObject(fileNameString));
        result = (*sourceFunction)(nullptr, std::move(args), &s);
    }
    imported.insert(std::make_pair(fileName.stem().string(), result));
    return result;
}*/

object::objectPtr ez_parse(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    argsConvertibleGuard<std::string>(args);
    std::string temp = args[0]->getConverted<std::string>();
    object::functionType root = object::makeFunction();
    treeParser::parseString(temp, root);
    return makeObject(root);
}

object::objectPtr constructorCaller(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    auto newObj = makeEmptyObject();
    newObj->addProperty("prototype"_n, (*thisObj)["classPrototype"_n]);
    if ((*newObj)["prototype"_n]->hasOwnProperty("constructor"_n))
        (*(*newObj)["constructor"_n])(newObj, std::move(args), st);
    return newObj;
}

object::objectPtr getStack(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    return st->toObject();
}