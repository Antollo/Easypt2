#ifndef BUFFER_H_
#define BUFFER_H_

#include <utility>
#include <algorithm>
#include <string>
#include <cstring>
#include <cstdlib>
#include <stdexcept>

class buffer
{
public:
    buffer() = default;
    buffer(const buffer &rhs) = delete;
    buffer(const std::string &str)
    {
        allocate<char>(str.size());
        std::memcpy(begin(), str.c_str(), str.size());
    }
    operator std::string() const
    {
        return std::string(reinterpret_cast<const char*>(begin()), reinterpret_cast<const char*>(end()));
    }
    buffer(buffer &&rhs)
    {
        if (rhs._data)
        {
            _data = rhs._data;
            _size = rhs._size;
            _t_size = rhs._t_size;
            rhs._data = nullptr;
            rhs._size = 0;
            rhs._t_size = 0;
        }
        else
        {
            _data = nullptr;
            _size = 0;
            _t_size = 0;
        }
    }
    buffer &operator=(const buffer &rhs) = delete;
    buffer &operator=(buffer &&rhs)
    {
        if (_data)
            std::free(_data);
        if (rhs._data)
        {
            _data = rhs._data;
            _size = rhs._size;
            _t_size = rhs._t_size;
            rhs._data = nullptr;
            rhs._size = 0;
            rhs._t_size = 0;
        }
        else
        {
            _data = nullptr;
            _size = 0;
            _t_size = 0;
        }
        return *this;
    }
    ~buffer()
    {
        if (_data)
            std::free(_data);
    }
    bool operator==(const buffer &rhs) const
    {
        return std::equal(reinterpret_cast<const char*>(begin()), reinterpret_cast<const char*>(end()),
                          reinterpret_cast<const char*>(rhs.begin()), reinterpret_cast<const char*>(rhs.end()));
    }
    template <class T>
    void allocate(size_t _n_size)
    {
        if (_data)
            std::free(_data);
        _size = _n_size;
        _t_size = sizeof(T);
        _data = std::malloc(_n_size * sizeof(T) + 15);
    }
    template <class T>
    void reallocate(size_t _n_size)
    {
        void *temp = std::malloc(_n_size * sizeof(T) + 15);
        if (_data)
        {
            std::memcpy(temp, _data, std::min(_n_size * sizeof(T) + 15, static_cast<size_t>(_size * _t_size + 15)));
            std::free(_data);
        }
        _size = _n_size;
        _t_size = sizeof(T);
        _data = temp;
    }
    template <class T>
    void assert(size_t index)
    {
        if ((index + 1) * sizeof(T) <= _size * _t_size)
            return;
        throw std::runtime_error(std::to_string((index + 1) * sizeof(T)) + " <= " + std::to_string(_size * _t_size));
    }
    template <class T>
    T &get(size_t index)
    {
        return *(reinterpret_cast<T *>(align(_data)) + index);
    }
    template <class T>
    const T &get(size_t index) const
    {
        return *(reinterpret_cast<const T *>(align(_data)) + index);
    }
    void clear()
    {
        if (_data)
        {
            std::free(_data);
            _data = nullptr;
            _size = 0;
            _t_size = 0;
        }
    }
    void *begin()
    {
        return align(_data);
    }
    void *end()
    {
        return reinterpret_cast<char*>(align(_data)) + _size * _t_size;
    }
    const void *begin() const
    {
        return align(_data);
    }
    const void *end() const
    {
        return reinterpret_cast<char*>(align(_data)) + _size * _t_size;
    }
    size_t size() const { return _size * _t_size; }
    bool empty() const { return _size == 0; }

private:
    static void *align(const void *ptr)
    {
        return reinterpret_cast<void*>(((uintptr_t)ptr + 15) & ~(uintptr_t)0x0F);
    }
    void *_data = nullptr;
    size_t _size = 0;
    int _t_size = 0;
};

#endif