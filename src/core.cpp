#include <filesystem>
#include "statement.h"
#include "osDependent.h"
#include "core.h"
#include "common.h"
#include "file.h"
#include "parser.h"

object::objectPtr import(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    static std::unordered_map<std::string, object::objectPtr> imported;
    argsConvertibleGuard<std::string>(args);
    std::filesystem::path fileName = args[0]->get<const std::string>();
    if (fileName.extension() != ".ez"s)
        fileName += ".ez"s;

    if (imported.count(fileName.stem().string()))
        return imported[fileName.stem().string()];

    std::filesystem::path executablePath = getExecutablePath().parent_path();

    if (std::filesystem::exists(fileName) || std::filesystem::exists(executablePath / fileName))
    {
        std::string source;
        file sourceFile(fileName.string());
        if (!sourceFile.isOpen())
        {
            sourceFile.open((executablePath / fileName).string());
            if (!sourceFile.isOpen())
                throw std::runtime_error("file " + fileName.string() + " cannot be opened");
        }
        source = sourceFile.readTo(EOF);
        sourceFile.close();
        if (fileName.stem().extension() == ".min"s)
            parser::registerLiterals(source);
        else
            parser::transpile(source);
        object::objectPtr sourceFunction = makeObject(compoundStatement(source));
        parser::clearCache();
        object::objectPtr result = (*sourceFunction)(nullptr, std::move(args), st);
        imported.insert(std::make_pair(fileName.stem().string(), result));
        return result;
    }
    else
        throw std::runtime_error("file " + fileName.string() + " not found");
}

object::objectPtr parse(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    argsConvertibleGuard<std::string>(args);
    std::string temp = args[0]->getConverted<std::string>();
    parser::transpile(temp);
    auto res = makeObject(compoundStatement(temp));
    parser::clearCache();
    return res;
}

object::objectPtr transpile(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    argsConvertibleGuard<std::string>(args);
    std::string temp = args[0]->getConverted<std::string>();
    parser::transpile(temp);
    parser::restoreLiterals(temp);
    auto res = makeObject(temp);
    parser::clearCache();
    return res;
}

object::objectPtr constructorCaller(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    auto newObj = makeUndefined();
    newObj->addProperty("prototype"_n, (*thisObj)["prototype"_n]);
    if ((*newObj)["prototype"_n]->hasOwnProperty("constructor"_n))
        (*(*newObj)["constructor"_n])(newObj, std::move(args), st);
    return newObj;
}