#include <filesystem>
#include <fstream>
#include "statement.h"
#include "osDependent.h"
#include "core.h"

object::objectPtr import(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    //TODO args guardian
    static std::unordered_map<std::string, object::objectPtr> imported;
    std::filesystem::path fileName = args[0]->get<const std::string>();
    if (fileName.extension() != ".ez"s)
        fileName += ".ez"s;
    std::filesystem::path executablePath = getExecutablePath().parent_path();

    if (imported.count(fileName.stem().string()))
        return imported[fileName.stem().string()];

    if (std::filesystem::exists(fileName) || std::filesystem::exists(executablePath / fileName))
    {
        std::string source;
        std::ifstream sourceFile(fileName.string());
        if (!sourceFile.is_open())
        {
            sourceFile.open((executablePath / fileName).string());
            if (!sourceFile.is_open())
                throw std::runtime_error(fileName.string() + " cannot be opened");
        }
        std::getline(sourceFile, source, (char)EOF);
        sourceFile.close();

        object::objectPtr sourceFunction = makeObject(compoundStatement(source));
        object::objectPtr result = (*sourceFunction)(nullptr, std::move(args), st);
        imported.insert(std::make_pair(fileName.stem().string(), result));
        return result;
    }
    else
        throw std::runtime_error(fileName.string() + " not found");
}