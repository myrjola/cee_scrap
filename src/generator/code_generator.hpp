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

#ifndef CEE_GENERATOR_CODE_GENERATOR__H
#define CEE_GENERATOR_CODE_GENERATOR__H

/**
 * @file
 * @brief Defines the classes and functions for managing code generation.
 */

#include "code_listing.hpp"
#include "../ast/ast.hpp"
#include "../symtab/symbol_table.hpp"
#include <string>
#include <vector>

/**
 * \brief Handles code generation.
 *
 * The CodeGenerator class converts the AST into code. The class derives the
 * AST::DefaultVisitor and processes the tree in a bottom-up fashion (as the
 * reflect the true execution of the program).
 */
class CodeGenerator : public AST::DefaultVisitor {
  public:
    /**
     * Creates a code generator. By default the code generator will be in
     * "R-value mode".
     */
    CodeGenerator(void);

    /**
     * \copydoc AST::DefaultVisitor::~DefaultVisitor
     */
    virtual ~CodeGenerator(void);

    /**
     * Generates code from a given program node and symbol table. If \c false is
     * returned the state of the code is undefined.
     *
     * @param root
     *        Root node to process.
     * @param symtab
     *        Symbol table.
     * @param code
     *        Code destination vector.
     * @returns \c true if the generation was successful.
     */
    bool generate(
        AST::NProgram* root,
        const SymbolTable* symtab,
        std::vector<char>* code);

    /* TASK: Override the appropriate visitor methods */

  private:
    /* TASK: Add whatever private members and methods you find necessary */
};

#endif
