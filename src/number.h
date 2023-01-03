#ifndef NUMBER_H_
#define NUMBER_H_

#include <variant>
#include <utility>
#include <string>

using namespace std::rel_ops;

class number
{
public:
    number() = default;
    number(int8_t n) : index(intIndex), i(static_cast<intType>(n)){};
    number(uint8_t n) : index(intIndex), i(static_cast<intType>(n)){};
    number(int16_t n) : index(intIndex), i(static_cast<intType>(n)){};
    number(uint16_t n) : index(intIndex), i(static_cast<intType>(n)){};
    number(int32_t n) : index(intIndex), i(static_cast<intType>(n)){};
    number(uint32_t n) : index(intIndex), i(static_cast<intType>(n)){};
    number(int64_t n) : index(intIndex), i(static_cast<intType>(n)){};
    number(uint64_t n) : index(intIndex), i(static_cast<intType>(n)){};

    number(const bool n) : index(intIndex), i(static_cast<intType>(n)){};

    number(const double n);

    number(const std::string &n);
    number(const char *n) : number(std::string(n)){};

    number(number &&) = default;
    number &operator=(number &&) = default;
    number(const number &) = default;
    number &operator=(const number &) = default;

    number &operator+=(const number &x);
    number &operator-=(const number &x);
    number &operator*=(const number &x);
    number &operator/=(const number &x);
    number &operator%=(const number &x);

    number operator+(const number &x) const;
    number operator-(const number &x) const;
    number operator*(const number &x) const;
    number operator/(const number &x) const;
    number operator%(const number &x) const;

    bool operator==(const number &x) const;
    bool operator<(const number &x) const;

    template <class F>
    auto visit(F &&fun) const
    {
        if (index == intIndex)
            return fun(i);
        else
            return fun(f);
    }

    number operator++(int);
    number operator--(int);
    number &operator++();
    number &operator--();
    number operator-() const;

    number toInteger() const;
    number toFloatingPoint() const;

    number ceil() const;
    number floor() const;

    template <class T, class U = std::enable_if_t<std::is_arithmetic_v<T>>>
    explicit operator T() const
    {
        if (index == intIndex)
            return i;
        else
            return f;
    }

    explicit operator std::string() const
    {
        if (index == intIndex)
            return std::to_string(i);
        else
            return std::to_string(f);
    }

    friend std::ostream &operator<<(std::ostream &, const number &);
    friend std::istream &operator>>(std::istream &, number &);

    using intType = int64_t;
    using floatType = double;

private:
    union
    {
        intType i;
        floatType f;
    };

    int8_t index = 0;

    static constexpr int8_t intIndex = 0;
    static constexpr int8_t floatIndex = 1;
};

std::ostream &operator<<(std::ostream &s, const number &x);
std::istream &operator>>(std::istream &s, number &x);

number operator"" _n(unsigned long long n);
number operator"" _n(long double n);

#endif /* !NUMBER_H_ */
