# Grammar

## Lexical grammar

### Identifier, whitespace and literals

```
identifier      : [_a-zA-Z][_a-zA-Z0-9]*
ws              : [ \t\n\r]*
float           : [0-9]+[.][0-9]+([eE]-?[0-9]+)?
decimal         : [1-9][0-9]*|0
hexadecimal     : 0[xX][0-9a-fA-F]+
octal           : 0[0-7]+
string          : \"[^"\\]*(\\.[^"\\]*)*\"
```

### Comments

```
\/\*            : BEGIN MULTILINE_COMMENT
\*\/            : END MULTILINE_COMMENT
```

```
\/\/            : BEGIN COMMENT
\n              : END COMMENT
<<EOF>>         : END COMMENT
```

### Keywords

```
class           : CLASS
extends         : EXTENDS
instanceOf      : INSTANCEOF
return          : RETURN
throw           : THROW
try             : TRY
catch           : CATCH
if              : IF
else            : ELSE
while           : WHILE
for             : FOR
let             : LET
function        : FUNCTION
await           : AWAIT
delete          : DELETE
break           : BREAK
```

### Identifier

```
{identifier}    : IDENTIFIER;
```

Identifiers starting with double underscores are 
reserved for internal interpreter implementation 
and should not be used.

### Literals

```
{float}         : NUMBER_LITERAL;
{decimal}       : NUMBER_LITERAL;
{hexadecimal}   : NUMBER_LITERAL;
{octal}         : NUMBER_LITERAL;
{string}        : STRING_LITERAL;
```

### "Fat arrow"

```
\(/({identifier},{ws})*{identifier}?\){ws}=>    : FAT_ARROW_BEGIN
=>                                              : FAT_ARROW
```

### Operators

```
@{identifier}@  : USER_OPERATOR
==              : EQUAL
!=              : NOT_EQUAL
\<              : LESS
\<=             : LESS_EQUAL
\>              : GREATER
\>=             : GREATER_EQUAL
\%              : MODULUS
=               : ASSIGNMENT
:               : JSON_ASSIGNMENT
\<\-            : INIT_ASSIGNMENT
!               : NOT
~               : COMPLEMENT
\+\+            : INCREMENT
\-\-            : DECREMENT
\+              : ADDITION
\-              : SUBTRACTION
\*              : MULTIPLICATION
\/              : DIVISION
\|\|            : OR
&&              : AND
\|              : BITWISE_OR
&               : BITWISE_AND
\<\<            : SHIFT_LEFT
\>\>            : SHIFT_RIGHT
,               : ','
;               : ';'
\.              : DOT
\:              : BRACES_OPEN
\}              : BRACES_CLOSE
\(              : PARENTHESES_OPEN
\)              : PARENTHESES_CLOSE
\[              : BRACKET_OPEN
\]              : BRACKET_CLOSE
\?              : CONDITIONAL
\.\.\.          : SPREAD_OPERATOR
```

## Grammar

```
input
    :
    [
        statement
    ]*
```

```
optionalSemicolon :
    [
        // while(true)
        ';'
    ] ?
```

```
simpleStatement :
    expression 
    optionalSemicolon
```

```
jsonStatement :
    expression
    [
        ','
    |
    ]
```

```
compoundStatement :
    BRACES_OPEN
    [
        statement
    ]*
    BRACES_CLOSE
```

```
jsonStatements :
    [
        jsonStatement
    ]*
```

```
ifStatement :
    IF
    PARENTHESES_OPEN
    expression
    PARENTHESES_CLOSE
    statement
    [
        // prefer
        ELSE
        statement
    |
    ]
```

```
whileStatement :
    WHILE
    PARENTHESES_OPEN
    expression
    PARENTHESES_CLOSE
    statement
```

```
forStatement :
    FOR
    PARENTHESES_OPEN
    expression
    ';'
    expression
    ';'
    expression
    PARENTHESES_CLOSE
    statement
```

```
tryCatchStatement :
    TRY
    statement
    CATCH
    statement
```

```
returnStatement :
    RETURN
    expression
    optionalSemicolon
```

```
throwStatement :
    THROW
    expression
    optionalSemicolon
```

```
statement :
    [
        // avoid
        simpleStatement
    |
        compoundStatement
    |
        ifStatement
    |
        whileStatement
    |
        forStatement
    |
        tryCatchStatement
    |
        returnStatement
    |
        throwStatement
    |
        BREAK
        optionalSemicolon
    ]
```

```
expression :
    factor
    [
        // while (newToken.priority <= expression.priority)
        [
            leftAssociativeOperator
            expression // parse with priority less by 1
        |
            rightAssociativeOperator
            expression
        |
            CONDITIONAL expression JSON_ASSIGNMENT expression
        |
            PARENTHESES_OPEN expressionList PARENTHESES_CLOSE
        |
            BRACKET_OPEN expressionList BRACKET_CLOSE
        |
            DOT IDENTIFIER
            [
                // prefer
                PARENTHESES_OPEN expressionList PARENTHESES_CLOSE
                |
            ]
        ]
    ]*
```

```
expressionList :
    |
    expression expressionListTail
```

```
expressionListTail :
    [
        ','
        expression
    ]*
```


```
factor :
    factorBase
    [
        // prefer
        INCREMENT
    |
        // prefer
        DECREMENT
    |
    ]
```

```
factorBase :
    PARENTHESES_OPEN expression PARENTHESES_CLOSE
    |
    SUBTRACTION
    expression
    |
    COMPLEMENT
    expression
    |
    NOT
    expression
    |
    AWAIT
    expression
    |
    DELETE
    expression
    |
    IDENTIFIER
    |
    NUMBER_LITERAL
    |
    STRING_LITERAL
    |
    LET
    IDENTIFIER
    |
    FUNCTION
    [
        IDENTIFIER
    |
    ]
    [
        PARENTHESES_OPEN
        [
            IDENTIFIER
            ',' ..?
        ]*
        PARENTHESES_CLOSE
    |
    ]
    compoundStatement
    |
    FAT_ARROW_BEGIN
    [
        IDENTIFIER
        ',' ..?
    ]*
    PARENTHESES_CLOSE
    FAT_ARROW
    statement
    |
    BRACES_OPEN
    jsonStatements
    BRACES_CLOSE
    |
    CLASS
    IDENTIFIER
    [
        EXTENDS
        IDENTIFIER
    |
    ]
    BRACES_OPEN
    jsonStatements
    BRACES_CLOSE
    |
    BRACKET_OPEN expressionList BRACKET_CLOSE
```

```
leftAssociativeOperator :
    [
    ADDITION
    | SUBTRACTION
    | MULTIPLICATION
    | DIVISION
    | MODULUS
    | XOR
    | BITWISE_OR
    | BITWISE_AND
    | SHIFT_LEFT
    | SHIFT_RIGHT
    | EQUAL
    | NOT_EQUAL
    | LESS
    | LESS_EQUAL
    | GREATER
    | GREATER_EQUAL
    | AND
    | OR
    | USER_OPERATOR
    | INSTANCEOF
    ]
```

```
rightAssociativeOperator :
    [
    ASSIGNMENT
    | JSON_ASSIGNMENT
    | INIT_ASSIGNMENT
    | NOT
    | COMPLEMENT
    ]
```