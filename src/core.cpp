#include <filesystem>
#include <fstream>
#include "statement.h"
#include "osDependent.h"
#include "core.h"
#include "common.h"

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
        std::ifstream sourceFile(fileName.string());
        if (!sourceFile.is_open())
        {
            sourceFile.open((executablePath / fileName).string());
            if (!sourceFile.is_open())
                throw std::runtime_error("file " + fileName.string() + " cannot be opened");
        }
        std::getline(sourceFile, source, (char)EOF);
        sourceFile.close();

        object::objectPtr sourceFunction = makeObject(compoundStatement(source));
        object::objectPtr result = (*sourceFunction)(nullptr, std::move(args), st);
        imported.insert(std::make_pair(fileName.stem().string(), result));
        return result;
    }
    else
        throw std::runtime_error("file " + fileName.string() + " not found");
}

#include "console.h"
object::objectPtr constructorCaller(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    auto newObj = makeUndefined();
    newObj->addProperty("prototype"_n, (*thisObj)["prototype"_n]);
    if ((*newObj)["prototype"_n]->hasOwnProperty("constructor"_n))
        (*(*newObj)["constructor"_n])(newObj, std::move(args), st);
    return newObj;
}