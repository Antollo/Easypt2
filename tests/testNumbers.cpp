#include "utils.h"
#include "number.h"

int main()
{
    staticAssert(1 == (int)1_n);
    staticAssert(1 == (int)1.2_n);
    staticAssert(1.2 == (double)1.2_n);
    staticAssert(2.0 == double(22.2_n / 11.1_n));
    auto a = 12_n;
    a += 2;
    staticAssert(a == 14_n);
    a -= 3;
    staticAssert(a == 11_n);
    a *= 3;
    staticAssert(a == 33_n);
    a /= 11;
    staticAssert(a == 3_n);
    a %= 2;
    staticAssert(a == 1_n);

    staticAssert(++a == 2_n);
    staticAssert(a++ == 2_n);
    staticAssert(a-- == 3_n);
    staticAssert(--a == 1_n);

    staticAssert(static_cast<number>(7) == 7_n);
    staticAssert(static_cast<double>(7_n) == 7.0);
    std::cerr << static_cast<number>("3.14") << std::endl;
    staticAssert(static_cast<number>("3.14") == 3.14);
    staticAssert(static_cast<std::string>(3.14_n).find("3.14") == 0);
    return 0;
}