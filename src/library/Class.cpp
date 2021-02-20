#include "../library.h"

void Class::init(stack *st)
{
    object::objectPtr Class = insertObject(n::Class, constructorCaller);
    (*object::classPrototype)[n::prototype] = object::functionPrototype;

    (*Class)[n::classPrototype] = object::classPrototype;

    addFunctionL(object::classPrototype, n::constructor, {
        argsConvertibleGuard<nullptr_t>(args);
        thisObj->setType<object::type::NativeFunction>();
        thisObj->get<object::type::NativeFunction&>() = constructorCaller;
        thisObj->addProperty(n::classPrototype, args[0]);
        // TODO: 2 following lines fixes prototype destructor calling
        // in classPrototype (inherited destructor)
        args[0]->setType<object::type::NativeFunction>();
        args[0]->get<object::type::NativeFunction&>() = constructorCaller;
        return thisObj;
    });
}