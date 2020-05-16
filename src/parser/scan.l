%{
    #include <stdexcept>
    #include <cstring>
    #include "gramUtility.h"
    #include "treeParser.h"

    #undef YY_INPUT
    #define YY_INPUT(b, r, s) (treeParser::read(b, &r, s))
%}
identifier [_a-zA-Z][_a-zA-Z0-9]*
ws [ \t\n\r]*
number [0-9]+([.][0-9]+)?
string \"[^"\\]*(\\.[^"\\]*)*\"
multilineComment \/\*([^*]*(\*[^\/])*)*\*\/
comment \/\/.*
%%
{multilineComment}          ;
{comment}$                  ;
{comment}/\0                ;
json                        { return JSON; }
class                       { return CLASS; }
return                      { return RETURN; }
throw                       { return THROW; }
try                         { return TRY; }
catch                       { return CATCH; }
if                          { return IF; }
else                        { return ELSE; }
while                       { return WHILE; }
for                         { return FOR; }
let                         { return LET; }
function                    { return FUNCTION; }

{identifier}                {
                                treeParser::text = yytext;
                                return IDENTIFIER;
                            }
{number}                    {
                                treeParser::text = yytext;
                                return NUMBER_LITERAL;
                            }
{string}                    {
                                treeParser::text = yytext;
                                return STRING_LITERAL;
                            }
@{identifier}@              { treeParser::text = std::string(yytext + 1, std::strlen(yytext) - 2); return USER_OPERATOR; }
==                          { treeParser::text = yytext; return EQUAL; }
!=                          { treeParser::text = yytext; return NOT_EQUAL; }
\<                          { treeParser::text = yytext; return LESS; }
\<=                         { treeParser::text = yytext; return LESS_EQUAL; }
\>                          { treeParser::text = yytext; return GREATER; }
\>=                         { treeParser::text = yytext; return GREATER_EQUAL; }
\%                          { treeParser::text = yytext; return MODULUS; }
\/                          { treeParser::text = yytext; return DIVISION; }
=                           { treeParser::text = yytext; return ASSIGNMENT; }
:                           { treeParser::text = yytext; return JSON_ASSIGNMENT; }
\<\-                        { treeParser::text = yytext; return INIT_ASSIGNMENT; }
!                           { treeParser::text = yytext; return NOT; }
\+\+                        { treeParser::text = yytext; return INCREMENT; }
\-\-                        { treeParser::text = yytext; return DECREMENT; }
\+                          { treeParser::text = yytext; return ADDITION; }
\-                          { treeParser::text = yytext; return SUBTRACTION; }
\*                          { treeParser::text = yytext; return MULTIPLICATION; }
\/                          { treeParser::text = yytext; return DIVISION; }

,                           { return ','; }
;                           { return ';'; }
\.                          { return DOT; }
\{                          { return BRACES_OPEN; }
\}                          { return BRACES_CLOSE; }
\(                          { return PARENTHESES_OPEN; }
\)                          { return PARENTHESES_CLOSE; }
\[                          { return BRACKET_OPEN; }
\]                          { return BRACKET_CLOSE; }
{ws}                        ;
.                           {

                                throw std::runtime_error("invalid character <" + std::string(1, yytext[0]) + "> in " + treeParser::file + ":" + std::to_string(yylineno));
                            }
%%
int yywrap(void) { return 1; }
