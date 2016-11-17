
#include "symbol_table.hpp"
#include <cstddef>

using std::string;

SymbolTable::SymbolTable(void)
{
  /* TASK: Add whatever code you find necessary */
}

SymbolTable::~SymbolTable(void) {
  /* TASK: Add whatever code you find necessary */
}

SymbolTable::Record* SymbolTable::lookUp(const string& name) const {
  /* TASK: Implement this method */
}

bool SymbolTable::insert(const string& name, int line, int column) {
  /* TASK: Implement this method */
}

void SymbolTable::clear(void) {
  /* TASK: Implement this method */
}

list<SymbolTable::Record*> SymbolTable::getRecords(void) const {
  /* TASK: Implement this method */
}

SymbolTable::Record::Record(
                            const string& name,
                            int line,
                            int column,
                            int memory_index)
  : _name(name),
    _line(line),
    _column(column),
    _memory_index(memory_index)
{}

SymbolTable::Record::~Record(void) {}

string SymbolTable::Record::getName(void) const {
  return _name;
}

int SymbolTable::Record::getLine(void) const {
  return _line;
}

int SymbolTable::Record::getColumn(void) const {
  return _column;
}

int SymbolTable::Record::getMemoryIndex(void) const {
  return _memory_index;
}

void SymbolTable::Record::setMemoryIndex(int index) {
  _memory_index = index;
}
