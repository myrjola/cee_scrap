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

#ifndef CEE_SYMTAB_SYMBOL_TABLE__H
#define CEE_SYMTAB_SYMBOL_TABLE__H

/**
 * @file
 * @brief Defines the classes and functions for managing the symbol table.
 */

#include <list>
#include <string>
#include <map>

/**
 * \brief Defines an implementation of a symbol table.
 *
 * The symbol table contains the variables that are declared and used within the
 * code. The table contains a set of records (see Record), which are generated
 * as the AST is traversed.
 */
class SymbolTable {
public:
  class Record;

public:
  /**
   * Creates a symbol table.
   */
  SymbolTable(void);

  /**
   * Destroys this symbol table, and all records that it contains.
   */
  ~SymbolTable(void);

  /**
   * Gets the record with a given identifier name. If no such record is found,
   * \c NULL is returned.
   *
   * @param name
   *        Identifier name.
   * @returns Record with matching name.
   */
  Record* lookUp(const std::string& name) const;

  /**
   * Adds an identifier with a given name to this symbol table. If an
   * identifier with an identical name has already been inserted, no change is
   * made and false is returned.
   *
   * @param name
   *        Identifier name.
   * @param line
   *        Line number where the identifer was declared.
   * @param column
   *        Column number where the identifer was declared.
   * @returns \c true if the identifier was inserted.
   */
  bool insert(const std::string& name, int line, int column);

  /**
   * Gets a list of all records in the symbol table. The returned list need
   * not be any particular order (this method is used for inspection purposes
   * only).
   *
   * @returns All records.
   */
  std::list<Record*> getRecords(void) const;

  /**
   * Clears the entire symbol table and destroys all records.
   */
  void clear(void);

private:
  /* TASK: Add whatever private members and methods you find necessary */
  std::map<std::string, Record*> symbol_map;
  unsigned int memory_index_counter = 0;

public:
  /**
   * \brief Class for representing entries in the symbol table.
   *
   * The Record class defines the name of the identifier, the line number at
   * which it declard, and a new memory index. The latter will be used to
   * specify where to store the value of the identifier at run time.
   */
  class Record {
  public:
    /**
     * Creates a record for an identifier.
     *
     * @param name
     *        Name of the identifier.
     * @param line
     *        Line number at which the identifier was declared.
     * @param column
     *        Column number at which the identifier was declared.
     * @param memory_index
     *        Memory index at where the value of the identifier will
     *        be stored.
     */
    Record(
           const std::string& name,
           int line,
           int column,
           int memory_index);

    /**
     * Destroys this record.
     */
    ~Record(void);

    /**
     * Gets the identifier name of this record.
     *
     * @returns Identifier name.
     */
    std::string getName(void) const;

    /**
     * Gets the line number at which the identifier of this record was
     * declared
     *
     * @returns Line number.
     */
    int getLine(void) const;

    /**
     * Gets the column number at which the identifier of this record was
     * declared
     *
     * @returns Column number.
     */
    int getColumn(void) const;

    /**
     * Gets the memory index of this record.
     *
     * @returns Memory index.
     */
    int getMemoryIndex(void) const;

    /**
     * Sets the memory index to use for this record. Through liveness
     * analysis, two or more records can be set to share the same memory
     * location, thus reducing the memory footprint.
     *
     * @param index
     *        Memory index.
     */
    void setMemoryIndex(int index);
        
  private:
    /**
     * Name of the variable.
     */
    std::string _name;

    /**
     * Line number at which the variable was declared.
     */
    int _line;

    /**
     * Column number at which the variable was declared.
     */
    int _column;

    /**
     * Memory index assigned to the variable.
     */
    int _memory_index;
  };
};

#endif
