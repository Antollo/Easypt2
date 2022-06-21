#include "CLambda.h"

#define contextType void
#define objectPtrPtr void*
#define closureType closure_type(objectPtrPtr, (contextType *))

void withContext(closureType closure, contextType *context);