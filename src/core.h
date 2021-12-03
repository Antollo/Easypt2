#ifndef CORE_H_
#define CORE_H_

#include "nobject.h"

/// Class that handle Easypt import function and modules
class modules
{
public:
    static object::objectPtr import(object::objectPtr thisObj, object::type::Array &&args, stack *st);
    static object::objectPtr getImportPaths(object::objectPtr thisObj, object::type::Array &&args, stack *st);

    static void init(objectPtrImpl newModulesPtr)
    {
        modulesPtr = newModulesPtr;
        imported = &*modulesPtr->_properties;
    }

    static void fini()
    {
        imported->clear();
        imported = nullptr;
        modulesPtr = nullptr;
    }

private:
    static inline object::propertiesType *imported = nullptr;
    static inline object::objectPtr modulesPtr = nullptr;
    static inline std::vector<std::filesystem::path> importPaths = {
        osDependant::getExecutablePath().parent_path().parent_path(),
        osDependant::getExecutablePath().parent_path(),
        std::filesystem::current_path()};
};

namespace interpreter
{
    object::objectPtr parse(object::objectPtr thisObj, object::type::Array &&args, stack *st);
    object::objectPtr execute(object::objectPtr thisObj, object::type::Array &&args, stack *st);
    object::objectPtr getStack(object::objectPtr thisObj, object::type::Array &&args, stack *st);
};

/// Objects of Class Easypt type store constructorCaller as internal value
object::objectPtr constructorCaller(object::objectPtr thisObj, object::type::Array &&args, stack *st);

#endif /* !CORE_H_ */
