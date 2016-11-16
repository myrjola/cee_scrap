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
 * USE: For testing the symbol table. It reads from the standard input and
 * passes it to the parser, which will convert the input into an AST. A symbol
 * table is then built from the AST. The content of the resulting symbol table
 * is then printed to the standard output.
 */

#include "../../symtab/symbol_table.hpp"
#include "../../symtab/symbol_table_builder.hpp"
#include "../../grammar/common.hpp" // Must be included before "parser.tab.h"
#include "../../grammar/parser.tab.h"
#include <list>
#include <string>

using std::list;
using std::string;
using std::stringstream;

// Declare the function which will invoke the parser to read the input and build
// the AST
extern int yyparse(void);

// Declare the global variable which will hold the output from the parser once
// it has finished parsing
extern NProgram* g_program;

int main(int argc, char** argv) {
    Reporter& out = *Reporter::getInstance();

    // Check command-line
    if (argc > 1) {
        out << out.beginError() << "This program takes no arguments. Try "
            << argv[0] << " < " << argv[1] << " instead." << out.endl();
        return 1;
    }

    // Read input and build AST (CTRL-d indicates end of input)
    yyparse();
    if (!g_program) return 0;

    out << out.endl();

    // Build symbol table
    out << out.beginInfo() << "Building symbol table...";
    SymbolTable symtab;
    SymbolTableBuilder symtab_builder;
    bool result = symtab_builder.build(g_program, &symtab);
    if (!result) return 0;
    out << "OK" << out.endl();

    // List all symbols
    out << out.endl();
    list<SymbolTable::Record*> records = symtab.getRecords();
    list<SymbolTable::Record*>::iterator it;
    out << "SYMBOL TABLE RECORDS (in no particular order):" << out.endl();
    for (it = records.begin(); it != records.end(); it++) {
        SymbolTable::Record* record = *it;
        out << " * Variable: " << record->getName() << ", "
            << "Defined at: " << record->getLine() << ":" << record->getColumn()
            << ", "
            << "Memory index: " << record->getMemoryIndex()
            << out.endl();
    }

    // Clean up
    delete g_program;

    return 0;
}
