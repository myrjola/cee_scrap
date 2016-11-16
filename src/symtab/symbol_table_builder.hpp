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

#ifndef CEE_SYMTAB_SYMBOL_TABLE_BUILDER__H
#define CEE_SYMTAB_SYMBOL_TABLE_BUILDER__H

/**
 * @file
 * @brief Defines the classes and functions for building the symbol table.
 */

#include "../ast/ast.hpp"
#include "symbol_table.hpp"

/**
 * \brief Class for building the symbol table.
 *
 * The SymbolTableBuilder implements a visitor which will traverse the entire
 * AST and build a symbol table. The visitor will also check that each variable
 * has been declared before use, and produce an error if invalid use is
 * detected.
 */
class SymbolTableBuilder : public AST::DefaultVisitor {
  public:
    /**
     * Builds a symbol table and checks that no variables are redefined or
     * used before having been declared. If the program is semantically
     * invalid, an error is reported and \c false is returned.
     *
     * The given table will be cleared before the build to ensure a clean
     * table. If an error is encountered, the state of the symbol table is
     * undefined.
     *
     * @param node
     *        Program node.
     * @param symtab
     *        Symbol table to build upon.
     * @returns \c true if the symbol table was successfully built.
     */
    bool build(AST::NProgram* node, SymbolTable* symtab);

    /* TASK: Override the appropriate visitor methods */

  private:
    /* TASK: Add whatever private members and methods you find necessary */
};

#endif
