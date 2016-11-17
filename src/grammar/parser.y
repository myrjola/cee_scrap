%pure-parser
%locations
%error-verbose

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

%right T_EQUAL
%left T_MINUS T_PLUS
%left T_MUL T_DIV
%left UNARY

/* Sets the starting rule
  */
%start program

%%

/* Rules */

program : statement_list {
    g_program = $$ = new NProgram($1, @$.first_line, @$.first_column);
 };

statement_list:
statement {
    $$ = new NStatementList(@$.first_line, @$.first_column);
    (*$$) << $1;
}
| statement_list statement { (*$1) << $2; };

statement:
assignment { $<statement>$ = $1; }
| print { $<statement>$ = $1; };

assignment : variable T_EQUAL expr T_SEMICOLON {
    $$ = new NAssignment($1, $3, @$.first_line, @$.first_column);
};

print : T_PRINT expr T_SEMICOLON {
    $$ = new NPrint($2, @$.first_line, @$.first_column);
};

expr:
variable { $<variable>$ = $1; }
| number { $<number>$ = $1; }
| expr T_PLUS expr {
    $$ = new NExpressionBinary($1, PLUS, $3,
                               @$.first_line, @$.first_column);
}
| expr T_MINUS expr {
    $$ = new NExpressionBinary($1, MINUS, $3,
                               @$.first_line, @$.first_column);
}
| expr T_MUL expr {
    $$ = new NExpressionBinary($1, MUL, $3,
                               @$.first_line, @$.first_column);
}
| expr T_DIV expr {
    $$ = new NExpressionBinary($1, DIV, $3,
                               @$.first_line, @$.first_column);
}
| T_MINUS expr %prec UNARY {
    $$ = new NExpressionUnary(MINUS, $2,
                              @$.first_line, @$.first_column);
}
| T_LPAREN expr T_RPAREN { $$ = $2; };

variable: T_IDENTIFIER { $$ = new NVariable($1, @$.first_line, @$.first_column); };

number : T_NUMBER { $$ = new NNumber($1, @$.first_line, @$.first_column); };

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
