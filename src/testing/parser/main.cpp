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
 * USE: For testing the parser. It reads from the standard input and passes it
 * to the parser, which will convert the input into an AST. The AST will then
 * be printed to the standard output using a pretty-printing visitor.
 */

#include "../../grammar/common.hpp" // Must be included before "parser.tab.h"
#include "../../grammar/parser.tab.h"
#include <sstream>

using std::string;
using std::stringstream;

// Declare the function which will invoke the parser to read the input and build
// the AST
extern int yyparse(void);

// Declare the global variable which will hold the output from the parser once
// it has finished parsing
extern NProgram* g_program;

/**
 * Implements a visitor which will traverse the entire AST and print it to
 * standard out in a "pretty-looking" fashion.
 */
class AstPrettyPrinter : public DefaultVisitor {
  public:
    AstPrettyPrinter(void) : _level(0), _out(*Reporter::getInstance()) {}

    void print(Node* node) {
        node->accept(this);
    }

    virtual void preVisit(Node* node) throw(NodeError) {}

    virtual void visit(Node* node) throw(NodeError) {}

    virtual void postVisit(Node* node) throw(NodeError) {}

    virtual void preVisit(NProgram* node) throw(NodeError) {}

    virtual void visit(NProgram* node) throw(NodeError) {
        _out << _out.beginInfo() << indentation() << "[NProgram] at "
             << location(node) << _out.endl();
    }

    virtual void postVisit(NProgram* node) throw(NodeError) {}

    virtual void preVisit(NStatementList* node) throw(NodeError) {
        _level++;
    }

    virtual void visit(NStatementList* node) throw(NodeError) {
        _out << _out.beginInfo() << indentation() << "[NStatementList] at "
             << location(node) << _out.endl();
    }

    virtual void postVisit(NStatementList* node) throw(NodeError) {
        _level--;
    }

    virtual void preVisit(NPrint* node) throw(NodeError) {
        _level++;
    }

    virtual void visit(NPrint* node) throw(NodeError) {
        _out << _out.beginInfo() << indentation() << "[NPrint] at "
             << location(node) << _out.endl();
    }

    virtual void postVisit(NPrint* node) throw(NodeError) {
        _level--;
    }

    virtual void preVisit(NAssignment* node) throw(NodeError) {
        _level++;
    }

    virtual void visit(NAssignment* node) throw(NodeError) {
        _out << _out.beginInfo() << indentation() << "[NAssignment] at "
             << location(node) << _out.endl();
    }

    virtual void postVisit(NAssignment* node) throw(NodeError) {
        _level--;
    }

    virtual void preVisit(NExpressionUnary* node) throw(NodeError) {
        _level++;
    }

    virtual void visit(NExpressionUnary* node) throw(NodeError) {
        _out << _out.beginInfo() << indentation() << "[NExpressionUnary("
             << opEnumToChar(node->getOperator()) << ")] at " << location(node) 
             << _out.endl();
    }

    virtual void postVisit(NExpressionUnary* node) throw(NodeError) {
        _level--;
    }

    virtual void preVisit(NExpressionBinary* node) throw(NodeError) {
        _level++;
    }

    virtual void visit(NExpressionBinary* node) throw(NodeError) {
        _out << _out.beginInfo() << indentation() << "[NExpressionBinary("
             << opEnumToChar(node->getOperator()) << ")] at " << location(node) 
             << _out.endl();
    }

    virtual void postVisit(NExpressionBinary* node) throw(NodeError) {
        _level--;
    }

    virtual void preVisit(NVariable* node) throw(NodeError) {
        _level++;
    }

    virtual void visit(NVariable* node) throw(NodeError) {
        _out << _out.beginInfo() << indentation() << "[NVariable("
             << node->getName() << ")] at " << location(node) << _out.endl();
    }

    virtual void postVisit(NVariable* node) throw(NodeError) {
        _level--;
    }

    virtual void preVisit(NNumber* node) throw(NodeError) {
        _level++;
    }

    virtual void visit(NNumber* node) throw(NodeError) {
        _out << _out.beginInfo() << indentation() << "[NNumber("
             << node->getNumber() << ")] at " << location(node) << _out.endl();
    }

    virtual void postVisit(NNumber* node) throw(NodeError) {
        _level--;
    }

  private:
    string indentation(void) {
        string str;
        for (int i = 0; i < _level; ++i) {
            str += INDENT;
        }
        return str;
    }

    string location(Node* node) {
        stringstream ss;
        ss << node->getLine() << ":" << node->getColumn();
        return ss.str();
    }

    char opEnumToChar(ExpressionOperator op) {
        switch (op) {
            case PLUS:  return '+';
            case MINUS: return '-';
            case MUL:   return '*';
            case DIV:   return '/';
            default:    return '?';
        }
    }

  public:
    static const string INDENT;

  private:
    int _level;
    Reporter& _out;
};

const string AstPrettyPrinter::INDENT("  ");

int main(int argc, char** argv) {
    // Check command-line
    if (argc > 1) {
        Reporter& out = *Reporter::getInstance();
        out << out.beginError() << "This program takes no arguments. Try "
            << argv[0] << " < " << argv[1] << " instead." << out.endl();
        return 1;
    }

    // Read input and produce AST (CTRL-d indicates end of input)
    yyparse();
    if (!g_program) return 0;

    // Print AST
    AstPrettyPrinter printer;
    printer.print(g_program);

    // Clean up
    delete g_program;

    return 0;
}
