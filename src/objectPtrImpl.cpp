#include "objectPtrImpl.h"
#include "nobject.h"
#include "console.h"

objectPtrImpl::objectPtrImpl(object *obj)
{
    _obj = obj;
    if (_obj != nullptr)
    {
        _refCount = new int;
        *_refCount = 1;
    }
    else
        _refCount = nullptr;
}

objectPtrImpl::objectPtrImpl(const objectPtrImpl &ptr)
{
    _obj = ptr._obj;
    _refCount = ptr._refCount;
    if (_refCount != nullptr) (*_refCount)++;
}

objectPtrImpl &objectPtrImpl::operator=(const objectPtrImpl &ptr)
{
    if (_refCount != nullptr)
    {
        (*_refCount)--;
        if (*_refCount == 0)
        {
            if (_obj != nullptr)
                delete _obj;
            delete _refCount;
        }
    }
    _obj = ptr._obj;
    _refCount = ptr._refCount;
    if (_refCount != nullptr) (*_refCount)++;
    return *this;
}

objectPtrImpl::objectPtrImpl(objectPtrImpl &&ptr)
{
    _obj = ptr._obj;
    _refCount = ptr._refCount;

    ptr._obj = nullptr;
    ptr._refCount = nullptr;
}

objectPtrImpl &objectPtrImpl::operator=(objectPtrImpl &&ptr)
{
    if (_refCount != nullptr)
    {
        (*_refCount)--;
        if (*_refCount == 0)
        {
            if (_obj != nullptr)
                delete _obj;
            delete _refCount;
        }
    }
    _obj = ptr._obj;
    _refCount = ptr._refCount;
    ptr._obj = nullptr;
    ptr._refCount = nullptr;
    return *this;
}

objectPtrImpl::~objectPtrImpl()
{
    if (_refCount != nullptr)
    {
        (*_refCount)--;
        if (*_refCount == 0)
        {
            if (_obj != nullptr)
                delete _obj;
            delete _refCount;
        }
    }
}

object &objectPtrImpl::operator*()
{
    //TODO remove check;
    if (_obj == nullptr)
        throw std::runtime_error("getting pointer to NULL");
    _obj->_thisPtr = this;
    return *_obj;
}

object *objectPtrImpl::operator->()
{
    //TODO remove check;
    if (_obj == nullptr)
        throw std::runtime_error("getting pointer to NULL");
    _obj->_thisPtr = this;
    return _obj;
}

const object &objectPtrImpl::operator*() const
{
    //TODO remove check;
    if (_obj == nullptr)
        throw std::runtime_error("getting pointer to NULL");
    _obj->_thisPtr = this;
    return *_obj;
}
const object *objectPtrImpl::operator->() const
{
    //TODO remove check;
    if (_obj == nullptr)
        throw std::runtime_error("getting pointer to NULL");
    _obj->_thisPtr = this;
    return _obj;
}