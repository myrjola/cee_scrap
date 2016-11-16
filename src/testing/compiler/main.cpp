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
 * USE: For testing the compiler. It reads a program file from the standard
 * input, and compiles the code.
 *
 * @author Gabriel Hjort Blindell <ghb@kth.se>
 */

#include "../../ast/ast.hpp"
#include "../../generator/code_generator.hpp"
#include "../../io/file_writer.hpp"
#include "../../io/reporter.hpp"
#include "../../symtab/symbol_table_builder.hpp"
#include "../../grammar/common.hpp" // Must be included before "parser.tab.h"
#include "../../grammar/parser.tab.h"
#include <ios>
#include <string>
#include <vector>

using namespace AST;
using std::ios_base;
using std::string;
using std::stringstream;
using std::vector;

// Declare the function which will invoke the parser to read the input and build
// the AST
extern int yyparse(void);

// Declare the global variable which will hold the output from the parser once
// it has finished parsing
extern NProgram* g_program;

int main(int argc, char** argv) {
    Reporter& out = *Reporter::getInstance();

    // Check command-line
    if (argc > 3) {
        out << out.beginError() << "Too many arguments. Use \"-h\" for help."
            << out.endl();
        return 1;
    }

    // Parse command-line
    bool default_output_file = true;
    string output_file;
    if (argc > 1) {
        bool has_error = false;
        string option(argv[1]);
        if (option[0] == '-') {
            if (option == "-h" || option == "--help") {
                out << "Usage: " << argv[0] << " [-h] [--help] [-o OUTPUT_FILE]"
                    << " < INPUT_FILE" << out.endl();
                return 0;
            }
            else if (option == "-o") {
                if (argc == 3) {
                    output_file = string(argv[2]);
                    default_output_file = false;
                }
                else {
                    has_error = true;
                }
            }
            else {
                has_error = true;
            }
        }
        else {
            has_error = true;
        }

        if (has_error) {
            out << out.beginError() << "Invalid option. Use \"-h\" for help."
                << out.endl();
            return 1;
        }
    }
    if (default_output_file) output_file = "program.o";

    // Read input and build AST (CTRL-d indicates end of input)
    yyparse();
    if (!g_program) return 0;

    // Build symbol table and check variable declarations
    SymbolTable symtab;
    SymbolTableBuilder symtab_builder;
    bool result = symtab_builder.build(g_program, &symtab);
    if (!result) return 0;

    // Generate code
    CodeGenerator generator;
    vector<char> code;
    result = generator.generate(g_program, &symtab, &code);
    if (!result) return 0;

    // Write to file
    FileWriter writer;
    try {
        writer.open(output_file);
    }
    catch (ios_base::failure) {
        out << out.beginError() << "Failed to open output file" << out.endl();
        return 1;
    }
    try {
        writer << code;
    }
    catch (ios_base::failure) {
        out << out.beginError() << "Failed to write to output file"
            << out.endl();
        return 1;
    }

    // Clean up
    delete g_program;

    return 0;
}
