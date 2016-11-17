#include "symbol_table.hpp"
#include <cstddef>

using std::string;

SymbolTable::SymbolTable(void) {}

SymbolTable::~SymbolTable(void) {
  clear();
}

SymbolTable::Record* SymbolTable::lookUp(const string& name) const {
  try {
    return symbol_map.at(name);
  } catch (const std::out_of_range& e) {
    return NULL;
  }
}

bool SymbolTable::insert(const string& name, int line, int column) {
  Record* record = new Record(name, line, column, memory_index_counter++);
  symbol_map.insert(std::make_pair(name, record));
  return true;
}

void SymbolTable::clear(void) {
  // Destroy all records
  std::map<string, Record*>::const_iterator it;
  for (it = symbol_map.begin(); it != symbol_map.end(); it++) {
    delete it->second;
  }

  symbol_map.clear();
}

std::list<SymbolTable::Record*> SymbolTable::getRecords(void) const {
  std::list<Record*> records;
  std::transform(symbol_map.begin(), symbol_map.end(),
                 std::back_inserter(records),
                 [](std::pair<string, Record*> val) {
                   return val.second;
                 });
  return records;
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
