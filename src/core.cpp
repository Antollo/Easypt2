#include "osDependent.h"
#include "core.h"
#include "common.h"
#include "file.h"
#include "treeParser.h"

object::objectPtr Import::getImportPaths(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    object::arrayType arr;
    arr.resize(importPaths.size());
    for (size_t i = 0; i < importPaths.size(); i++)
        arr[arr.size() - i - 1] = object::makeObject(std::filesystem::absolute(importPaths[i]).string());
    return object::makeObject(arr);
}

object::objectPtr Import::import(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    argsConvertibleGuard<std::nullptr_t>(args);
    std::string fileNameString;
    std::filesystem::path fileName;
    object::functionType root = object::makeFunction();
    if (args[0]->isOfType<std::string>())
    {
        fileName = args[0]->get<const std::string>();
        if (fileName.extension() != ".ez"s)
            fileName += ".ez"s;

        if (imported != nullptr) 
        {
            auto it = imported->find(name(fileName.stem().string()));
            if  (it != imported->end() && it->second->hasOwnProperty("exports"_n))
                    return (*it->second)["exports"_n];
        }

        for (auto it = importPaths.crbegin(); !(it == importPaths.crend()); it++)
        {
            if (std::filesystem::exists(*it / fileName))
            {
                fileNameString = (*it / fileName).string();
                break;
            }
        }

        if (fileNameString.empty())
            throw std::runtime_error("file " + fileName.string() + " not found");
    }
    else if (args[0]->isOfType<std::shared_ptr<file>>())
    {
        auto &sourceFile = args[0]->get<std::shared_ptr<file>>();
        fileName = sourceFile->getPath();

        if (!sourceFile->isOpen())
            throw std::runtime_error("file " + fileName.string() + " is not opened");

        fileNameString = sourceFile->getPath().string();
    }
    else
        throw std::runtime_error("wrong type of argument");

    treeParser::parseFile(fileNameString, *root);
    object::objectPtr sourceFunction = object::makeObject(root);
    object::objectPtr result;
    {
        stack s(st);
        auto oldDir = std::filesystem::current_path();
        auto dir = std::filesystem::absolute(std::filesystem::path(fileNameString)).parent_path();
        try
        {
            std::filesystem::current_path(dir);
            importPaths.push_back(dir);
            auto module = s.insert("module"_n, object::makeEmptyObject());
            module->addProperty("name"_n, object::makeObject(std::filesystem::path(fileNameString).stem().string()));
            module->addProperty("filename"_n, object::makeObject(std::filesystem::path(fileNameString).filename().string()));
            module->addProperty("path"_n, object::makeObject(dir.string()));
            if (imported != nullptr)
                imported->insert_or_assign(name(fileName.stem().string()), module);
            result = (*sourceFunction)(sourceFunction, std::move(args), &s);
            (*module)["exports"_n] = result;
            std::filesystem::current_path(oldDir);
            importPaths.pop_back();
        }   
        catch(...)
        {
            std::filesystem::current_path(oldDir);
            importPaths.pop_back();
            throw;
        }
    }
    return result;
}

object::objectPtr ez_parse(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    argsConvertibleGuard<std::string>(args);
    std::string temp = args[0]->getConverted<std::string>();
    object::functionType root = object::makeFunction();
    treeParser::parseString(temp, *root);
    return object::makeObject(root);
}

object::objectPtr execute(object::objectPtr thisObj, object::arrayType &&args, stack *st)
{
    argsConvertibleGuard<std::string>(args);
    std::string temp = args[0]->getConverted<std::string>();
    object::functionType root = object::makeFunction();
    treeParser::parseString(temp, *root);
    size_t i = 0;
    try
    {
        for (; i < root->children().size(); i++)
            root->children()[i].evaluate(*st);
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
    auto obj = object::makeEmptyObject();
    st->copyToObject(obj, true);
    return obj;
}