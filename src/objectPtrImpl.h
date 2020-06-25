#ifndef OBJECTPTRIMPL_H_
#define OBJECTPTRIMPL_H_

#include <stdexcept>
#include "allocator.h"

class object;

class objectPtrImpl
{

public:
    objectPtrImpl(object *obj = nullptr);
    objectPtrImpl(const objectPtrImpl &ptr);
    objectPtrImpl &operator=(const objectPtrImpl &ptr);
    objectPtrImpl(objectPtrImpl &&ptr);
    objectPtrImpl &operator=(objectPtrImpl &&ptr);
    bool operator==(std::nullptr_t) const;
    bool operator!=(std::nullptr_t) const;
    ~objectPtrImpl();

    inline operator bool() const { return _obj != nullptr; }
    object &operator*();
    object *operator->();
    const object &operator*() const;
    const object *operator->() const;
    inline object *get() { return _obj; }

private:
    static allocatorBuffer<sizeof(int)> memory;
    object *_obj;
    int *_refCount;
};

#endif /* !OBJECTPTRIMPL_H_ */
