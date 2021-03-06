#ifndef BUFFER_H_
#define BUFFER_H_

#include <utility>
#include <algorithm>
#include <string>
#include <cstring>
#include <stdexcept>

class buffer
{
public:
    buffer() = default;
    buffer(const buffer &rhs) = delete;
    buffer(buffer &&rhs)
    {
        if (rhs._data)
        {
            _data = rhs._data;
            _size = rhs._size;
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
            delete[] _data;
        if (rhs._data)
        {
            _data = rhs._data;
            _size = rhs._size;
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
            delete[] _data;
    }
    bool operator==(const buffer &rhs) const
    {
        return std::equal(begin(), end(), rhs.begin(), rhs.end());
    }
    template <class T>
    void allocate(size_t _n_size)
    {
        _size = _n_size;
        _t_size = sizeof(T);
        _data = reinterpret_cast<int8_t *>(new T[_size]);
    }
    template <class T>
    void reallocate(size_t _n_size)
    {
        T *temp = new T[_n_size];
        if (_data)
        {
            std::memcpy(temp, _data, std::min(_n_size * sizeof(T), static_cast<size_t>(_size * _t_size)));
            delete[] _data;
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
        return *(reinterpret_cast<T *>(_data) + index);
    }
    template <class T>
    const T &get(size_t index) const
    {
        return *(reinterpret_cast<const T *>(_data) + index);
    }
    void clear()
    {
        if (_data)
        {
            delete[] _data;
            _data = nullptr;
            _size = 0;
            _t_size = 0;
        }
    }
    int8_t *begin()
    {
        return _data;
    }
    int8_t *end()
    {
        return _data + _size * _t_size;
    }
    const int8_t *begin() const
    {
        return _data;
    }
    const int8_t *end() const
    {
        return _data + _size * _t_size;
    }
    size_t size() const { return _size; }
    bool empty() const { return _size == 0; }

private:
    int8_t *_data = nullptr;
    int32_t _size = 0;
    int8_t _t_size = 0;
};

#endif