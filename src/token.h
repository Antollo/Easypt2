#error "deprecated"

#ifndef TOKEN_H_
#define TOKEN_H_

#include <memory>
#include "name.h"
#include "objectPtrImpl.h"

class stack;

class token
{
public:
    enum class tokenType
    {
        Operator,
        CallOperator,
        ReadOperator,
        Name,
        Value,
        StringLiteral,
        NumberLiteral,
        CompoundStatement,
        Invalid
    };
    token() : _name("<no name>"s), arityOrIndex(-1), type(tokenType::Invalid), value(nullptr){};
    token(const name &n) : _name(n), arityOrIndex(-1), type(tokenType::Name), value(nullptr){};
    token(const std::string &n, tokenType t = tokenType::Name);
    token(const std::string &n, int a, tokenType t = tokenType::CallOperator) : _name(n), arityOrIndex(a), type(t), value(nullptr){};
    token(std::string &&n, tokenType t = tokenType::Name);
    token(std::string &&n, int a, tokenType t = tokenType::CallOperator) : _name(n), arityOrIndex(a), type(t), value(nullptr){};
    token(const objectPtrImpl &v) : _name("<no name>"s), arityOrIndex(-1), type(tokenType::Value), value(v){};
    token(objectPtrImpl &&v) : _name("<no name>"s), arityOrIndex(-1), type(tokenType::Value), value(v){};
    objectPtrImpl &resolve(stack *st);
    inline const name &getName() const { return _name; }
    inline const int &getArity() const { return arityOrIndex; }
    inline const int &getCompoundStatementIndex() const { return arityOrIndex; }
    inline const tokenType &getType() const { return type; }

private:
    name _name;
    int arityOrIndex;
    tokenType type;
    objectPtrImpl value;
};

#endif /* !TOKEN_H_ */
