#include "utils.h"
#include "nobject.h"

int main()
{
    auto x = makeObject(1_n);
    staticAssert(std::is_same_v<number&, decltype(x->get<number>())>);
    staticAssert(std::is_same_v<const number&, decltype(x->get<const number>())>);
    x->setConst();
    staticAssert(std::is_same_v<const number&, decltype(x->get<const number>())>);
    staticAssert(std::is_same_v<number&, decltype(x->get<number>())>);

    bool throwed = false;
    try
    {
        x->get<const number>();
    }
    catch(const std::exception&){}
    staticAssert(throwed == false);

    try
    {
        x->get<number>();
    }
    catch(const std::exception&)
    {
        throwed = true;
    }
    staticAssert(throwed);

    
    return 0;
}