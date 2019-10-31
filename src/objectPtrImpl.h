#ifndef OBJECTPTRIMPL_H_
#define OBJECTPTRIMPL_H_

#include <stdexcept>

class object;

class objectPtrImpl
{

public:
    objectPtrImpl(object *obj = nullptr);
    objectPtrImpl(const objectPtrImpl &ptr);
    objectPtrImpl &operator=(const objectPtrImpl &ptr);
    objectPtrImpl(objectPtrImpl &&ptr);
    objectPtrImpl &operator=(objectPtrImpl &&ptr);
    ~objectPtrImpl();

    inline operator bool() const { return _obj != nullptr; }
    object &operator*();
    object *operator->();
    const object &operator*() const;
    const object *operator->() const;
    inline object *get() { return _obj; }

private:
    object *_obj;
    int *_refCount;
};

#endif /* !OBJECTPTRIMPL_H_ */
