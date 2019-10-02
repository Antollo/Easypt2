#ifndef CORE_H_
#define CORE_H_

#include "nobject.h"

object::objectPtr import(object::objectPtr thisObj, object::arrayType &&args, stack *st);

#endif /* !CORE_H_ */
