#error "deprecated"

#include "stack.h"
#include "defines.h"
#include "statement.h"
#include "error.h"
#include "parser.h"
#include "console.h"
#include "nobject.h"

std::vector<compoundStatement> compoundStatement::_compoundStatements;
std::unordered_map<name, token (*)(stack *st, std::vector<token> &)> expressionStatement::operators;

template <class T>
inline void guard(const std::stack<T> &s, std::string message = "invalid expression"s)
{
    if (s.empty())
        throw std::runtime_error(message);
}

void expressionStatement::addOperator(name n, token (*op)(stack *st, std::vector<token> &))
{
    operators.insert(std::make_pair(n, op));
}

token expressionStatement::operator()(stack *st, executionMemory &memory)
{
    if (_tokens.empty())
        return token();
    if (_tokens.front().getType() == token::tokenType::CompoundStatement && _tokens.size() == 1)
    {
        compoundStatement::get(_tokens.front().getCompoundStatementIndex())(st, memory);
        return token();
    }
    std::stack<token> &stack = memory.stack;
    std::vector<token> &args = memory.args;
    object::arrayType &arr = memory.arr;

    for (std::vector<token>::iterator it = _tokens.begin(); it != _tokens.end(); it++)
    {
        switch (it->getType())
        {
        case token::tokenType::Name:
        case token::tokenType::Value:
        case token::tokenType::StringLiteral:
        case token::tokenType::NumberLiteral:
        case token::tokenType::CompoundStatement:
            stack.push(*it);
            break;
        case token::tokenType::CallOperator:
            arr.resize(it->getArity());
            for (int i = it->getArity() - 1; i >= 0; i--)
            {
                arr[i] = stack.top().resolve(st);
                stack.pop();
            }
            if (std::next(it) != _tokens.end() && std::next(it)->getName() == "."_n)
            {
                object::objectPtr caller = stack.top().resolve(st);
                stack.pop();
                //st->insert("#called"_n, makeObject(static_cast<std::string>(it->getName())));
                stack.push(token((*(*caller)[it->getName()])(caller, std::move(arr), st)));
                it++;
            }
            else
            {
                //st->insert("#called"_n, makeObject(static_cast<std::string>(it->getName())));
                stack.push(token((*it->resolve(st))(nullptr, std::move(arr), st)));
            }
            break;
        case token::tokenType::Operator:
            if (it->getName() == "."_n)
            {
                token b = stack.top();
                stack.pop();
                token a = stack.top();
                stack.pop();
                stack.push(token((*a.resolve(st))[b.getName()]));
            }
            else
            {
                args.resize(it->getArity());
                for (int i = it->getArity() - 1; i >= 0; i--)
                {
                    args[i] = stack.top();
                    stack.pop();
                }
                if (operators.count(it->getName()) == 0)
                {
                    arr.resize(args.size());
                    for (size_t i = 0; i < args.size(); i++)
                        arr[i] = args[i].resolve(st);
                    stack.push(token((*(*st)[it->getName()])(nullptr, std::move(arr), st)));
                    arr.clear();
                    //throw std::runtime_error("unsupported operator " + static_cast<std::string>(it->getName()));
                }
                else
                    stack.push(operators[it->getName()](st, args));
            }
            break;
        default:
            throw std::runtime_error("unsupported token: " + static_cast<std::string>(it->getName()));
            break;
        }
    }
    return stack.top();
}

const token &expressionStatement::back() const
{
    return _tokens.back();
}

bool expressionStatement::empty() const
{
    return _tokens.empty();
}

compoundStatement::compoundStatement(std::string str) : compoundStatement(parser::parse(str)) {}

compoundStatement::compoundStatement(std::vector<std::vector<token>> &&tokens)
{
    _statements.reserve(tokens.size());
    std::transform(tokens.begin(), tokens.end(), std::back_inserter(_statements),
                   [](auto el) {
                       return expressionStatement(el);
                   });
}

expressionStatement::expressionStatement(const std::vector<token> &tokens) : _tokens(tokens) { check(); }

