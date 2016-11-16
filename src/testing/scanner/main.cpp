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

/*
 * USE: For testing the scanner. It reads from the standard input and passes it
 * to the scanner, which will convert the input into tokens. Each token is
 * printed to the standard output.
 */

#include "../../grammar/common.hpp" // Must be included before "parser.tab.h"
#include "../../grammar/parser.tab.h"
#include "../../grammar/lex.yy.h"
#include "../../io/reporter.hpp"
#include <string>

using std::string;

YYSTYPE yylval;
YYLTYPE yylloc;

int main(int argc, char** argv) {
    Reporter& out = *Reporter::getInstance();

    // Check command-line
    if (argc > 1) {
        out << out.beginError() << "This program takes no arguments. Try "
            << argv[0] << " < " << argv[1] << " instead." << out.endl();
        return 1;
    }

    // Read input (CTRL-d indicates end of input)
    int token;
    while ((token = yylex(&yylval, &yylloc)) != 0) {
        switch (token) {
            case T_IDENTIFIER: {
                string value(yylval.token_string);
                if (value == "quit" || value == "exit") return 0;

                out << out.beginInfo() << "T_IDENTIFIER("
                    << value << ")" << out.endl();
                break;
            }

            case T_NUMBER: {
                out << out.beginInfo() << "T_NUMBER(" << yylval.token_string
                    << ")" << out.endl();
                break;
            }

            case T_EQUAL: {
                out << out.beginInfo() << "T_EQUAL" << out.endl();
                break;
            }

            case T_LPAREN: {
                out << out.beginInfo() << "T_LPAREN" << out.endl();
                break;
            }

            case T_RPAREN: {
                out << out.beginInfo() << "T_RPAREN" << out.endl();
                break;
            }

            case T_PLUS: {
                out << out.beginInfo() << "T_PLUS" << out.endl();
                break;
            }

            case T_MINUS: {
                out << out.beginInfo() << "T_MINUS" << out.endl();
                break;
            }

            case T_DIV: {
                out << out.beginInfo() << "T_DIV" << out.endl();
                break;
            }

            case T_MUL: {
                out << out.beginInfo() << "T_MUL" << out.endl();
                break;
            }

            case T_SEMICOLON: {
                out << out.beginInfo() << "T_SEMICOLON" << out.endl();
                break;
            }

            case T_PRINT: {
                out << out.beginInfo() << "T_PRINT" << out.endl();
                break;
            }
                
            case T_UNKNOWN: {
                out << out.beginError() << "Unknown input \""
                    << yylval.token_string << "\" at "
                    << yylloc.first_line << ":" << yylloc.first_column
                    << out.endl();
                break;
            }

            default: {
                out << out.beginError() << "Unknown token" << out.endl();
                break;
            }
        }
    }

    return 0;
}
