#include "parser.h"
#include "statement.h"
#include <queue>

template <class T>
inline void guard(const std::stack<T> &s, std::string message = "something is wrong"s)
{
    if (s.empty())
        throw std::runtime_error(message);
}

inline std::string atOperator(std::string s)
{
    if (s.front() == '@')
    {
        s.erase(s.begin());
        if (s.empty())
            throw std::runtime_error("empty @ operator");
    }
    if (s.back() == '@')
    {
        s.pop_back();
        if (s.empty())
            throw std::runtime_error("empty @ operator");
    }
    return s;
}

std::vector<std::string> parser::literals;

std::vector<token> parser::shuntingYard(std::vector<std::string> &&input)
{
    std::stack<std::string> stack;
    std::stack<bool> wereValues;
    std::stack<int> argCount;
    std::vector<token> res;
    res.reserve(input.size());
    for (auto it = input.begin(); it != input.end(); it++)
    {
        // if function argument separator
        if (*it == ","s)
        {
            while (!stack.empty() && (stack.top() != "("s && stack.top() != "["s))
            {
                //res.push_back(stack.top());
                res.push_back(token(atOperator(stack.top()), operatorArity(stack.top()), token::tokenType::Operator));
                stack.pop();
            }
            guard(stack, "\"(\" or \"[\" not found");
            guard(wereValues);
            auto w = wereValues.top();
            wereValues.pop();
            if (w)
            {
                guard(argCount);
                auto a = argCount.top();
                argCount.pop();
                a++;
                argCount.push(a);
            }
            wereValues.push(false);
        }
        else if (*it == "("s)
        {
            stack.push(*it);
        }
        else if (*it == "["s)
        {
            stack.push(*it);
        }
        else if (*it == ")"s)
        {
            while (!stack.empty() && stack.top() != "("s)
            {
                //res.push_back(stack.top());
                res.push_back(token(atOperator(stack.top()), operatorArity(stack.top()), token::tokenType::Operator));
                stack.pop();
            }
            guard(stack, "\"(\" not found");

            stack.pop();
            if (stack.top().back() == '(')
            {
                auto f = stack.top().substr(0, stack.top().size() - 1);
                stack.pop();
                guard(argCount);
                auto a = argCount.top();
                argCount.pop();
                guard(wereValues);
                auto w = wereValues.top();
                wereValues.pop();
                if (w)
                    a++;
                //std::cout<<"function: "<<f<<"\n";
                ///res.push_back(f + std::to_string(a));
                res.push_back(token(f, a));
            }
        }
        else if (*it == "]"s)
        {
            while (!stack.empty() && stack.top() != "["s)
            {
                //res.push_back(stack.top());
                res.push_back(token(atOperator(stack.top()), operatorArity(stack.top()), token::tokenType::Operator));
                stack.pop();
            }
            guard(stack, "\"[\" not found");

            stack.pop();
            if (stack.top().back() == '[')
            {
                auto f = stack.top().substr(0, stack.top().size() - 1);
                stack.pop();
                guard(argCount);
                auto a = argCount.top();
                argCount.pop();
                guard(wereValues);
                auto w = wereValues.top();
                wereValues.pop();
                if (w)
                    a++;
                ///res.push_back(f + std::to_string(a));

                auto ai = a;
                std::stack<token> helper;

                while (ai--)
                {
                    helper.push(res.back());
                    res.pop_back();
                }
                if (f.find("#s"s) == 0)
                    res.push_back(token(literals[std::stoi(f.substr(2))], token::tokenType::StringLiteral));
                else if (isNumberLiteral(f))
                    res.push_back(token(f, token::tokenType::NumberLiteral));
                else
                    res.push_back(token(f));

                while (!helper.empty())
                {
                    res.push_back(helper.top());
                    helper.pop();
                }
                res.push_back(token("readOperator", a, token::tokenType::CallOperator));

                res.push_back(token(".", operatorArity("."), token::tokenType::Operator));
            }
        }
        // if operator
        else if (isOperator(*it))
        {
            if ((*it == "+" || *it == "-" || *it == "*") && (it == input.begin() || (isOperator(*std::prev(it)) && *std::prev(it) != "]" && *std::prev(it) != ")")))
                *it += "u";
            while ((!stack.empty() && isOperator(stack.top()) && stack.top() != "(" && stack.top() != "[") &&
                   ((operatorAssociativity(*it) && operatorPrecedence(*it) >= operatorPrecedence(stack.top())) ||
                    (!operatorAssociativity(*it) && operatorPrecedence(*it) > operatorPrecedence(stack.top()))))
            {
                res.push_back(token(atOperator(stack.top()), operatorArity(stack.top()), token::tokenType::Operator));
                stack.pop();
            }
            stack.push(*it);
        }
        //if function token
        else if (std::next(it) != input.end() && (*std::next(it) == "("s || *std::next(it) == "["s))
        {
            stack.push(*it + *std::next(it));
            argCount.push(0);
            if (!wereValues.empty())
            {
                wereValues.pop();
                wereValues.push(true);
            }
            wereValues.push(false);
        }
        // normal token
        else
        {
            if (it->find("#s"s) == 0)
                res.push_back(token(literals[std::stoi(it->substr(2))], token::tokenType::StringLiteral));
            else if (isNumberLiteral(*it))
                res.push_back(token(*it, token::tokenType::NumberLiteral));
            else if (it->find("#c"s) == 0)
                res.push_back(token(*it, std::stoi(it->substr(2)), token::tokenType::CompoundStatement));
            else
                res.push_back(token(*it));
            if (!wereValues.empty())
            {
                wereValues.pop();
                wereValues.push(true);
            }
        }
    }
    while (!stack.empty())
    {
        res.push_back(token(atOperator(stack.top()), operatorArity(stack.top()), token::tokenType::Operator));
        if (stack.top() == "("s || stack.top() == ")"s)
            throw std::runtime_error("mismatched parenthesis");
        stack.pop();
    }
    return res;
}

