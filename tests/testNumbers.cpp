#include "utils.h"
#include "number.h"
#include "number.cpp"

int main()
{
    assert(1==(int)1_n);
    assert(1==(int)1.2_n);
    assert(1.2==(double)1.2_n);
    assert(2.0==double(22.2_n/11.1_n));
    auto a = 12_n;
    a+=2;
    assert(a==14_n);
    a-=3;
    assert(a==11_n);
    a*=3;
    assert(a==33_n);
    a/=11;
    assert(a==3_n);
    a%=2;
    assert(a==1_n);

    assert(++a==2_n);
    assert(a++==2_n);
    assert(a--==3_n);
    assert(--a==1_n);

    assert(static_cast<number>(7)==7_n);
    assert(static_cast<double>(7_n)==7.0);
    std::cerr<<static_cast<number>("3.14")<<std::endl;
    assert(static_cast<number>("3.14")==3.14);
    assert(static_cast<std::string>(3.14_n).find("3.14") == 0);
    return 0;
}