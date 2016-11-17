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


  /**
   * Sets the mode to "L" mode.
   *
   * @param node
   *        Assignment node.
   * @throws NodeError
   *         Will not be thrown.
   */
  virtual void preVisit(AST::NAssignment* node) throw(AST::NodeError);

  /**
   * Sets the mode back to "R" mode.
   *
   * @param node
   *        Assignment node.
   * @throws NodeError
   *         Will not be thrown.
   */
  virtual void betweenChildren(AST::NAssignment* node) throw(AST::NodeError);

  /**
   * Adds the identifier on the left-hand side to the symbol table.
   *
   * @param node
   *        Assignment node.
   * @throws NodeError
   *         When a variable is redefined.
   */
  virtual void postVisit(AST::NAssignment* node) throw(AST::NodeError);

  /**
   * Visits the variable node and checks whether the variable has been
   * declared before use.
   *
   * @param node
   *        Variable node.
   * @throws NodeError
   *         When the variable is not available in the symbol table.
   */
  virtual void visit(AST::NVariable* node) throw(AST::NodeError);


private:
  /**
   * Contains the symbol table.
   */
  SymbolTable* symbol_table;

  /**
   * Flag for controlling "L" and "R" mode in assignment nodes. We will need
   * this to handle the very special case when you have a line which reads
   * <code>a = a;</code>.
   */
  bool right_side_mode;
};

#endif
