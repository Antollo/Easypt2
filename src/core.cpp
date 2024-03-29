#include "osDependent.h"
#include "core.h"
#include "common.h"
#include "file.h"
#include "treeParser.h"
#include "externalLibrary.h"

object::objectPtr modules::getImportPaths(object::objectPtr thisObj, object::type::Array &&args, stack *st)
{
    object::type::Array arr;
    arr.resize(importPaths.size());
    for (size_t i = 0; i < importPaths.size(); i++)
        arr[arr.size() - i - 1] = object::makeObject(std::filesystem::absolute(importPaths[i]).string());
    return object::makeObject(arr);
}

object::objectPtr modules::import(object::objectPtr thisObj, object::type::Array &&args, stack *st)
{
    argsConvertibleGuard<std::nullptr_t>(args);
    std::filesystem::path filePath;
    std::filesystem::path fileName;
    if (args[0]->isOfType<std::string>())
    {
        fileName = args[0]->get<const std::string>();

        if (imported != nullptr)
        {
            auto it = imported->find(name(fileName.filename().string()));
            if (it != imported->end() && it->second->hasOwnProperty(n::exports))
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
        {

            fileName.replace_extension(".dll"s);

            for (auto it = importPaths.crbegin(); !(it == importPaths.crend()); it++) // != gives error C2593 on msvc
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

            object::objectPtr result;
            if (filePath.extension() == ".ez")
            {
                object::type::Function root = object::makeFunction();
                treeParser::parseFile(filePath.string(), root->node);
                object::objectPtr sourceFunction = object::makeObject(root);
                result = (*sourceFunction)(sourceFunction, std::move(args), &s);
            }
            else
            {
                result = externalLibrary::load(filePath);
            }
            (*module)[n::exports] = result;
            std::filesystem::current_path(oldDir);
            importPaths.pop_back();
            return result;
        }
        catch (...)
        {
            std::filesystem::current_path(oldDir);
            importPaths.pop_back();
            throw;
        }
    }
}

object::objectPtr interpreter::parse(object::objectPtr thisObj, object::type::Array &&args, stack *st)
{
    argsConvertibleGuard<std::string>(args);
    std::string temp = args[0]->getConverted<object::type::String>();
    object::type::Function root = object::makeFunction();
    treeParser::parseString(temp, root->node);
    return object::makeObject(root);
}

object::objectPtr interpreter::execute(object::objectPtr thisObj, object::type::Array &&args, stack *st)
{
    argsConvertibleGuard<std::string>(args);
    std::string temp = args[0]->getConverted<object::type::String>();
    object::type::Function root = object::makeFunction();
    treeParser::parseString(temp, root->node);
    try
    {
        auto &children = root->node.children();

        if (!children.empty())
        {
            const size_t size = children.size() - 1;
            for (size_t i = 0; i < size; i++)
                children[i].evaluateVoid(*st);
            auto evalRet = children.back().evaluate(*st);
            if (evalRet)
                return evalRet;
        }
    }
    catch (const object::objectPtr &ret)
    {
        if (ret)
            return ret;
    }
    catch (const objectException &e)
    {
        throw;
    }
    catch (const std::exception &e)
    {
        throw;
    }
    if (thisObj)
        return thisObj;
    return object::makeEmptyObject();
}

object::objectPtr constructorCaller(object::objectPtr thisObj, object::type::Array &&args, stack *st)
{
    auto newObj = object::makeEmptyObject();
    newObj->addProperty(n::prototype, (*thisObj)[n::classPrototype]);
    if ((*newObj)[n::prototype]->hasOwnProperty(n::constructor))
        (*(*newObj)[n::constructor])(newObj, std::move(args), st);
    if ((*newObj)[n::prototype]->hasOwnProperty(n::destructor))
        newObj->setDestructible();
    return newObj;
}

object::objectPtr interpreter::getStack(object::objectPtr thisObj, object::type::Array &&args, stack *st)
{
    auto obj = object::makeEmptyObject();
    st->copyToObject(obj, true);
    return obj;
}