#include <cmath>
#include <stdexcept>
#include <iostream>
#include <limits>
#include "number.h"

number::number(double n)
{
    if (std::trunc(n) == n && n >= std::numeric_limits<intType>::min() && n <= std::numeric_limits<intType>::max())
    {
        index = intIndex;
        i = static_cast<intType>(n);
    }
    else
    {
        index = floatIndex;
        f = n;
    }
};

number::number(const std::string &n)
{
    try
    {
        if (n.find('.') == std::string::npos)
        {
            index = intIndex;
            i = std::stoll(n, 0, 0);
        }
        else
            *this = number(std::stod(n));
    }
    catch (const std::invalid_argument &)
    {
        throw std::runtime_error("cannot convert \"" + n + "\" to number");
    }
    catch (const std::out_of_range &)
    {
        throw std::runtime_error("cannon convert \"" + n + "\" to number, converted value would overflow the number");
    }
};

number &number::operator+=(const number &x)
{
    if (index == intIndex && x.index == intIndex)
    {
        i += x.i;
        return *this;
    }
    else
        return *this = *this + x;
}
number &number::operator-=(const number &x)
{
    if (index == intIndex && x.index == intIndex)
    {
        i -= x.i;
        return *this;
    }
    else
        return *this = *this - x;
}
number &number::operator*=(const number &x)
{
    if (index == intIndex && x.index == intIndex)
    {
        i *= x.i;
        return *this;
    }
    else
        return *this = *this * x;
}
number &number::operator/=(const number &x)
{
    return *this = *this / x;
}
number &number::operator%=(const number &x)
{
    if (index == intIndex && x.index == intIndex)
    {
        i %= x.i;
        return *this;
    }
    else
    return *this = *this % x;
}

number number::operator+(const number &x) const
{
    if (index == intIndex)
    {
        if (x.index == intIndex)
            return i + x.i;
        else
            return static_cast<floatType>(i) + x.f;
    }
    else
    {
        if (x.index == intIndex)
            return f + static_cast<floatType>(x.i);
        else
            return f + x.f;
    }
}
number number::operator-(const number &x) const
{
    if (index == intIndex)
    {
        if (x.index == intIndex)
            return i - x.i;
        else
            return static_cast<floatType>(i) - x.f;
    }
    else
    {
        if (x.index == intIndex)
            return f - static_cast<floatType>(x.i);
        else
            return f - x.f;
    }
}

number number::operator*(const number &x) const
{
    if (index == intIndex)
    {
        if (x.index == intIndex)
            return i * x.i;
        else
            return static_cast<floatType>(i) * x.f;
    }
    else
    {
        if (x.index == intIndex)
            return f * static_cast<floatType>(x.i);
        else
            return f * static_cast<floatType>(x.f);
    }
}
number number::operator/(const number &x) const
{
    if ((index == intIndex && x.i == 0) || (index == floatIndex && x.f == 0.f))
        throw std::runtime_error("division by zero");

    if (index == intIndex)
    {
        if (x.index == intIndex)
            return static_cast<floatType>(i) / static_cast<floatType>(x.i);
        else
            return static_cast<floatType>(i) / x.f;
    }
    else
    {
        if (x.index == intIndex)
            return f / static_cast<floatType>(x.i);
        else
            return f / x.f;
    }
}
number number::operator%(const number &x) const
{
    if ((index == intIndex && x.i == 0) || (index == floatIndex && x.f == 0.f))
        throw std::runtime_error("modulo by zero");

    if (index == intIndex)
    {
        if (x.index == intIndex)
            return i % x.i;
        else
            return i % static_cast<intType>(x.f);
    }
    else
    {
        if (x.index == intIndex)
            return static_cast<intType>(f) % x.i;
        else
            return static_cast<intType>(f) % static_cast<intType>(x.f);
    }
}

bool number::operator==(const number &x) const
{
    if (index == intIndex)
    {
        if (x.index == intIndex)
            return i == x.i;
        else
            return i == x.f;
    }
    else
    {
        if (x.index == intIndex)
            return f == x.i;
        else
            return f == x.f;
    }
}
bool number::operator<(const number &x) const
{
    if (index == intIndex)
    {
        if (x.index == intIndex)
            return i < x.i;
        else
            return i < x.f;
    }
    else
    {
        if (x.index == intIndex)
            return f < x.i;
        else
            return f < x.f;
    }
}

number number::operator++(int)
{
    number res = *this;
    if (index == intIndex)
        i++;
    else
        f++;
    return res;
}
number number::operator--(int)
{
    number res = *this;
    if (index == intIndex)
        i--;
    else
        f--;
    return res;
}
number &number::operator++()
{
    if (index == intIndex)
        i++;
    else
        f++;
    return *this;
}
number &number::operator--()
{
    if (index == intIndex)
        i--;
    else
        f--;
    return *this;
}

number number::operator-() const
{
    if (index == intIndex)
        return -i;
    else
        return -f;
}

number number::toInteger() const
{
    return static_cast<intType>(*this);
}

number number::toFloatingPoint() const
{
    number n;
    n.index = floatIndex;
    n.f = static_cast<floatType>(*this);
    return n;
}

number number::ceil() const
{
    if (index == number::intIndex)
        return *this;
    else
        return static_cast<intType>(std::ceil(f));
}

number number::floor() const
{
    return toInteger();
}

std::ostream &operator<<(std::ostream &s, const number &x)
{
    if (x.index == number::intIndex)
        return s << x.i;
    else
        return s << x.f;
}
std::istream &operator>>(std::istream &s, number &x)
{
    if (x.index == number::intIndex)
        return s >> x.i;
    else
        return s >> x.f;
}

number operator"" _n(unsigned long long n)
{
    return number(static_cast<number::intType>(n));
}

number operator"" _n(long double n)
{
    return number(static_cast<number::floatType>(n));
}