#include <cmath>
#include "number.h"

number::number(const int &n) : v(n){};
number::number(const double &n) {
    if(std::floor(n) == std::ceil(n))
        v = static_cast<int>(n);
    else
        v = n;
};
number::number(const std::string &n)
{
    if (n.find('.') == std::string::npos)
        v = std::stoi(n);
    else
        v = std::stod(n);
};
number::number(const char* n) : number(std::string(n)) {};
number::number(const bool &n) : v(static_cast<int>(n)){};

number &number::operator+=(const number &x)
{
    return *this = *this + x;
}
number &number::operator-=(const number &x)
{
    return *this = *this - x;
}
number &number::operator*=(const number &x)
{
    return *this = *this * x;
}
number &number::operator/=(const number &x)
{
    return *this = *this / x;
}
number &number::operator%=(const number &x)
{
    return *this = *this % x;
}

number number::operator+(const number &x) const
{
    return std::visit([](auto &&t, auto &&x) -> number { return t + x; }, v, x.v);
}
number number::operator-(const number &x) const
{
    return std::visit([](auto &&t, auto &&x) -> number { return t - x; }, v, x.v);
}

number number::operator*(const number &x) const
{
    return std::visit([](auto &&t, auto &&x) -> number { return t * x; }, v, x.v);
}
number number::operator/(const number &x) const
{
    if (static_cast<double>(x) == 0.0)
        throw std::runtime_error("division by zero");
    return std::visit([](auto &&t, auto &&x) -> number { return static_cast<double>(t) / static_cast<double>(x); }, v, x.v);
}
number number::operator%(const number &x) const
{
    if (static_cast<int>(x) == 0)
        throw std::runtime_error("modulo by zero");
    return std::visit([](auto &&t, auto &&x) -> number { return static_cast<int>(t) % static_cast<int>(x); }, v, x.v);
}

bool number::operator==(const number &x) const
{
    return std::visit([](auto &&t, auto &&x) -> bool { return t == x; }, v, x.v);
}
bool number::operator<(const number &x) const
{
    return std::visit([](auto &&t, auto &&x) -> bool { return t < x; }, v, x.v);
}

number number::operator++(int)
{
    number res = *this;
    std::visit([](auto &&t) { t++; }, v);
    return res;
}
number number::operator--(int)
{
    number res = *this;
    std::visit([](auto &&t) { t--; }, v);
    return res;
}
number &number::operator++()
{
    std::visit([](auto &&t) { t++; }, v);
    return *this;
}
number &number::operator--()
{
    std::visit([](auto &&t) { t--; }, v);
    return *this;
}

number::operator double() const
{
    return std::visit([](auto &&t) -> double { return t; }, v);
}
number::operator int() const
{
    return std::visit([](auto &&t) -> int { return static_cast<int>(t); }, v);
}
number::operator std::string() const
{
    return std::visit([](auto &&t) { return std::to_string(t); }, v);
}
number::operator bool() const
{
    return std::visit([](auto &&t) -> bool { return t; }, v);
}

number number::toInteger()
{
    return static_cast<int>(*this);
}

std::ostream &operator<<(std::ostream &s, const number &x)
{
    return std::visit([&s](auto &&x) -> std::ostream & { return s << x; }, x.v);
}
std::istream &operator>>(std::istream &s, number &x)
{
    return std::visit([&s](auto &&x) -> std::istream & { return s >> x; }, x.v);
}

number operator"" _n(unsigned long long n)
{
    return number(static_cast<int>(n));
}
number operator"" _n(long double n)
{
    return number(static_cast<double>(n));
}