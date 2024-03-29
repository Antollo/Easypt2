{
    #include "gramUtility.h"
    #include "Node.h"
    #include "treeParser.h"

    #define op(x) operatorPriority(x)
    #define INI yylineno, treeParser::file

    extern int yylineno;
}

%options "verbose generate-lexer-wrapper generate-symbol-table lowercase-symbols";

%token ADDITION;
%token SUBTRACTION;
%token MULTIPLICATION;
%token DIVISION;
%token MODULUS;
%token XOR;
%token BITWISE_OR;
%token BITWISE_AND;
%token COMPLEMENT;
%token SHIFT_LEFT;
%token SHIFT_RIGHT;
%token ASSIGNMENT;
%token JSON_ASSIGNMENT;
%token INIT_ASSIGNMENT;
%token EQUAL;
%token NOT_EQUAL;
%token LESS;
%token LESS_EQUAL;
%token GREATER;
%token GREATER_EQUAL;
%token NOT;
%token AND;
%token OR;
%token BINARY_FUNCTION_OPERATOR;
%token RETURN;
%token THROW;
%token TRY;
%token CATCH;
%token LET;
%token FUNCTION;
%token JSON;
%token CLASS;
%token INCREMENT;
%token DECREMENT;
%token BRACKET_OPEN;
%token BRACKET_CLOSE;
%token PARENTHESES_OPEN;
%token PARENTHESES_CLOSE;
%token BRACES_OPEN;
%token BRACES_CLOSE;
%token DOT;
%token FOR;
%token WHILE;
%token IF;
%token ELSE;
%token IDENTIFIER;
%token NUMBER_LITERAL;
%token STRING_LITERAL;

%token READ_OPERATOR;
%token CALL_OPERATOR;
%token METHOD_CALL_OPERATOR;
%token STATEMENT;
%token COMPOUND_STATEMENT;
%token ARRAY_LITERAL;
%token IF_ELSE;
%token EXTENDS;
%token INSTANCEOF;

%token FAT_ARROW_BEGIN;
%token FAT_ARROW;
%token AWAIT;
%token DELETE_;
%token BREAK;

%token CONDITIONAL;
%token COMPOUND_STATEMENT_STACKLESS;
%token WHILE_COMPOUND_STATEMENT;
%token WHILE_COMPOUND_STATEMENT_STACKLESS;
%token FOR_COMPOUND_STATEMENT;
%token FOR_COMPOUND_STATEMENT_STACKLESS;
%token INIT_ASSIGNMENT_LET;
%token INIT_ASSIGNMENT_DOT;
%token INIT_ASSIGNMENT_IDENTIFIER;

%token SPREAD_OPERATOR;
%token RETURN_LAST;
%token DECORATOR;
%token IS;
%token IN_;
%token FOR_IN;
%token FOR_IN_COMPOUND_STATEMENT;
%token FOR_IN_COMPOUND_STATEMENT_STACKLESS;
%token RANGE;
%token RANGE_WITH_STEP;

%start parse, input;

input { Node a(INI); }
    :
    [
        statement(a) { treeParser::root->addChild(a); a = Node(INI); }
    ]*
    { treeParser::root->token(COMPOUND_STATEMENT); }
    ;

optionalSemicolon :
    [
        %while(1)
        ';'
    ] ?
    ;

simpleStatement(Node& me) { Node a(INI); } :
    expression(100, a) { me.addChild(a); }
    optionalSemicolon
    { me.token(STATEMENT); }
    ;

jsonStatement(Node& me) { Node a(INI); } :
    expression(100, a) { me.addChild(a); }
    [
        ','
    |
    ]
    { me.token(STATEMENT); }
    ;

compoundStatement(Node& me) { Node a(INI); } :
    BRACES_OPEN
    { a.init(INI); }
    [
        statement(a) { me.addChild(a); a = Node(INI); }
    ]*
    BRACES_CLOSE
    { me.token(COMPOUND_STATEMENT); }
    ;

jsonStatements(Node& me) { Node a(INI); } :
    [
        jsonStatement(a) { me.addChild(a); a = Node(INI); }
    ]*
    ;

