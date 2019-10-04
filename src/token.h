#ifndef TOKEN_H_
#define TOKEN_H_

#include <memory>
#include "name.h"

class object;
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
    token() : name("<no name>"s), arity(-1), type(tokenType::Invalid), value(nullptr){};
    token(const std::string &n, tokenType t = tokenType::Name);
    token(const std::string &n, int a, tokenType t = tokenType::CallOperator) : name(n), arity(a), type(t), value(nullptr){};
    token(const std::shared_ptr<object> &v) : name("<no name>"s), arity(-1), type(tokenType::Value), value(v){};
    std::shared_ptr<object> &resolve(stack *st) const;
    inline const name &getName() const { return name; }
    inline const int &getArity() const { return arity; }
    inline const tokenType &getType() const { return type; }

private:
    name name;
    int arity;
    tokenType type;
    mutable std::shared_ptr<object> value;
};

#endif /* !TOKEN_H_ */
