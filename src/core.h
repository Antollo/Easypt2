#ifndef CORE_H_
#define CORE_H_

#include "nobject.h"

object::objectPtr import(object::objectPtr thisObj, object::arrayType &&args, stack *st);
object::objectPtr parse(object::objectPtr thisObj, object::arrayType &&args, stack *st);
object::objectPtr transpile(object::objectPtr thisObj, object::arrayType &&args, stack *st);
object::objectPtr constructorCaller(object::objectPtr thisObj, object::arrayType &&args, stack *st);
object::objectPtr getStack(object::objectPtr thisObj, object::arrayType &&args, stack *st);

#endif /* !CORE_H_ */
