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

    staticAssert("+(a,*(b,c))\n" == console::getOutput([&a]() {
                     a.check(true);
                 }));
    staticAssert("+(a,-(b,c))\n" == console::getOutput([&b]() {
                     b.check(true);
                 }));
    staticAssert("+(a,-u(b))\n" == console::getOutput([&c]() {
                     c.check(true);
                 }));
    staticAssert("f(x,y)\n" == console::getOutput([&d]() {
                     d.check(true);
                 }));
    staticAssert("*u(f(g(x)))\n" == console::getOutput([&e]() {
                     e.check(true);
                 }));
    staticAssert(".(A,readOperator(\" str\",3.14))\n" == console::getOutput([&f]() {
                     f.check(true);
                 }));
    staticAssert(".(\"g\",readOperator(1,2))\n" == console::getOutput([&g]() {
                     g.check(true);
                 }));

    return 0;
}