std::vector<std::vector<token>> parser::parse(std::string &input)
{
    registerLiterals(input);
    removeComments(input);
    registerConditionals(input);
    registerCompounds(input);
    fixParenthesis(input);
    return parseFlat(input);
}

void parser::registerLiterals(std::string &input)
{
    static std::regex stringRegex(stringRegexString);
    static int counter = 0;
    std::sregex_token_iterator begin(input.begin(), input.end(), stringRegex, 0), end;
    std::vector<std::string> res;

    std::copy(begin, end, std::back_inserter(res));
    size_t pos = 0;
    for (auto &string : res)
    {
        pos = input.find(string, pos);
        input.replace(pos, string.size(), "#s"s + std::to_string(counter));
        literals.push_back(string);
        counter++;
    }
}

void parser::removeComments(std::string &input)
{
    static std::regex blockComment(R"(\/\*([^*]*(\*[^\/])*)*\*\/)");
    static std::regex lineComment(R"(\/\/[^\n]*\n)");
    std::smatch sm;

    while (std::regex_search(input, sm, blockComment))
        input.erase(sm.position(), sm.length());

    while (std::regex_search(input, sm, lineComment))
        input.erase(sm.position(), sm.length());
}

void parser::fixParenthesis(std::string &input)
{
    static std::regex chainingParenthesis(R"(\)\()");
    //TODO remove regexes, add normal find
    static std::regex brackets1(R"(\[)");
    static std::regex brackets2(R"(\])");
    std::smatch sm;

    while (std::regex_search(input, sm, chainingParenthesis))
        input.replace(sm.position(), sm.length(), ").callOperator(");

    while (std::regex_search(input, sm, brackets1))
        input.replace(sm.position(), sm.length(), ".readOperator(");
    while (std::regex_search(input, sm, brackets2))
        input.replace(sm.position(), sm.length(), ")");
}

void parser::registerConditionals(std::string &input)
{
    static std::regex compoundRegex(R"((if|while)\s*\(([^;]+)\)[\ \r\t\f\v]*\n([^;]*);)");
    std::smatch sm;
    while (std::regex_search(input, sm, compoundRegex))
    {
        input.replace(sm.position(), sm.length(), sm[1].str() + ";" + sm[2].str() + ";" + sm[3].str() + ";");
    }
}

