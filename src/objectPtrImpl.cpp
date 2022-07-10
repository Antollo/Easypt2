#include "objectPtrImpl.h"
#include "nobject.h"
#include "console.h"
#include "core.h"

objectPtrImpl::objectPtrImpl(object *obj)
{
    _obj = obj;
    if (_obj != nullptr)
        _obj->_refCount = 1;
}

objectPtrImpl::objectPtrImpl(const objectPtrImpl &ptr)
{
    _obj = ptr._obj;
    if (_obj != nullptr)
        _obj->_refCount++;
}

objectPtrImpl &objectPtrImpl::operator=(const objectPtrImpl &ptr)
{
    if (_obj != nullptr)
    {
        _obj->_refCount--;
        if (_obj->_refCount == 0)
            deleter();
    }
    _obj = ptr._obj;
    if (_obj != nullptr)
        _obj->_refCount++;
    return *this;
}

objectPtrImpl::objectPtrImpl(objectPtrImpl &&ptr)
{
    _obj = ptr._obj;
    ptr._obj = nullptr;
}

objectPtrImpl &objectPtrImpl::operator=(objectPtrImpl &&ptr)
{
    if (_obj != nullptr)
    {
        _obj->_refCount--;
        if (_obj->_refCount == 0)
            deleter();
    }
    _obj = ptr._obj;
    ptr._obj = nullptr;
    return *this;
}

objectPtrImpl::~objectPtrImpl()
{
    if (_obj != nullptr)
    {
        _obj->_refCount--;
        if (_obj->_refCount == 0)
        {
            deleter();
        }
    }
}

void objectPtrImpl::deleter()
{
    if (_obj->isDestructible())
    {
        auto obj = _obj->read(n::prototype);
        if (obj)
            obj = obj->read(n::destructor);
        if (object::globalStack != nullptr && obj != nullptr && (!_obj->isOfType<object::type::NativeFunction>() || _obj->get<object::type::NativeFunction>() != constructorCaller))
        {
            _obj->_refCount++;
            try
            {
                (*obj)(*this, {}, nullptr);
            }
            catch (objectException &e)
            {
                auto eObj = e.getPtr();
                console::printStackTrace();
                if (eObj->isConvertible<std::string>())
                    console::error(eObj->getConverted<object::type::String>());
                else
                    console::error((std::string)e.what());
            }
            catch (std::exception &e)
            {
                console::printStackTrace();
                console::error((std::string)e.what());
            }
            catch (...)
            {
                console::printStackTrace();
                console::error("unknown error in destructor");
            }
            _obj->_refCount--;
            if (_obj->_refCount == 0)
            {
                object::reuse(_obj);
                _obj = nullptr;
            }
            return;
        }
    }
    object::reuse(_obj);
    _obj = nullptr;
}