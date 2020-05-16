#error "deprecated"

#include "parser.h"
#include "statement.h"
#include <queue>

template <class T>
inline void guard(const std::stack<T> &s, std::string message = "something is wrong"s)
{
    if (s.empty())
        throw std::runtime_error(message);
}

void replaceAll(std::string &templ, const std::string &from, const std::string &to)
{
    size_t pos = 0;
    while (true)
    {
        pos = templ.find(from, pos);
        if (pos == std::string::npos)
            return;
        templ.replace(pos, from.size(), to);
    }
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
int parser::literalsCounter = 0;

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
            /*auto w = wereValues.top();
            wereValues.pop();
            if (w)
            {
                guard(argCount);
                auto a = argCount.top();
                argCount.pop();
                a++;
                argCount.push(a);
            }
            wereValues.push(false);*/
            if (wereValues.top())
            {
                wereValues.top() = false;
                guard(argCount);
                argCount.top()++;
            }
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

                /*auto ai = a;
                std::stack<token> helper;

                while (ai--)
                {
                    helper.push(res.back());
                    res.pop_back();
                }*/
                std::vector<token> helper(res.end() - a, res.end());
                res.erase(res.end() - a, res.end());

                if (f.find("#s"s) == 0)
                    res.push_back(token(literals[std::stoi(f.substr(2))], token::tokenType::StringLiteral));
                else if (isNumberLiteral(f))
                    res.push_back(token(f, token::tokenType::NumberLiteral));
                else
                    res.push_back(token(f));

                /*while (!helper.empty())
                {
                    res.push_back(helper.top());
                    helper.pop();
                }*/
                res.insert(res.end(), helper.begin(), helper.end());

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
            wereValues.top() = true;
            /*{
                wereValues.pop();
                wereValues.push(true);
            }*/
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
                wereValues.top() = true;
            /*{
                wereValues.pop();
                wereValues.push(true);
            }*/
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

std::vector<std::vector<token>> parser::transpileAndParse(std::string &input)
{
    transpile(input);
    return parse(input);
}

std::vector<std::vector<token>> parser::parse(std::string &input)
{
    registerCompounds(input);
    //console::debug(input);
    return parseFlat(input);
}

void parser::transpile(std::string &input)
{
    if (input.empty() || input.back() != '\n')
        input += "\n";
    registerLiterals(input);
    removeComments(input);
    transformConditionals(input);
    transformSyntacticSugar(input);
    transformBrackets(input);
    //console::debug(input);
}

void parser::restoreLiterals(std::string &input)
{
    static std::regex stringMarkerRegex(R"(#s([0-9]+))");
    std::smatch sm;

    while (std::regex_search(input, sm, stringMarkerRegex))
        input.replace(sm.position(), sm.length(), literals[std::stoi(sm[1].str())]);
}

void parser::registerLiterals(std::string &input)
{
    static std::regex stringRegex(stringRegexString);
    std::sregex_token_iterator begin(input.begin(), input.end(), stringRegex, 0), end;
    std::vector<std::string> res;

    std::copy(begin, end, std::back_inserter(res));
    size_t pos = 0;
    for (auto &string : res)
    {
        pos = input.find(string, pos);
        input.replace(pos, string.size(), "#s"s + std::to_string(literalsCounter));
        literals.push_back(string);
        literalsCounter++;
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

void parser::transformBrackets(std::string &input)
{
    static std::regex chainingParenthesis(R"(\)\()");
    std::smatch sm;

    replaceAll(input, "["s, ".readOperator("s);
    replaceAll(input, "]"s, ")"s);

    while (std::regex_search(input, sm, chainingParenthesis))
        input.replace(sm.position(), sm.length(), ").callOperator(");
}

void parser::transformConditionals(std::string &input)
{
    static std::regex ifWhileRegex(R"((if|while|for)\s*\(([^\;\{]+)\)[\ \r\t\f\v]*[\n]+([^\;\{]*)\;)");
    static std::regex ifWhileRegexSecond(R"((if|while|for)\s*\(([^\;\{]+)\)[\ \r\t\f\v]*[\n]*\{)");
    std::smatch sm;
    while (std::regex_search(input, sm, ifWhileRegex))
    {
        input.replace(sm.position(), sm.length(), sm[1].str() + ";" + sm[2].str() + ";" + sm[3].str() + ";");
    }
    while (std::regex_search(input, sm, ifWhileRegexSecond))
    {
        input.replace(sm.position(), sm.length(), sm[1].str() + ";" + sm[2].str() + "; {");
    }
}

void parser::transformSyntacticSugar(std::string &input)
{
    static std::regex jsonRegex(R"((:|\,|\(|\[)\s*\{)");
    static std::regex jsonArrayRegex(R"(:\s*\[)");
    static std::regex jsonCommaColon(",(\\s)*(" + stringRegexString + "|" + nameRegexString + ")(\\s)*:");

    std::smatch sm;
    while (std::regex_search(input, sm, jsonRegex))
        input.replace(sm.position(), sm.length(), sm[1].str() + "json{"s);
    while (std::regex_search(input, sm, jsonArrayRegex))
        input.replace(sm.position(), sm.length(), ":Array["s);
    while (std::regex_search(input, sm, jsonCommaColon))
        input.replace(sm.position(), sm.length(), ";" + sm[2].str() + ":");
}

void parser::registerCompounds(std::string &input)
{
    static std::regex compoundRegex(R"(\{[^\{\}]*\})", std::regex::ECMAScript | std::regex::optimize);
    static std::regex classRegex(R"(class\s+([#_A-Za-z0-9]+)\s+(\{[^\{\}]*\}))", std::regex::ECMAScript | std::regex::optimize);
    static std::regex valuedCompoundRegex(R"((function|json)\s*(\{[^\{\}]*\}))", std::regex::ECMAScript | std::regex::optimize);
    static std::regex catchCompoundRegex(R"((\{[^\{\}]*\})\s*(catch))", std::regex::ECMAScript | std::regex::optimize);
    static int counter = 0;
    std::smatch sm;
    std::string temp;
    bool continuation = true, foundCompound = false;
    while (continuation)
    {
        foundCompound = false;
        while (std::regex_search(input, sm, classRegex))
        {
            input.replace(sm.position(), sm.length(), "let "s + std::string(sm[1].first, sm[1].second) + " <- Class(json "s + std::string(sm[2].first, sm[2].second) + ");"s);
        }
        while (std::regex_search(input, sm, valuedCompoundRegex))
        {
            temp = sm[2].str();
            compoundStatement::_compoundStatements.push_back(parseFlat(temp));
            input.replace(sm[2].first, sm[2].second, "#c"s + std::to_string(counter));
            counter++;
            foundCompound = true;
        }
        while (std::regex_search(input, sm, catchCompoundRegex))
        {
            temp = sm[1].str();
            compoundStatement::_compoundStatements.push_back(parseFlat(temp));
            input.replace(sm[1].first, sm[1].second, "#c"s + std::to_string(counter));
            counter++;
            foundCompound = true;
        }
        if (foundCompound)
            continue;
        continuation = false;
        if (std::regex_search(input, sm, compoundRegex))
        {
            temp = sm.str();
            compoundStatement::_compoundStatements.push_back(parseFlat(temp));
            input.replace(sm.position(), sm.length(), "#c"s + std::to_string(counter) + ";"s);
            counter++;
            continuation = true;
        }
    }
    //console::log(input);
}

std::vector<std::vector<token>> parser::parseFlat(std::string &input)
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

void parser::clearCache()
{
    literals.clear();
    literalsCounter = 0;
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
	static std::regex operatorRegex(operatorRegexString);
    return std::regex_match(token, operatorRegex);
}

bool parser::isUnaryOperator(const std::string &token)
{
    return token.back() == 'u' || token == "++" || token == "--" || token == "!" || token == "~" || token == "let" || token == "function" || token == "return" || token == "throw" || token == "json" || (token.back() == '@' && token.front() != '@') || (token.back() != '@' && token.front() == '@');
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
    else if (token == "+" || token == "-" || token == "catch")
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
    else if (token.back() == '=' || token == ":" || token == "return" || token == "throw" || token == "<-")
        return 17;
    else
        throw std::runtime_error("operator has no precedence");
}

// true if left-associative
bool parser::operatorAssociativity(const std::string &token)
{
    if (token == "+u" || token == "-u" || token == "!" || token == "~" || token == "*u" || token == "=" || token == "<-" || token == ":" || token == "+=" || token == "-=" || token == "*=" || token == "/=" || token == "%=" || token == "<<=" || token == ">>=" || token == "&=" || token == "^=" || token == "|=" || token == "let" || token == "function" || token == "json" || token == "return" || token == "throw" || (token.back() == '@' && token.front() != '@'))
        return false;
    return true;
}