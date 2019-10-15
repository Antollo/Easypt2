#ifndef NUMBER_H_
#define NUMBER_H_

#include <variant>
#include <utility>
#include <string>

using namespace std::rel_ops;

class number
{

public:
    number(const int &n);
    number(const double &n);
    number(const size_t &n);
    number(const std::string &n);
    number(const char* n);
    number(const bool &n);

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

    number operator++(int);
    number operator--(int);
    number &operator++();
    number &operator--();

    number toInteger();

    explicit operator double() const;
    explicit operator int() const;
    explicit operator bool() const;
    explicit operator std::string() const;

    friend std::ostream &operator<<(std::ostream &, const number &);
    friend std::istream &operator>>(std::istream &, number &);

private:
    std::variant<int, double> v;
};

std::ostream &operator<<(std::ostream &s, const number &x);
std::istream &operator>>(std::istream &s, number &x);

number operator"" _n(unsigned long long n);
number operator"" _n(long double n);

#endif /* !NUMBER_H_ */
