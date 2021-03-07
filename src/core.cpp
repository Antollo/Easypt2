#include "osDependent.h"
#include "core.h"
#include "common.h"
#include "file.h"
#include "treeParser.h"

object::objectPtr Import::getImportPaths(object::objectPtr thisObj, object::type::Array &&args, stack *st)
{
    object::type::Array arr;
    arr.resize(importPaths.size());
    for (size_t i = 0; i < importPaths.size(); i++)
        arr[arr.size() - i - 1] = object::makeObject(std::filesystem::absolute(importPaths[i]).string());
    return object::makeObject(arr);
}

object::objectPtr Import::import(object::objectPtr thisObj, object::type::Array &&args, stack *st)
{
    argsConvertibleGuard<std::nullptr_t>(args);
    std::filesystem::path filePath;
    std::filesystem::path fileName;
    object::type::Function root = object::makeFunction();
    if (args[0]->isOfType<std::string>())
    {
        fileName = args[0]->get<const std::string>();

        if (imported != nullptr) 
        {
            auto it = imported->find(name(fileName.filename().string()));
            if  (it != imported->end() && it->second->hasOwnProperty(n::exports))
                    return (*it->second)[n::exports];
        }

        fileName.replace_extension(".ez"s);

        for (auto it = importPaths.crbegin(); !(it == importPaths.crend()); it++)
        {
            if (std::filesystem::exists(*it / fileName))
            {
                filePath = (*it / fileName);
                break;
            }
        }

        if (filePath.empty())
            throw std::runtime_error("file " + fileName.string() + " not found");
    }
    else if (args[0]->isOfType<std::shared_ptr<file>>())
    {
        auto &sourceFile = args[0]->get<std::shared_ptr<file>>();
        fileName = sourceFile->getPath();

        if (!sourceFile->isOpen())
            throw std::runtime_error("file " + fileName.string() + " is not opened");

        filePath = sourceFile->getPath().string();
    }
    else
        throw std::runtime_error("wrong type of argument");

    treeParser::parseFile(filePath.string(), *root);
    object::objectPtr sourceFunction = object::makeObject(root);
    object::objectPtr result;
    {
        stack s(st);
        auto oldDir = std::filesystem::current_path();
        auto dir = std::filesystem::absolute(filePath.parent_path());
        try
        {
            std::filesystem::current_path(dir);
            importPaths.push_back(dir);
            auto module = s.insert(n::module, object::makeEmptyObject());
            module->addProperty(n::name_, object::makeObject(filePath.stem().string()));
            module->addProperty(n::filename, object::makeObject(filePath.filename().string()));
            module->addProperty(n::path, object::makeObject(dir.string()));
            if (imported != nullptr)
                imported->insert_or_assign(name(fileName.stem().string()), module);
            result = (*sourceFunction)(sourceFunction, std::move(args), &s);
            (*module)[n::exports] = result;
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

object::objectPtr ez_parse(object::objectPtr thisObj, object::type::Array &&args, stack *st)
{
    argsConvertibleGuard<std::string>(args);
    std::string temp = args[0].getConverted<std::string>();
    object::type::Function root = object::makeFunction();
    treeParser::parseString(temp, *root);
    return object::makeObject(root);
}

object::objectPtr execute(object::objectPtr thisObj, object::type::Array &&args, stack *st)
{
    argsConvertibleGuard<std::string>(args);
    std::string temp = args[0].getConverted<std::string>();
    object::type::Function root = object::makeFunction();
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

object::objectPtr constructorCaller(object::objectPtr thisObj, object::type::Array &&args, stack *st)
{
    auto newObj = object::makeEmptyObject();
    newObj->addProperty(n::prototype, (*thisObj)[n::classPrototype]);
    if ((*newObj)[n::prototype]->hasOwnProperty(n::constructor))
        (*(*newObj)[n::constructor])(newObj, std::move(args), st);
    return newObj;
}

object::objectPtr getStack(object::objectPtr thisObj, object::type::Array &&args, stack *st)
{
    auto obj = object::makeEmptyObject();
    st->copyToObject(obj, true);
    return obj;
}