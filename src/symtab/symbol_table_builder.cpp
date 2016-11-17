#include <sstream>

#include "symbol_table_builder.hpp"
#include "../io/reporter.hpp"


using namespace AST;

bool SymbolTableBuilder::build(NProgram* node, SymbolTable* symtab) {
  symbol_table = symtab;
  symbol_table->clear();
  right_side_mode = true;
  try {
    node->accept(this);
    return true;
  }
  catch (NodeError& ex) {
    Reporter& out = *Reporter::getInstance();
    out << out.beginError() << ex.what() << out.endl();
    return false;
  }
}

void SymbolTableBuilder::preVisit(AST::NAssignment* node) throw(AST::NodeError)
{
  right_side_mode = false;
}

void SymbolTableBuilder::betweenChildren(NAssignment* node) throw(NodeError) {
  right_side_mode = true;
}

void SymbolTableBuilder::postVisit(AST::NAssignment* node) throw(AST::NodeError) {
  NVariable* variable = node->getVariable();
  std::string name = variable->getName();

  // Add the identifier to the symbol table
  bool result = symbol_table->insert(variable->getName(),
                                     variable->getLine(),
                                     variable->getColumn());
  if (!result) {
    SymbolTable::Record* record = symbol_table->lookUp(name);
    std::stringstream ss;
    ss << "Redefinition of variable at "
       << node->getLine() << ":" << node->getColumn() << "; "
       << "\"" << variable->getName() << "\" was already declared at "
       << record->getLine() << ":" << record->getColumn();
    throw NodeError(ss.str());
  }
}

void SymbolTableBuilder::visit(NVariable* node) throw(NodeError) {
  if (!right_side_mode) return;

  // Check that the identifier has already been declared
  if (!symbol_table->lookUp(node->getName())) {
    std::stringstream ss;
    ss << "Invalid use of variable at "
       << node->getLine() << ":" << node->getColumn() << "; "
       << "\"" << node->getName() << "\" has not yet been declared";
    throw NodeError(ss.str());
  }
}