void parser::registerCompounds(std::string &input)
{
    static std::regex jsonRegex(R"(:\s*\{)");
    static std::regex jsonArrayRegex(R"(:\s*\[)");
    static std::regex compoundRegex(R"(\{[^\{\}]*\})");
    static int counter = 0;
    std::smatch sm;
    while (std::regex_search(input, sm, jsonRegex))
        input.replace(sm.position(), sm.length(), ":json{"s);
    while (std::regex_search(input, sm, jsonArrayRegex))
        input.replace(sm.position(), sm.length(), ":Array["s);
    while (std::regex_search(input, sm, compoundRegex))
    {
        //compoundCallback(parseFlat(sm.str()));
        compoundStatement::_compoundStatements.push_back(parseFlat(sm.str()));
        input.replace(sm.position(), sm.length(), "#c"s + std::to_string(counter) + ";"s);
        counter++;
    }
}

std::vector<std::vector<token>> parser::parseFlat(const std::string &input)
{
    auto expressions = splitExpressions(input);
    std::vector<std::vector<token>> res;
    std::transform(expressions.begin(), expressions.end(), std::back_inserter(res),
                   [](const auto &expression) {
                       return shuntingYard(tokenize(expression));
                   });
    return res;
}

std::vector<std::string> parser::splitExpressions(const std::string &input)
{
    std::vector<std::string> res;
    std::string temp;
    std::istringstream tokenStream(input);
    while (std::getline(tokenStream, temp, ';'))
    {
        res.push_back(temp);
        //std::cout<<"'"<<temp<<"'\n";
    }

    return res;
}

std::vector<std::string> parser::tokenize(const std::string &input)
{
    static std::regex tokenRegex(tokenRegexString);
    std::sregex_token_iterator begin(input.begin(), input.end(), tokenRegex, 0), end;
    std::vector<std::string> res;
    std::copy(begin, end, std::back_inserter(res));
    return res;
}

bool parser::isOperator(const std::string &token)
{
    return std::regex_match(token, std::regex(operatorRegexString));
}

bool parser::isUnaryOperator(const std::string &token)
{
    return token.back() == 'u' || token == "++" || token == "--" || token == "!" || token == "~" || token == "let" || token == "function" || token == "return" || token == "json" || (token.back() == '@' && token.front() != '@') || (token.back() != '@' && token.front() == '@');
}

bool parser::isNumberLiteral(const std::string &token)
{
    return std::regex_match(token, std::regex(numberRegexString));
}

int parser::operatorArity(const std::string &token)
{
    //TODO REMOVE
    if (!isOperator(token))
        throw std::runtime_error("this is not operator: "s + token);
    return 1 + (int)(!isUnaryOperator(token));
}

int parser::operatorPrecedence(const std::string &token)
{
    if (token == "++" || token == "--" || token == "." || token == "(" || token == ")" || token == "[" || token == "]")
        return 2;
    else if (token == "+u" || token == "-u" || token == "!" || token == "~" || token == "*u" || token == "let" || token == "function" || token == "json")
        return 3;
    else if (token == "*" || token == "/" || token == "%")
        return 5;
    else if (token == "+" || token == "-")
        return 6;
    else if (token == "<<" || token == ">>")
        return 7;
    else if (token == "<" || token == ">" | token == "<=" || token == ">=")
        return 9;
    else if (token == "==" || token == "!=")
        return 10;
    else if (token == "&")
        return 11;
    else if (token == "^")
        return 12;
    else if (token == "|")
        return 13;
    else if (token == "&&")
        return 14;
    else if (token == "||")
        return 15;
    else if (token.front() == '@' || token.back() == '@')
        return 16;
    else if (token.back() == '=' || token == ":" || token == "return")
        return 17;
    else
        throw std::runtime_error("operator has no precedence");
}

// true if left-associative
bool parser::operatorAssociativity(const std::string &token)
{
    if (token == "+u" || token == "-u" || token == "!" || token == "~" || token == "*u" || token == "=" || token == ":" || token == "+=" || token == "-=" || token == "*=" || token == "/=" || token == "%=" || token == "<<=" || token == ">>=" || token == "&=" || token == "^=" || token == "|=" || token == "let" || token == "function" || token == "json" || token == "return" || (token.back() == '@' && token.front() != '@'))
        return false;
    return true;
}