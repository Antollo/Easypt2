#include "../library.h"

void Array::init(stack *st)
{
    auto Array = insertObject("Array"_n, constructorCaller);

addFunctionL(Array, "readOperator"_n, {
        return makeObject(args);
    });

    (*Array)["prototype"_n] = object::arrayPrototype;

    addFunctionL(object::arrayPrototype, "constructor"_n, {
        thisObj->setType<object::arrayType>();
        return thisObj;
    });

    addFunctionL(object::arrayPrototype, "readOperator"_n, {
        argsConvertibleGuard<number>(args);
        return thisObj->get<const object::arrayType>().at(static_cast<int>(args[0]->getConverted<number>()));
    });
    
    addFunctionL(object::arrayPrototype, "length"_n, {
        return makeObject(static_cast<number>(static_cast<int>(thisObj->get<const object::arrayType>().size())));
    });
}