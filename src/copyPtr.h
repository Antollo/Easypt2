#ifndef B31C7DDE_1C97_4375_909E_6820E5346341
#define B31C7DDE_1C97_4375_909E_6820E5346341

#include <stdexcept>
#include <type_traits>
#include "allocator.h"

template <class T>
class copyPtr
{
public:
    copyPtr() : _data(reinterpret_cast<T *>(memory.allocate()))
    {
        new (_data) T(); 
    }
    copyPtr(copyPtr &&rhs)
    {
        _data = rhs._data;
        rhs._data = nullptr;
    }
    copyPtr(const copyPtr &rhs) : _data(reinterpret_cast<T *>(memory.allocate()))
    {
        *_data = *rhs._data;
    }
    copyPtr &operator=(copyPtr &&rhs)
    {
        if (_data)
        {
            _data->~T();
            memory.deallocate(_data);
        }
        _data = rhs._data;
        rhs._data = nullptr;
        return *this;
    }
    copyPtr &operator=(const copyPtr &rhs)
    {
        *_data = *rhs._data;
        return *this;
    }
    ~copyPtr()
    {
        if (_data)
        {
            _data->~T();
            memory.deallocate(_data);
        }
    }

    operator T&() { return *_data; }
    operator const T&() const { return *_data; }
    operator bool() const { return _data; }

    T *operator->() { return _data; }
    T &operator*() { return *_data; }
    const T *operator->() const { return _data; }
    const T &operator*() const { return *_data; }
    T *get() { return _data; }
    const T *get() const { return _data; }

private:
    T *_data;
    static inline allocatorBuffer<sizeof(T)> memory;
};

#endif /* B31C7DDE_1C97_4375_909E_6820E5346341 */
