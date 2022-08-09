%{
    #include <stdexcept>
    #include <cstring>
    #include "gramUtility.h"
    #include "treeParser.h"

    #undef YY_INPUT
    #define YY_INPUT(b, r, s) (treeParser::read(b, &r, s))
%}

%option never-interactive yylineno

identifier [_a-zA-Z][_a-zA-Z0-9]*
ws [ \t\n\r]*
float [0-9]+[.][0-9]+([eE]-?[0-9]+)?
decimal [1-9][0-9]*|0
hexadecimal 0[xX][0-9a-fA-F]+
octal 0[0-7]+
string \"[^"\\]*(\\.[^"\\]*)*\"
multilineComment \/\*([^*]*(\*[^\/])*)*\*\/
comment \/\/.*

%s COMMENT
%s MULTILINE_COMMENT

%%
<INITIAL>\/\*               { BEGIN MULTILINE_COMMENT; }
<MULTILINE_COMMENT>\*\/     { BEGIN INITIAL; }
<MULTILINE_COMMENT>\n       ;
<MULTILINE_COMMENT>.        ;
<MULTILINE_COMMENT><<EOF>>  {
                                treeParser::throwLater("multiline comment not closed in " + treeParser::file + ":" + std::to_string(yylineno)); 
                                BEGIN INITIAL;
                            }

<INITIAL>\/\/               { BEGIN COMMENT; }
<COMMENT>\n                 { BEGIN INITIAL; }
<COMMENT><<EOF>>            { BEGIN INITIAL; }
<COMMENT>.                  ;

<INITIAL>class              { return CLASS; }
<INITIAL>extends            { return EXTENDS; }
<INITIAL>instanceOf         { return INSTANCEOF; }
<INITIAL>return             { return RETURN; }
<INITIAL>throw              { return THROW; }
<INITIAL>try                { return TRY; }
<INITIAL>catch              { return CATCH; }
<INITIAL>if                 { return IF; }
<INITIAL>else               { return ELSE; }
<INITIAL>while              { return WHILE; }
<INITIAL>for                { return FOR; }
<INITIAL>let                { return LET; }
<INITIAL>function           { return FUNCTION; }
<INITIAL>await              { return AWAIT; }
<INITIAL>delete             { return DELETE_; }
<INITIAL>break              { return BREAK; }
<INITIAL>is                 { return IS; }
<INITIAL>in                 { return IN_; }
<INITIAL>\.\.\.             { return SPREAD_OPERATOR; }

<INITIAL>{identifier}       {
                                treeParser::text = yytext;
                                return IDENTIFIER;
                            }
<INITIAL>{float}            {
                                treeParser::text = yytext;
                                return NUMBER_LITERAL;
                            }
<INITIAL>{decimal}          {
                                treeParser::text = yytext;
                                return NUMBER_LITERAL;
                            }
<INITIAL>{hexadecimal}      {
                                treeParser::text = yytext;
                                return NUMBER_LITERAL;
                            }
<INITIAL>{octal}            {
                                treeParser::text = yytext;
                                return NUMBER_LITERAL;
                            }
<INITIAL>{string}           {
                                treeParser::text = yytext;
                                return STRING_LITERAL;
                            }
<INITIAL>\(/({identifier},{ws})*{identifier}?\){ws}=> { return FAT_ARROW_BEGIN; }
<INITIAL>=>                 { return FAT_ARROW; }
<INITIAL>@{identifier}@     { treeParser::text = std::string(yytext + 1, std::strlen(yytext) - 2); return BINARY_FUNCTION_OPERATOR; }
<INITIAL>@{identifier}      { treeParser::text = std::string(yytext + 1, std::strlen(yytext) - 1); return DECORATOR; }
<INITIAL>==                 { treeParser::text = yytext; return EQUAL; }
<INITIAL>!=                 { treeParser::text = yytext; return NOT_EQUAL; }
<INITIAL>\<                 { treeParser::text = yytext; return LESS; }
<INITIAL>\<=                { treeParser::text = yytext; return LESS_EQUAL; }
<INITIAL>\>                 { treeParser::text = yytext; return GREATER; }
<INITIAL>\>=                { treeParser::text = yytext; return GREATER_EQUAL; }
<INITIAL>\%                 { treeParser::text = yytext; return MODULUS; }
<INITIAL>=                  { treeParser::text = yytext; return ASSIGNMENT; }
<INITIAL>:                  { treeParser::text = yytext; return JSON_ASSIGNMENT; }
<INITIAL>\<\-               { treeParser::text = yytext; return INIT_ASSIGNMENT; }
<INITIAL>!                  { treeParser::text = yytext; return NOT; }
<INITIAL>~                  { treeParser::text = yytext; return COMPLEMENT; }
<INITIAL>\+\+               { treeParser::text = yytext; return INCREMENT; }
<INITIAL>\-\-               { treeParser::text = yytext; return DECREMENT; }
<INITIAL>\+                 { treeParser::text = yytext; return ADDITION; }
<INITIAL>\-                 { treeParser::text = yytext; return SUBTRACTION; }
<INITIAL>\*                 { treeParser::text = yytext; return MULTIPLICATION; }
<INITIAL>\/                 { treeParser::text = yytext; return DIVISION; }
<INITIAL>\|\|               { treeParser::text = yytext; return OR; }
<INITIAL>&&                 { treeParser::text = yytext; return AND; }
<INITIAL>\|                 { treeParser::text = yytext; return BITWISE_OR; }
<INITIAL>&                  { treeParser::text = yytext; return BITWISE_AND; }
<INITIAL>\<\<               { treeParser::text = yytext; return SHIFT_LEFT; }
<INITIAL>\>\>               { treeParser::text = yytext; return SHIFT_RIGHT; }
<INITIAL>,                  { return ','; }
<INITIAL>;                  { return ';'; }
<INITIAL>\.                 { return DOT; }
<INITIAL>\{                 { return BRACES_OPEN; }
<INITIAL>\}                 { return BRACES_CLOSE; }
<INITIAL>\(                 { return PARENTHESES_OPEN; }
<INITIAL>\)                 { return PARENTHESES_CLOSE; }
<INITIAL>\[                 { return BRACKET_OPEN; }
<INITIAL>\]                 { return BRACKET_CLOSE; }
<INITIAL>\?                 { return CONDITIONAL; }
<INITIAL>{ws}               ;
<INITIAL>.                  {
                                treeParser::throwLater("invalid character <" + std::string(1, yytext[0]) + "> in " + treeParser::file + ":" + std::to_string(yylineno));
                            }
%%
int yywrap(void) { return 1; }