ifStatement(Node& me) { Node a(INI), b(INI), c(INI); } :
    IF
    PARENTHESES_OPEN
    { a.init(INI); }
    expression(100, a) { me.addChild(a); }
    PARENTHESES_CLOSE
    { b.init(INI); }
    statement(b)
    { me.token(IF); me.addChild(b); }
    [
        %prefer
        ELSE
        { c.init(INI); }
        statement(c)
        { me.token(IF_ELSE); me.addChild(c); }
    
    |
    ]
    ;

whileStatement(Node& me) { Node a(INI), b(INI); } :
    WHILE
    PARENTHESES_OPEN
    { a.init(INI); }
    expression(100, a) { me.addChild(a); }
    PARENTHESES_CLOSE
    { b.init(INI); }
    statement(b)
    { me.token(WHILE); me.addChild(b); }
    ;

forStatement(Node& me) { Node a(INI), b(INI), c(INI), d(INI); bool normalFor = false; } :
    FOR
    PARENTHESES_OPEN
    { a.init(INI); }
    expression(100, a)
    [
        ';'
        { b.init(INI); }
        expression(100, b)
        ';'
        { c.init(INI); }
        expression(100, c)
        { normalFor = true; }
    |
    ]
    PARENTHESES_CLOSE
    { d.init(INI); }
    statement(d)
    { 
        if (normalFor)
        {
            me.token(FOR);
            me.addChild(a);
            me.addChild(b);
            me.addChild(c);
            me.addChild(d);
        }
        else
        {
            if (a.token() == IN_)
            {
                me.token(FOR_IN);
                me.children() = std::move(a.children());
                me.addChild(d);
            }
            else
                LLmessage(IN_, a.token());
        }
    }
    ;

tryCatchStatement(Node& me) { Node a(INI), b(INI); } :
    TRY
    { a.init(INI); }
    statement(a) { me.addChild(a); }
    CATCH
    { b.init(INI); }
    statement(b) { me.addChild(b); }
    { me.token(TRY); }
    ;

returnStatement(Node& me) { Node a(INI); } :
    RETURN
    expression(100, a)
    optionalSemicolon
    { me.token(RETURN); me.addChild(a); }
    ;

throwStatement(Node& me) { Node a(INI); } :
    THROW
    expression(100, a)
    optionalSemicolon
    { me.token(THROW); me.addChild(a); }
    ;

statement(Node& me) :
    [
        %avoid
        simpleStatement(me)
    |
        compoundStatement(me)
    |
        ifStatement(me)
    |
        whileStatement(me)
    |
        forStatement(me)
    |
        tryCatchStatement(me)
    |
        returnStatement(me)
    |
        throwStatement(me)
    |
        BREAK
        optionalSemicolon
        { me.token(BREAK); }
    ]
    ;

expression(int priority, Node& me) { Node a(INI), b(INI), c(INI); int token; } :
    factor(me)
    postUnaryOperator(me)
    [
        %while (op(LLsymb) <= priority)
        { a = Node(INI); std::swap(me, a); me.addChild(a); me.token(token = LLsymb); me.text(treeParser::text); }
        [
            leftAssociativeOperator
            expression(op(token) - 1, b)    { me.addChild(b); }
        |
            rightAssociativeOperator
            expression(op(token), b)        { me.addChild(b); }
        |
            CONDITIONAL expression(op(CONDITIONAL), b) { me.addChild(b); } 
            JSON_ASSIGNMENT expression(op(CONDITIONAL), c) { me.addChild(c); } 
        |
            RANGE expression(op(RANGE)-1, b) { me.addChild(b); }
            [
                %prefer
                RANGE expression(op(RANGE), c) { me.addChild(c); me.token(RANGE_WITH_STEP); }
            |
            ]
        |
            { me.token(CALL_OPERATOR); }
            PARENTHESES_OPEN expressionList(me) PARENTHESES_CLOSE
            postUnaryOperator(me)
        |
            { me.token(READ_OPERATOR); }
            BRACKET_OPEN expressionList(me) BRACKET_CLOSE
            postUnaryOperator(me)
        |
            DOT IDENTIFIER { b.token(IDENTIFIER); b.text(treeParser::text); me.addChild(b); me.token(DOT); }
            postUnaryOperator(me)
            [
                %prefer
                { me.token(METHOD_CALL_OPERATOR); }
                PARENTHESES_OPEN expressionList(me) PARENTHESES_CLOSE
                postUnaryOperator(me)
                |
            ]
        ]
    ]*
    ;

