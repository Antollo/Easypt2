#include "utils.h"
#include "nobject.h"

int main()
{
    auto x = makeObject(1_n);
    assert(std::is_same_v<number&, decltype(x->get<number>())>);
    assert(std::is_same_v<const number&, decltype(x->get<const number>())>);
    x->setConst();
    assert(std::is_same_v<const number&, decltype(x->get<const number>())>);
    assert(std::is_same_v<number&, decltype(x->get<number>())>);

    bool throwed = false;
    try
    {
        x->get<const number>();
    }
    catch(const std::exception&){}
    assert(throwed == false);

    try
    {
        x->get<number>();
    }
    catch(const std::exception&)
    {
        throwed = true;
    }
    assert(throwed);

    
    return 0;
}