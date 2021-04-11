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
    enum class type {
        Int8,
        Int16,
        Int32,
        Int64,
        Float,
        Double,
        Unknown
    };

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
            _t = rhs._t;
            rhs._data = nullptr;
            rhs._size = 0;
            rhs._t_size = 0;
            rhs._t = type::Unknown;
        }
        else
        {
            _data = nullptr;
            _size = 0;
            _t_size = 0;
            _t = type::Unknown;
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
            _t = rhs._t;
            rhs._data = nullptr;
            rhs._size = 0;
            rhs._t_size = 0;
            rhs._t = type::Unknown;
        }
        else
        {
            _data = nullptr;
            _size = 0;
            _t_size = 0;
            _t = type::Unknown;
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
        _data = std::calloc(_n_size * sizeof(T) + 15, 1);
        if constexpr(std::is_same_v<T, int8_t> || std::is_same_v<T, uint8_t>)
            _t = type::Int8;
        else if constexpr(std::is_same_v<T, int16_t> || std::is_same_v<T, uint16_t>)
            _t = type::Int16;
        else if constexpr(std::is_same_v<T, int32_t> || std::is_same_v<T, uint32_t>)
            _t = type::Int32;
        else if constexpr(std::is_same_v<T, int64_t> || std::is_same_v<T, uint64_t>)
            _t = type::Int64;
        else if constexpr(std::is_same_v<T, float>)
            _t = type::Float;
        else if constexpr(std::is_same_v<T, double>)
            _t = type::Double;
        else
            _t = type::Unknown;
    }
    //template <class T>
    void reallocate(size_t _n_size)
    {
        void *temp = std::calloc(_n_size * _t_size + 15, 1);
        if (_data)
        {
            std::memcpy(temp, _data, std::min(_n_size * _t_size + 15, static_cast<size_t>(_size * _t_size + 15)));
            std::free(_data);
        }
        _size = _n_size;
        //_t_size = sizeof(T);
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
    size_t length() const { return _size; }
    type elementType() const { return _t; }
    bool empty() const { return _size == 0; }

private:
    static void *align(const void *ptr)
    {
        return reinterpret_cast<void*>(((uintptr_t)ptr + 15) & ~(uintptr_t)0x0F);
    }
    void *_data = nullptr;
    size_t _size = 0;
    uint8_t _t_size = 0;
    type _t;
};

#endif