expressionList(Node& me) { Node a(INI); } :
    |
    expression(100, a) { me.addChild(a); } expressionListTail(me)
    ;

expressionListTail(Node& me) { Node a(INI); } :
    [
        ','
        { a.init(INI); }
        expression(100, a) { me.addChild(a); a = Node(INI); }
    ]*
    ;


postUnaryOperator(Node& me)  { Node a(INI); } :
    [
        %prefer
        INCREMENT { a.token(INCREMENT); a.addChild(me); me = std::move(a); }
    |
        %prefer
        DECREMENT { a.token(DECREMENT); a.addChild(me); me = std::move(a); }
    |
    ]
    ;

factor(Node& me)  { Node b(INI); } :
    PARENTHESES_OPEN expression(100, me) PARENTHESES_CLOSE
    |
    SUBTRACTION
    expression(op(MULTIPLICATION), b) { me.token(SUBTRACTION); Node a(INI); a.token(NUMBER_LITERAL); a.text("0"); me.addChild(a); me.addChild(b); }
    |
    COMPLEMENT
    expression(100, b) { me.token(COMPLEMENT); me.addChild(b); }
    |
    NOT
    expression(100, b) { me.token(NOT); me.addChild(b); }
    |
    AWAIT
    expression(100, b) { me.token(AWAIT); me.addChild(b); }
    |
    DELETE_
    expression(100, b) { me.token(DELETE_); me.addChild(b); }
    |
    DECORATOR
    { me.text(treeParser::text); }
    expression(100, b) { me.token(DECORATOR); me.addChild(b); }
    |
    SPREAD_OPERATOR
    expression(100, b) { me.token(SPREAD_OPERATOR); me.addChild(b); }
    |
    IDENTIFIER { me.token(IDENTIFIER); me.text(treeParser::text); }
    |
    NUMBER_LITERAL { me.token(NUMBER_LITERAL); me.text(treeParser::text); }
    |
    STRING_LITERAL { me.token(STRING_LITERAL); me.text(treeParser::text); }
    |
    LET
    IDENTIFIER { me.token(LET); me.text(treeParser::text); }
    |
    FUNCTION
    [
        IDENTIFIER { me.text(treeParser::text); }
    |
    ]
    [
        PARENTHESES_OPEN
        [
            IDENTIFIER { me.addName(treeParser::text); }
            ',' ..?
        ]*
        PARENTHESES_CLOSE
    |
    ]
    { b.init(INI); }
    compoundStatement(b) { me.token(FUNCTION); me.addChild(b); }
    |
    FAT_ARROW_BEGIN
    [
        IDENTIFIER { me.addName(treeParser::text); }
        ',' ..?
    ]*
    PARENTHESES_CLOSE
    FAT_ARROW
    { b.init(INI); }
    statement(b) { me.token(FUNCTION); me.addChild(b); }
    |
    BRACES_OPEN
    jsonStatements(me) { me.token(JSON); }
    BRACES_CLOSE
    |
    CLASS
    IDENTIFIER { me.text(treeParser::text); }
    [
        { b.init(INI); }
        EXTENDS
        IDENTIFIER { b.token(IDENTIFIER); b.text(treeParser::text); }
    |
        { b.init(INI); b.token(IDENTIFIER); b.text("Object"); }
    ]
    BRACES_OPEN
    jsonStatements(me) { me.token(CLASS); me.addChild(b); }
    BRACES_CLOSE
    |
    BRACKET_OPEN expressionList(me) BRACKET_CLOSE
    { me.token(ARRAY_LITERAL); }
    ;

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
    | BINARY_FUNCTION_OPERATOR
    | INSTANCEOF
    | IS
    | IN_
    ]
    ;

rightAssociativeOperator :
    [
    ASSIGNMENT
    | JSON_ASSIGNMENT
    | INIT_ASSIGNMENT
    ]
    ;
{}