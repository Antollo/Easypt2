#include "objectPtrImpl.h"
#include "nobject.h"
#include "console.h"
#include "core.h"

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
    if (_refCount != nullptr)
        (*_refCount)++;
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
    if (_refCount != nullptr)
        (*_refCount)++;
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

bool objectPtrImpl::operator==(std::nullptr_t) const
{
    if (_obj == nullptr)
        return true;
    return false;
}

bool objectPtrImpl::operator!=(std::nullptr_t) const
{
    if (_obj != nullptr)
        return true;
    return false;
}

objectPtrImpl::~objectPtrImpl()
{
    if (_refCount != nullptr)
    {
        (*_refCount)--;
        if (*_refCount == 0)
        {
            if (_obj != nullptr)
            {
                auto obj = _obj->read("prototype"_n);
                if (obj) obj = obj->read("destructor"_n);
                if (object::globalStack != nullptr && obj != nullptr && (!_obj->isOfType<object::nativeFunctionType>() || _obj->get<object::nativeFunctionType>() != constructorCaller))
                {
                    (*_refCount)++;
                    try
                    {
                        (*obj)(*this, {}, nullptr);
                    }
                    catch (objectException &e)
                    {
                        auto obj = e.getPtr();
                        console::stackTrace();
                        if (obj->isConvertible<std::string>())
                            console::error(obj->getConverted<std::string>());
                        else
                            console::error((std::string)e.what());
                    }
                    catch (std::exception &e)
                    {
                        console::stackTrace();
                        console::error((std::string)e.what());
                    }
                    catch (...)
                    {
                        console::stackTrace();
                        console::error("Unknown error in destructor");
                    }
                    (*_refCount)--;
                    if (*_refCount == 0)
                    {
                        delete _obj;
                        delete _refCount;
                        _obj = nullptr;
                        _refCount = nullptr;
                    }
                }
                else
                {
                    delete _obj;
                    delete _refCount;
                    _obj = nullptr;
                    _refCount = nullptr;
                }
            }
            else
            {
                delete _refCount;
                _refCount = nullptr;
            }
        }
    }
}

object &objectPtrImpl::operator*()
{
    //TODO remove check;
    //if (_obj == nullptr)
    //    throw std::runtime_error("getting pointer to NULL");
    _obj->_thisPtr = this;
    return *_obj;
}

object *objectPtrImpl::operator->()
{
    //TODO remove check;
    //if (_obj == nullptr)
    //    throw std::runtime_error("getting pointer to NULL");
    _obj->_thisPtr = this;
    return _obj;
}

const object &objectPtrImpl::operator*() const
{
    //TODO remove check;
    //if (_obj == nullptr)
    //    throw std::runtime_error("getting pointer to NULL");
    _obj->_thisPtr = this;
    return *_obj;
}
const object *objectPtrImpl::operator->() const
{
    //TODO remove check;
    //if (_obj == nullptr)
    //    throw std::runtime_error("getting pointer to NULL");
    _obj->_thisPtr = this;
    return _obj;
}