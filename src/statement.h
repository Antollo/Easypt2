#error "deprecated"

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <functional>
#include <vector>
#include <stack>
#include "token.h"
#include "objectPtrImpl.h"

class stack;

class executionMemory
{
public:
    executionMemory() = default;
    executionMemory(const executionMemory &) = delete;
    executionMemory &operator=(const executionMemory &) = delete;
    executionMemory(executionMemory &&) = delete;
    executionMemory &operator=(executionMemory &&) = delete;

private:
    std::stack<token> stack;
    std::vector<token> args;
    std::vector<objectPtrImpl> arr;
    friend class expressionStatement;
    friend class compoundStatement;
};

class expressionStatement
{
public:
    static void addOperator(name n, token (*op)(stack *st, std::vector<token> &));

private:
    expressionStatement(const std::vector<token> &tokens);
    void check(bool verbose = false) const;
    token operator()(stack *st, executionMemory& memory);
    const token &back() const;
    bool empty() const;
    std::vector<token> _tokens;
    friend class compoundStatement;
    static std::unordered_map<name, token (*)(stack *st, std::vector<token> &)> operators;
};

class compoundStatement
{
public:
    compoundStatement(std::string str);
    compoundStatement(std::vector<std::vector<token>> &&tokens);
    void check(bool verbose = false) const;
    void operator()(stack *st);
    void operator()(stack &st);
    void operator()(stack *st, executionMemory& memory);
    void operator()(stack &localStack, executionMemory& memory);
    static compoundStatement &get(int i);

private:
    std::vector<expressionStatement> _statements;
    static std::vector<compoundStatement> _compoundStatements;
    friend class expressionStatement;
    friend class parser;
};

#endif /* !EXPRESSION_H_ */
