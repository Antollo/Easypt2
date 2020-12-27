#ifndef CORE_H_
#define CORE_H_

#include "nobject.h"

class Import
{
public:
    static object::objectPtr import(object::objectPtr thisObj, object::arrayType &&args, stack *st);
    static object::objectPtr getImportPaths(object::objectPtr thisObj, object::arrayType &&args, stack *st);
    static void fini() { imported.clear(); }

private:
    static inline std::unordered_map<std::string, object::objectPtr> imported;
    static inline std::vector<std::filesystem::path> importPaths = {
        getExecutablePath().parent_path().parent_path(),
        getExecutablePath().parent_path(),
        std::filesystem::current_path()};
};

object::objectPtr ez_parse(object::objectPtr thisObj, object::arrayType &&args, stack *st);
object::objectPtr execute(object::objectPtr thisObj, object::arrayType &&args, stack *st);
object::objectPtr getStack(object::objectPtr thisObj, object::arrayType &&args, stack *st);
object::objectPtr constructorCaller(object::objectPtr thisObj, object::arrayType &&args, stack *st);

#endif /* !CORE_H_ */
