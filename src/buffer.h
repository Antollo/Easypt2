#ifndef BUFFER_H_
#define BUFFER_H_

#include <utility>
#include <algorithm>
#include <string>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <cstddef>
#include <span>

#ifdef assert
#undef assert
#endif

#if __STDCPP_DEFAULT_NEW_ALIGNMENT__ < 16
#error "Default new alignment is less than 16"
#endif

/// Represents an array of bytes viewed as simple data types. It corresponds to Buffer Easypt type.
class buffer
{
public:
    enum class type
    {
        Int8,
        Int16,
        Int32,
        Int64,
        Uint8,
        Uint16,
        Uint32,
        Uint64,
        Float,
        Double,
        Unknown
    };

    buffer() = default;
    buffer(const buffer &rhs) = delete;
    buffer(buffer &&rhs)
    {
        if (rhs._data)
        {
            _data = rhs._data;
            _length = rhs._length;
            _t_size = rhs._t_size;
            _t = rhs._t;
            rhs._data = nullptr;
            rhs._length = 0;
            rhs._t_size = 0;
            rhs._t = type::Unknown;
        }
        else
        {
            _data = nullptr;
            _length = 0;
            _t_size = 0;
            _t = type::Unknown;
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
            _length = rhs._length;
            _t_size = rhs._t_size;
            _t = rhs._t;
            rhs._data = nullptr;
            rhs._length = 0;
            rhs._t_size = 0;
            rhs._t = type::Unknown;
        }
        else
        {
            _data = nullptr;
            _length = 0;
            _t_size = 0;
            _t = type::Unknown;
        }
        return *this;
    }

    buffer(const std::string &str)
    {
        allocate<char>(str.size());
        std::memcpy(begin(), str.c_str(), str.size());
    }

    buffer(const char *str)
    {
        size_t _n_length = std::strlen(str);
        allocate<char>(_n_length);
        std::memcpy(begin(), str, _n_length);
    }

    ~buffer()
    {
        if (_data)
            delete[] _data;
    }

    operator std::string() const
    {
        return std::string(reinterpret_cast<const char *>(begin()), reinterpret_cast<const char *>(end()));
    }

    bool operator==(const buffer &rhs) const
    {
        return std::equal(reinterpret_cast<const char *>(begin()), reinterpret_cast<const char *>(end()),
                          reinterpret_cast<const char *>(rhs.begin()), reinterpret_cast<const char *>(rhs.end()));
    }

    template <class U>
    void allocate(size_t _n_length)
    {
        using T = std::remove_cv_t<U>;
        if (_data)
            delete[] _data;
        _length = _n_length;
        _t_size = sizeof(T);
        _data = new std::byte[_n_length * _t_size]();

        if constexpr (std::is_same_v<T, int8_t> || std::is_same_v<T, char> || std::is_same_v<T, std::byte>)
            _t = type::Int8;
        else if constexpr (std::is_same_v<T, int16_t>)
            _t = type::Int16;
        else if constexpr (std::is_same_v<T, int32_t>)
            _t = type::Int32;
        else if constexpr (std::is_same_v<T, int64_t>)
            _t = type::Int64;
        else if constexpr (std::is_same_v<T, uint8_t> || std::is_same_v<T, unsigned char>)
            _t = type::Uint8;
        else if constexpr (std::is_same_v<T, uint16_t>)
            _t = type::Uint16;
        else if constexpr (std::is_same_v<T, uint32_t>)
            _t = type::Uint32;
        else if constexpr (std::is_same_v<T, uint64_t>)
            _t = type::Uint64;
        else if constexpr (std::is_same_v<T, float>)
            _t = type::Float;
        else if constexpr (std::is_same_v<T, double>)
            _t = type::Double;
        else
            _t = type::Unknown;
    }

    void reallocate(size_t _n_length)
    {
        std::byte *temp = new std::byte[_n_length * _t_size]();
        if (_data)
        {
            std::memcpy(temp, _data, std::min(_n_length * _t_size, _length * _t_size));
            delete[] _data;
        }
        _length = _n_length;
        _data = temp;
    }

    template <class T>
    void assert(size_t index)
    {
        if ((index + 1) * sizeof(T) <= _length * _t_size)
            return;
        throw std::runtime_error("buffer index out of bounds: " + std::to_string(index * sizeof(T)) + " >= " + std::to_string(_length * _t_size));
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
            _length = 0;
            _t_size = 0;
        }
    }

    void *begin()
    {
        return _data;
    }

    void *end()
    {
        return reinterpret_cast<char *>(_data) + size();
    }

    const void *begin() const
    {
        return _data;
    }

    const void *end() const
    {
        return reinterpret_cast<char *>(_data) + size();
    }

    template <class T>
    auto asSpan()
    {
        return std::span<T, std::dynamic_extent>(reinterpret_cast<T *>(begin()), reinterpret_cast<T *>(end()));
    }

    template <class T>
    auto asSpan() const
    {
        return std::span<const T, std::dynamic_extent>(reinterpret_cast<const T *>(begin()), reinterpret_cast<const T *>(end()));
    }

    size_t size() const { return _length * _t_size; }
    size_t length() const { return _length; }
    type elementType() const { return _t; }
    bool empty() const { return _length == 0; }

private:
    std::byte *_data = nullptr;
    size_t _length = 0;
    uint8_t _t_size = 0;
    type _t = type::Unknown;
};

#endif