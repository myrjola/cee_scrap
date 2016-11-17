%option bison-bridge bison-locations
%option yylineno

%{
/*
 *  Copyright:
 *     Gabriel Hjort Blindell, 2012
 *     Martin Yrjölä, 2016
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "common.hpp"
#include "parser.tab.h"

/* Required to tell the lexer to stop when end-of-file is reached. */
extern "C" int yywrap(void) { return 1; }

/* Macro for saving of token strings. */
#define SAVE_TOKEN_STRING (yylval->token_string = yytext)

/* Manage token location. */
int yycolumn = 1;
#define YY_USER_ACTION \
    yylloc->first_line = yylloc->last_line = yylineno; \
    yylloc->first_column = yycolumn; \
    yylloc->last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;
%}

%%

    /* Core rules */

"print"                { return T_PRINT; }
"="                    { return T_EQUAL; }
";"                    { return T_SEMICOLON; }
[a-zA-Z][a-zA-Z0-9_]*  { SAVE_TOKEN_STRING; return T_IDENTIFIER; }
"+"                    { return T_PLUS; }
"-"                    { return T_MINUS; }
"*"                    { return T_MUL; }
"/"                    { return T_DIV; }
"("                    { return T_LPAREN; }
")"                    { return T_RPAREN; }
[0-9]+                 { SAVE_TOKEN_STRING; return T_NUMBER; }
[ \r\t]*

    /* Ignore new lines and reset column counter */
\n*                    { yycolumn = 1; }

    /* Indicate error on any other input */
.                      { SAVE_TOKEN_STRING; return T_UNKNOWN; }
