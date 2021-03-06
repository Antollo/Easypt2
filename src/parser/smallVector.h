#include <utility>
#include <algorithm>

template <class T>
class smallVector
{
public:
    smallVector() = default;
    smallVector(const smallVector &rhs) = delete;
    smallVector(smallVector &&rhs)
    {
        if (rhs._data)
        {
            _data = rhs._data;
            _size = rhs._size;
            rhs._data = nullptr;
            rhs._size = 0;
        }
        else
        {
            _data = nullptr;
            _size = 0;
        }
    }
    smallVector &operator=(const smallVector &rhs) = delete;
    smallVector &operator=(smallVector &&rhs)
    {
        if (_data)
            delete[] _data;
        if (rhs._data)
        {
            _data = rhs._data;
            _size = rhs._size;
            rhs._data = nullptr;
            rhs._size = 0;
        }
        else
        {
            _data = nullptr;
            _size = 0;
        }
        return *this;
    }
    ~smallVector()
    {
        if (_data)
            delete[] _data;
    }
    bool operator==(const smallVector &rhs) const
    {
        return std::equal(begin(), end(), rhs.begin(), rhs.end());
    }
    template <class U>
    void push_back(U element)
    {
        T *temp = new T[_size + 1];
        if (_data)
        {
            for (size_t i = 0; i < _size; i++)
                temp[i] = std::move(_data[i]);
            delete[] _data;
        }
        _data = temp;
        _data[_size] = std::forward<U>(element);
        _size++;
    }
    void clear()
    {
        if (_data)
        {
            delete[] _data;
            _data = nullptr;
            _size = 0;
        }
    }
    T &operator[](size_t index)
    {
        return _data[index];
    }
    const T &operator[](size_t index) const
    {
        return _data[index];
    }
    T *begin()
    {
        return _data;
    }
    T *end()
    {
        return _data + _size;
    }
    const T *begin() const
    {
        return _data;
    }
    const T *end() const
    {
        return _data + _size;
    }
    T &front()
    {
        return _data[0];
    }
    T &back()
    {
        return _data[_size - 1];
    }
    const T &front() const
    {
        return _data[0];
    }
    const T &back() const
    {
        return _data[_size - 1];
    }
    size_t size() const { return _size; }
    bool empty() const { return _size == 0; }

private:
    T *_data = nullptr;
    size_t _size = 0;
};