# Grammar

## Lexical grammar

### Identifier, whitespace and literals

```
identifier  : [_a-zA-Z][_a-zA-Z0-9]*
ws          : [ \t\n\r]*
number      : [0-9]+([.][0-9]+)?
string      : \"[^"\\]*(\\.[^"\\]*)*\"
```

### Comments

```
\/\*        : BEGIN MULTILINE_COMMENT
\*\/        : END MULTILINE_COMMENT
```

```
\/\/        : BEGIN COMMENT
\n          : END COMMENT
<<EOF>>     : END COMMENT
```

### Keywords

```
class       : return CLASS
extends     : return EXTENDS
instanceOf  : return INSTANCEOF
return      : return RETURN
throw       : return THROW
try         : return TRY
catch       : return CATCH
if          : return IF
else        : return ELSE
while       : return WHILE
for         : return FOR
let         : return LET
function    : return FUNCTION
await       : return AWAIT
delete      : return DELETE_
break       : return BREAK
```

### Identifier

```
{identifier}    : return IDENTIFIER;
```

### Literals

```
{number}    : return NUMBER_LITERAL;
{string}    : return STRING_LITERAL;
```

### "Fat arrow"

```
\(/({identifier},{ws})*{identifier}?\){ws}=>    : return FAT_ARROW_BEGIN
=>                                              : return FAT_ARROW
```

### Operators

```
@{identifier}@  : return USER_OPERATOR
==              : return EQUAL
!=              : return NOT_EQUAL
\<              : return LESS
\<=             : return LESS_EQUAL
\>              : return GREATER
\>=             : return GREATER_EQUAL
\%              : return MODULUS
=               : return ASSIGNMENT
:               : return JSON_ASSIGNMENT
\<\-            : return INIT_ASSIGNMENT
!               : return NOT
~               : return COMPLEMENT
\+\+            : return INCREMENT
\-\-            : return DECREMENT
\+              : return ADDITION
\-              : return SUBTRACTION
\*              : return MULTIPLICATION
\/              : return DIVISION
\|\|            : return OR
&&              : return AND
\|              : return BITWISE_OR
&               : return BITWISE_AND
\<\<            : return SHIFT_LEFT
\>\>            : return SHIFT_RIGHT
,               : return ','
;               : return ';'
\.              : return DOT
\:              : return BRACES_OPEN
\}              : return BRACES_CLOSE
\(              : return PARENTHESES_OPEN
\)              : return PARENTHESES_CLOSE
\[              : return BRACKET_OPEN
\]              : return BRACKET_CLOSE
```

## Grammar

```
input
    :
    [
        statement
    ]*
    ;
```

```
simpleStatement :
    expression ';'
    ;
```

```
jsonStatement :
    expression
    [
        ','
    |
    ]
    ;
```

```
compoundStatement :
    BRACES_OPEN
    [
        statement
    ]*
    BRACES_CLOSE
    ;
```

```
jsonStatements :
    [
        jsonStatement
    ]*
    ;
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
    ;
```

```
whileStatement :
    WHILE
    PARENTHESES_OPEN
    expression
    PARENTHESES_CLOSE
    statement
    ;
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
    ;
```

```
tryCatchStatement :
    TRY
    statement
    CATCH
    statement
    ;
```

```
returnStatement :
    RETURN
    expression
    ';'
    ;
```

```
throwStatement :
    THROW
    expression
    ';'
    ;
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
        ';'
    ]
    ;
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
    ;
```

```
expressionList :
    |
    expression expressionListTail
    ;
```

```
expressionListTail :
    [
        ','
        expression
    ]*
    ;
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
    ;
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
    DELETE_
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
    ;
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
    ;
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
    ;
```