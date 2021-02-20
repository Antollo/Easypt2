#include "objectPtrImpl.h"
#include "nobject.h"
#include "console.h"
#include "core.h"


objectPtrImpl::objectPtrImpl(object *obj)
{
    _obj = obj;
    if (_obj != nullptr)
    {
        _refCount = static_cast<refCountType*>(memory.allocate());
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
                object::reuse(_obj);
            memory.deallocate(_refCount);
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
                object::reuse(_obj);
            memory.deallocate(_refCount);
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
            {
                auto obj = _obj->read(n::prototype);
                if (obj) obj = obj->read(n::destructor);
                if (object::globalStack != nullptr && obj != nullptr && (!_obj->isOfType<object::type::NativeFunction>() || _obj->get<object::type::NativeFunction>() != constructorCaller))
                {
                    (*_refCount)++;
                    try
                    {
                        (*obj)(*this, {}, nullptr);
                    }
                    catch (objectException &e)
                    {
                        auto eObj = e.getPtr();
                        console::stackTrace();
                        if (eObj->isConvertible<std::string>())
                            console::error(eObj->getConverted<std::string>());
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
                        console::error("unknown error in destructor");
                    }
                    (*_refCount)--;
                    if (*_refCount == 0)
                    {
                        object::reuse(_obj);
                        memory.deallocate(_refCount);
                        _obj = nullptr;
                        _refCount = nullptr;
                    }
                }
                else
                {
                    object::reuse(_obj);
                    memory.deallocate(_refCount);
                    _obj = nullptr;
                    _refCount = nullptr;
                }
            }
            else
            {
                memory.deallocate(_refCount);
                _refCount = nullptr;
            }
        }
    }
}