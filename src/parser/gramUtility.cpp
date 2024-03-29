#include <string>
#include <iostream>
#include <cctype>
#include <stdexcept>
#include "gramUtility.h"
#include "treeParser.h"

extern int yylineno;

std::string tokenToName(int token)
{
    std::string ret = LLgetSymbol(token);
    if (ret.back() == '_')
        ret.pop_back();
    return ret;
}

void LLmessage(int expectedToken)
{
    LLmessage(expectedToken, LLsymb);
}

void LLmessage(int expectedToken, int token)
{
    std::string message = "syntax error in " + treeParser::file + ":" + std::to_string(yylineno);
    switch (expectedToken)
    {
    case LL_MISSINGEOF:
        message += " expected EOF not encountered, unexpected token <" + tokenToName(token) + "> found";
        break;
    case LL_DELETE:
        message += " unexpected token <" + tokenToName(token) + "> detected";
        break;
    default:
        message += " expected token <" + tokenToName(expectedToken) + "> not found";
        message += " unexpected token <" + tokenToName(token) + "> detected";
        break;
    }
    treeParser::throwLater(message);
}

int operatorPriority(int token)
{
    switch (token)
    {
    case INCREMENT:
    case DECREMENT:
    case DOT:
    case PARENTHESES_OPEN:
    case PARENTHESES_CLOSE:
    case BRACKET_OPEN:
    case BRACKET_CLOSE:
        return 2;
    case NOT:
    case COMPLEMENT:
    case LET:
    case FUNCTION:
    case JSON:
    case DECORATOR:
        return 3;
    case MULTIPLICATION:
    case DIVISION:
    case MODULUS:
        return 5;
    case ADDITION:
    case SUBTRACTION:
    case CATCH:
        return 6;
    case SHIFT_LEFT:
    case SHIFT_RIGHT:
    case RANGE:
        return 7;
    case LESS:
    case LESS_EQUAL:
    case GREATER:
    case GREATER_EQUAL:
    case INSTANCEOF:
    case IS:
    case IN_:
        return 9;
    case EQUAL:
    case NOT_EQUAL:
        return 10;
    case BITWISE_AND:
        return 11;
    case XOR:
        return 12;
    case BITWISE_OR:
        return 13;
    case AND:
        return 14;
    case OR:
        return 15;
    case BINARY_FUNCTION_OPERATOR:
        return 16;
    case CONDITIONAL:
        return 17;
    case ASSIGNMENT:
    case JSON_ASSIGNMENT:
    case INIT_ASSIGNMENT:
    case RETURN:
    case THROW:
    case SPREAD_OPERATOR:
        return 18;

    default:
        treeParser::throwLater("operator " + tokenToName(token) + " has no priority");
        return 100;
    }
}