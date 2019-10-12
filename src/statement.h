#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <functional>
#include "token.h"

class stack;

class expressionStatement
{
public:
    static void addOperator(name n, token(*op)(stack *st, std::vector<token> &));

private:
    expressionStatement(const std::vector<token> &tokens);
    void check(bool verbose = false) const;
    token operator()(stack *st) const;
    const token& back() const;
    bool empty() const;
    std::vector<token> _tokens;
    friend class compoundStatement;
    static std::unordered_map<name, token(*)(stack *st, std::vector<token> &)> operators;
};

class compoundStatement
{
public:
    compoundStatement(std::string str);
    compoundStatement(std::vector<std::vector<token>> &&tokens);
    void check(bool verbose = false) const;
    void operator()(stack *st) const;
    void operator()(stack &localStack) const;
    static compoundStatement& get(int i);

private:
    std::vector<expressionStatement> _statements;
    static std::vector<compoundStatement> _compoundStatements;
    friend class expressionStatement;
    friend class parser;
};

#endif /* !EXPRESSION_H_ */