void expressionStatement::check(bool verbose) const
{
    if (_tokens.empty())
        return;
    if (_tokens.front().getType() == token::tokenType::CompoundStatement && _tokens.size() == 1)
    {
        compoundStatement::get(_tokens.front().getCompoundStatementIndex()).check(verbose);
        return;
    }
    std::stack<token> stack;
    std::vector<token> args;
    std::string temp;
    /*for (const auto &el : _tokens)
    {
        std::cout << (std::string)el.getName() << "_" << el.getArity() << "_" << (int)el.getType() << "  ";
        std::cout << std::endl;
    }*/
    /*for (const auto &el : _tokens)
    {
        console::write((std::string)el.getName(), " ");
    }*/
    for (const auto &el : _tokens)
    {
        switch (el.getType())
        {
        case token::tokenType::Name:
        case token::tokenType::StringLiteral:
        case token::tokenType::NumberLiteral:
        case token::tokenType::Value:
        case token::tokenType::CompoundStatement:
            stack.push(el);
            break;

        case token::tokenType::Operator:
        case token::tokenType::CallOperator:
            args.resize(el.getArity());
            for (int i = el.getArity() - 1; i >= 0; i--)
            {
                guard(stack);
                args[i] = stack.top();
                stack.pop();
            }
            temp = (std::string)el.getName() + "("s;
            for (const auto &a : args)
                temp += (std::string)a.getName() + ","s;
            if (temp.back() == ',')
                temp.pop_back();
            temp += ")"s;
            stack.push(token(temp));
            break;

        default:
            break;
        }
    }
    guard(stack);
    if (verbose)
        console::writeLine(static_cast<std::string>(stack.top().getName()));
}

compoundStatement &compoundStatement::get(int i)
{
    return _compoundStatements[i];
}

void compoundStatement::check(bool verbose) const
{
    try
    {
        for (const auto &statement : _statements)
            statement.check(verbose);
    }
    catch (const os_error &)
    {
        throw std::runtime_error("invalid expression (not expected)");
    }
    catch (const std::exception &e)
    {
        throw e;
    }
}

void compoundStatement::operator()(stack *st)
{
    executionMemory memory;
    stack localStack(st);
    (*this)(localStack, memory);
}

void compoundStatement::operator()(stack &st)
{
    executionMemory memory;
    (*this)(st, memory);
}

void compoundStatement::operator()(stack *st, executionMemory &memory)
{
    stack localStack(st);
    (*this)(localStack, memory);
}

void compoundStatement::operator()(stack &localStack, executionMemory &memory)
{
    for (std::vector<expressionStatement>::iterator it = _statements.begin(); it < _statements.end(); it++)
    {
        if (it->empty())
            continue;
        if (it->back().getName() == "if"_n || it->back().getName() == "while"_n || it->back().getName() == "for"_n)
        {
            if (it->back().getName() == "if"_n && it + 2 < _statements.end())
            {
                if ((*(it + 1))(&localStack, memory).resolve(&localStack)->getConverted<bool>())
                    (*(it + 2))(&localStack, memory);
                it += 2;
            }
            else if (it->back().getName() == "while"_n && it + 2 < _statements.end())
            {
                while ((*(it + 1))(&localStack, memory).resolve(&localStack)->getConverted<bool>())
                    (*(it + 2))(&localStack, memory);
                it += 2;
            }
            else if (it->back().getName() == "for"_n && it + 2 < _statements.end())
            {
                auto cp = *(it + 1);
                std::vector<token> &t = cp._tokens;
                if (t.size() >= 3)
                {
                    if (t[1].getName() == "of"_n)
                    {
                        auto name = t[0].getName();
                        t.erase(t.begin(), t.begin() + 2);
                        auto arrObj = cp(&localStack, memory).resolve(&localStack);
                        if (arrObj->isOfType<object::arrayType>())
                        {
                            object::arrayType &arr = arrObj->get<object::arrayType>();
                            stack loopStack(&localStack);
                            loopStack.insert(name, nullptr);
                            for (const auto &el : arr)
                            {
                                loopStack[name] = el;
                                (*(it + 2))(&loopStack, memory);
                            }
                        }
                        console::log((std::string)t[0].getName(), (std::string)t[2].getName());
                    }
                    else if (t[1].getName() == "in"_n)
                    {
                        auto name = t[0].getName();
                        t.erase(t.begin(), t.begin() + 2);
                        auto arrObj = cp(&localStack, memory).resolve(&localStack);
                        object::arrayType arr = arrObj->getOwnPropertyNames();
                        stack loopStack(&localStack);
                        loopStack.insert(name, nullptr);
                        for (const auto &el : arr)
                        {
                            if (el->isOfType<std::string>() && el->get<std::string>() == "prototype" || el->get<std::string>() == "classPrototype")
                                continue;
                            loopStack[name] = el;
                            (*(it + 2))(&loopStack, memory);
                        }
                    }
                    else
                        throw std::runtime_error("unsupported for loop type");
                }
                else
                    throw std::runtime_error("wrong number of arguments for for loop");
                it += 2;
            }
        }
        else
            (*it)(&localStack, memory);
    }
}