#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <stack>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <functional>
#include "token.h"

class parser
{
public:
    static std::vector<std::vector<token>> transpileAndParse(std::string &input);
    static std::vector<std::vector<token>> parse(std::string &input);
    static void transpile(std::string &input);
    static void restoreLiterals(std::string &input);
    static void registerLiterals(std::string &input);
    static void clearCache();

private:
    const static inline std::string basicOperatorRegexString = R"((<<|>>|\+|-|\*|\/|%|&|\||\^|<|>)=|\|\||&&|<<|>>|--|\+\+|->|==|\%|\&|\+|\-|\=|\/|\||\.|\*|\:|>|<|\!|\?|~|\^|\(|\)|\,|\[|\]|\+u|-u|\*u)";
    const static inline std::string extendedOperatorRegexString = "(@[_A-Za-z0-9]+@?|[_A-Za-z0-9]+@|let|function|return|json)";
    const static inline std::string operatorRegexString = basicOperatorRegexString + "|"s + extendedOperatorRegexString;
    const static inline std::string numberRegexString = R"([0-9]+([.][0-9]+)?)";
    const static inline std::string nameRegexString = R"([#_A-Za-z0-9]+)";
    const static inline std::string stringRegexString = R"("[^"\\]*(\\.[^"\\]*)*")";
    const static inline std::string tokenRegexString = operatorRegexString + "|" + numberRegexString + "|" + stringRegexString + "|" + nameRegexString;

    static std::vector<std::string> literals;
    static int literalsCounter;

    static void removeComments(std::string &input);
    static void transformBrackets(std::string &input);
    static void transformConditionals(std::string &input);
    static void registerCompounds(std::string &input);
    static void transformSyntacticSugar(std::string &input);
    static std::vector<std::vector<token>> parseFlat(std::string &input);
    static std::vector<std::string> splitExpressions(const std::string &input);
    static std::vector<std::string> tokenize(const std::string &input);
    static std::vector<token> shuntingYard(std::vector<std::string> &&input);
    static bool isOperator(const std::string &token);
    static bool isUnaryOperator(const std::string &token);
    static bool isNumberLiteral(const std::string &token);
    static int operatorArity(const std::string &token);
    static int operatorPrecedence(const std::string &token);
    static bool operatorAssociativity(const std::string &token); // true if left-associative
};

#endif /* !PARSER_H_ */
