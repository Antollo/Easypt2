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

    assert("+(a,*(b,c))\n" == console::getOutput([&a]() {
               a.check(true);
           }));
    assert("+(a,-(b,c))\n" == console::getOutput([&b]() {
               b.check(true);
           }));
    assert("+(a,-u(b))\n" == console::getOutput([&c]() {
               c.check(true);
           }));
    assert("f(x,y)\n" == console::getOutput([&d]() {
               d.check(true);
           }));
    assert("*u(f(g(x)))\n" == console::getOutput([&e]() {
               e.check(true);
           }));
    assert(".(A,readOperator(\" str\",3.14))\n" == console::getOutput([&f]() {
               f.check(true);
           }));
    assert(".(\"g\",readOperator(1,2))\n" == console::getOutput([&g]() {
               g.check(true);
           }));

    return 0;
}