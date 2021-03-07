#ifndef OBJECTPTRIMPL_H_
#define OBJECTPTRIMPL_H_

#include <stdexcept>
#include <type_traits>
#include "allocator.h"

template <class T>
using remove_cref_t = std::remove_const_t<std::remove_reference_t<T>>;

class object;

class objectPtrImpl
{

public:
    objectPtrImpl(object *obj = nullptr);
    objectPtrImpl(const objectPtrImpl &ptr);
    objectPtrImpl &operator=(const objectPtrImpl &ptr);
    objectPtrImpl(objectPtrImpl &&ptr);
    objectPtrImpl &operator=(objectPtrImpl &&ptr);
    bool operator==(std::nullptr_t) const { return _obj == nullptr; };
    bool operator!=(std::nullptr_t) const { return _obj != nullptr; };
    bool operator==(const objectPtrImpl &ptr) const { return _obj == ptr._obj; };
    bool operator!=(const objectPtrImpl &ptr) const { return _obj != ptr._obj; };
    bool operator==(objectPtrImpl &&ptr) const { return _obj == ptr._obj; };
    bool operator!=(objectPtrImpl &&ptr) const { return _obj != ptr._obj; };
    ~objectPtrImpl();

    operator bool() const { return _obj != nullptr; }

    object &operator*() { return *_obj; }
    object *operator->() { return _obj; }
    const object &operator*() const { return *_obj; }
    const object *operator->() const { return _obj; }
    object *get() { return _obj; }
    const object *get() const { return _obj; }

    template <class T>
    remove_cref_t<T> getConverted() const; // defined in "nobject.h"

private:
    using refCountType = uint_fast16_t;
    static inline allocatorBuffer<sizeof(refCountType)> memory;
    object *_obj;
    refCountType *_refCount;
};

#endif /* !OBJECTPTRIMPL_H_ */
