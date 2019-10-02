#include "utils.h"
#include "statement.h"

int main()
{
    auto a = compoundStatement("a + b * c");
    auto b = compoundStatement("a + (b - c)");
    auto c = compoundStatement("a + (-b)");
    auto d = compoundStatement("f(x, y)");
    auto e = compoundStatement("*f(g(x))");
    auto f = compoundStatement("A[\" str\" , 3.14]");
    auto g = compoundStatement("\"g\"[1, 2]");

    assert("+(a,*(b,c))\n" == out([&a]() {
               a.check(true);
           }));
    assert("+(a,-(b,c))\n" == out([&b]() {
               b.check(true);
           }));
    assert("+(a,-u(b))\n" == out([&c]() {
               c.check(true);
           }));
    assert("f(x,y)\n" == out([&d]() {
               d.check(true);
           }));
    assert("*u(f(g(x)))\n" == out([&e]() {
               e.check(true);
           }));
    assert(".(A,readOperator(\" str\",3.14))\n" == out([&f]() {
               f.check(true);
           }));
    assert(".(\"g\",readOperator(1,2))\n" == out([&g]() {
               g.check(true);
           }));

    return 0;
}