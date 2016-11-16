%pure-parser
%locations
%error-verbose

%{
/*
 *  Copyright:
 *     Gabriel Hjort Blindell, 2012
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

/* The top-level root node of the AST. */
NProgram* g_program = NULL;

/* Forward declarations. */
extern int yylex(union YYSTYPE* yyval_param, struct YYLTYPE* yylloc_param);
void yyerror(const char*);
%}



/* Container for holding data as the parsing is progressing (i.e. as terminals
   and non-terminals are converted into AST elements)
 */
%union {
    NProgram* program;
    NStatementList* statement_list;
    NStatement* statement;
    NExpression* expr;
    NNumber* number;
    NVariable* variable;
    char* token_string;
}

/* Defines the terminal tokens, and their data type with respect to the union
   structure. The tokens must match those used in the scanner.l flex file.
 */
%token <token_string> T_IDENTIFIER T_NUMBER
%token T_PRINT T_EQUAL T_LPAREN T_RPAREN T_SEMICOLON T_PLUS T_MINUS T_MUL T_DIV

/* For reporting errors */
%token <token_string> T_UNKNOWN

/* Defines the data type of the non-terminal tokens (again, with respect to the
   union structure)
 */
%type <program> program
%type <statement_list> statement_list
%type <statement> statement
%type <statement> assignment
%type <statement> print
%type <expr> expr
%type <variable> variable
%type <number> number

/* Sets the operator precedence (this also defines the tokens for the
   operators)
 */
/* TASK: Decide associativity order for operators (last means highest) */

/* Sets the starting rule
  */
%start program

%%

/* Rules */
/* TASK: Add missing rules */

%%

/**
 * Report error.
 *
 * @param str
 *        Error message.
 */
void yyerror(const char* str) {
    Reporter& out = *Reporter::getInstance();
    out << out.beginError() << str << out.endl();
}
