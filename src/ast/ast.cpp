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

#include "ast.hpp"

using namespace AST;
using std::list;
using std::string;

Node::Node(int line, int column) : _line(line), _column(column) {}

Node::~Node(void) {}

int Node::getLine(void) const {
    return _line;
}

int Node::getColumn(void) const {
    return _column;
}

NProgram::NProgram(
    NStatementList* statements,
    int line,
    int column)
        : Node(line, column), _statements(statements)
{}

NProgram::~NProgram(void) {
    delete _statements;
}

NStatementList* NProgram::getStatementList(void) const {
    return _statements;
}

void NProgram::accept(IVisitor* visitor) throw(NodeError) {
    visitor->preVisit(this);
    visitor->visit(this);
    _statements->accept(visitor);
    visitor->postVisit(this);
}

NStatementList::NStatementList(int line, int column) : Node(line, column) {}

NStatementList::~NStatementList(void) {
    list<NStatement*>::iterator it;
    for (it = _statements.begin(); it != _statements.end(); it++) {
        delete *it;
    }
}

void NStatementList::appendStatement(NStatement* node) {
    _statements.push_back(node);
}

NStatementList& NStatementList::operator<<(NStatement* node) {
    appendStatement(node);
    return *this;
}

list<NStatement*> NStatementList::getStatements(void) {
    return _statements;
}

void NStatementList::accept(IVisitor* visitor) throw(NodeError) {
    visitor->preVisit(this);
    visitor->visit(this);
    switch (visitor->getChildVisitOrder(this)) {
        case IVisitor::NORMAL: {
            list<NStatement*>::iterator it;
            for (it = _statements.begin(); it != _statements.end(); it++) {
                (*it)->accept(visitor);
                if (it != --_statements.end()) {
                    visitor->betweenChildren(this);
                }
            }
            break;
        }

        case IVisitor::REVERSED: {
            list<NStatement*>::reverse_iterator it;
            for (it = _statements.rbegin(); it != _statements.rend(); it++) {
                (*it)->accept(visitor);
                if (it != --_statements.rend()) {
                    visitor->betweenChildren(this);
                }
            }
            break;
        }

        default:
            throw NodeError("Unknown visit order");
    }
    visitor->postVisit(this);
}

NStatement::NStatement(int line, int column) : Node(line, column) {}

NStatement::~NStatement(void) {}

NAssignment::NAssignment(
    NVariable* variable,
    NExpression* expr,
    int line,
    int column)
        : NStatement(line, column), _variable(variable), _expr(expr)
{}

NAssignment::~NAssignment(void) {
    delete _variable;
    delete _expr;
}

NVariable* NAssignment::getVariable(void) const {
    return _variable;
}

NExpression* NAssignment::getExpression(void) const {
    return _expr;
}

void NAssignment::accept(IVisitor* visitor) throw(NodeError) {
    visitor->preVisit(this);
    visitor->visit(this);
    switch (visitor->getChildVisitOrder(this)) {
        case IVisitor::NORMAL: {
            _variable->accept(visitor);
            visitor->betweenChildren(this);
            _expr->accept(visitor);
            break;
        }

        case IVisitor::REVERSED: {
            _expr->accept(visitor);
            visitor->betweenChildren(this);
            _variable->accept(visitor);
            break;
        }

        default:
            throw NodeError("Unknown visit order");
    }
    visitor->postVisit(this);
}

NPrint::NPrint(NExpression* expr, int line, int column)
        : NStatement(line, column), _expr(expr)
{}

NPrint::~NPrint(void) {}

NExpression* NPrint::getExpression(void) const {
    return _expr;
}

void NPrint::accept(IVisitor* visitor) throw(NodeError) {
    visitor->preVisit(this);
    visitor->visit(this);
    _expr->accept(visitor);
    visitor->postVisit(this);
}

NExpression::NExpression(int line, int column) : Node(line, column) {}

NExpression::~NExpression(void) {}

NExpressionUnary::NExpressionUnary(
    ExpressionOperator op,
    NExpression* expr,
    int line,
    int column)
        : NExpression(line, column), _op(op), _expr(expr)
{}

NExpressionUnary::~NExpressionUnary(void) {
    delete _expr;
}

NExpression* NExpressionUnary::getExpression(void) const {
    return _expr;
}

ExpressionOperator NExpressionUnary::getOperator(void) const {
    return _op;
}

void NExpressionUnary::accept(IVisitor* visitor) throw(NodeError) {
    visitor->preVisit(this);
    visitor->visit(this);
    _expr->accept(visitor);
    visitor->postVisit(this);
}

NExpressionBinary::NExpressionBinary(
    NExpression* lhs,
    ExpressionOperator op,
    NExpression* rhs,
    int line,
    int column)
        : NExpression(line, column), _lhs(lhs), _op(op), _rhs(rhs)
{}

NExpressionBinary::~NExpressionBinary(void) {
    delete _lhs;
    delete _rhs;
}

NExpression* NExpressionBinary::getLhsExpression(void) const {
    return _lhs;
}

ExpressionOperator NExpressionBinary::getOperator(void) const {
    return _op;
}

NExpression* NExpressionBinary::getRhsExpression(void) const {
    return _rhs;
}

void NExpressionBinary::accept(IVisitor* visitor) throw(NodeError) {
    visitor->preVisit(this);
    visitor->visit(this);
    switch (visitor->getChildVisitOrder(this)) {
        case IVisitor::NORMAL: {
            _lhs->accept(visitor);
            visitor->betweenChildren(this);
            _rhs->accept(visitor);
            break;
        }

        case IVisitor::REVERSED: {
            _rhs->accept(visitor);
            visitor->betweenChildren(this);
            _lhs->accept(visitor);
            break;
        }

        default:
            throw NodeError("Unknown visit order");
    }
    visitor->postVisit(this);
}

NNumber::NNumber(const string& number, int line, int column)
        : NExpression(line, column), _number(number) {}

NNumber::~NNumber(void) {}

string NNumber::getNumber(void) const {
    return _number;
}

void NNumber::accept(IVisitor* visitor) throw(NodeError) {
    visitor->preVisit(this);
    visitor->visit(this);
    visitor->postVisit(this);
}

NVariable::NVariable(const string& name, int line, int column)
        : NExpression(line, column), _name(name) {}

NVariable::~NVariable(void) {}

string NVariable::getName(void) const {
    return _name;
}

void NVariable::accept(IVisitor* visitor) throw(NodeError) {
    visitor->preVisit(this);
    visitor->visit(this);
    visitor->postVisit(this);
}

IVisitor::~IVisitor(void) {}

DefaultVisitor::~DefaultVisitor(void) {}

void DefaultVisitor::preVisit(Node* node) throw(NodeError) {}

void DefaultVisitor::visit(Node* node) throw(NodeError) {}

void DefaultVisitor::postVisit(Node* node) throw(NodeError) {}

void DefaultVisitor::preVisit(NProgram* node) throw(NodeError) {}

void DefaultVisitor::visit(NProgram* node) throw(NodeError) {}

void DefaultVisitor::postVisit(NProgram* node) throw(NodeError) {}

void DefaultVisitor::preVisit(NStatementList* node) throw(NodeError) {}

void DefaultVisitor::visit(NStatementList* node) throw(NodeError) {}

void DefaultVisitor::postVisit(NStatementList* node) throw(NodeError) {}

void DefaultVisitor::betweenChildren(NStatementList* node) throw(NodeError) {}

IVisitor::VisitOrder DefaultVisitor::getChildVisitOrder(NStatementList* node)
    throw(NodeError)
{
    return IVisitor::NORMAL;
}

void DefaultVisitor::preVisit(NStatement* node) throw(NodeError) {}

void DefaultVisitor::visit(NStatement* node) throw(NodeError) {}

void DefaultVisitor::postVisit(NStatement* node) throw(NodeError) {}

void DefaultVisitor::preVisit(NAssignment* node) throw(NodeError) {}

void DefaultVisitor::visit(NAssignment* node) throw(NodeError) {}

void DefaultVisitor::postVisit(NAssignment* node) throw(NodeError) {}

void DefaultVisitor::betweenChildren(NAssignment* node) throw(NodeError) {}

IVisitor::VisitOrder DefaultVisitor::getChildVisitOrder(NAssignment* node)
    throw(NodeError)
{
    return IVisitor::NORMAL;
}

void DefaultVisitor::preVisit(NPrint* node) throw(NodeError) {}

void DefaultVisitor::visit(NPrint* node) throw(NodeError) {}

void DefaultVisitor::postVisit(NPrint* node) throw(NodeError) {}

void DefaultVisitor::preVisit(NExpression* node) throw(NodeError) {}

void DefaultVisitor::visit(NExpression* node) throw(NodeError) {}

void DefaultVisitor::postVisit(NExpression* node) throw(NodeError) {}

void DefaultVisitor::preVisit(NExpressionUnary* node) throw(NodeError) {}

void DefaultVisitor::visit(NExpressionUnary* node) throw(NodeError) {}

void DefaultVisitor::postVisit(NExpressionUnary* node) throw(NodeError) {}

void DefaultVisitor::preVisit(NExpressionBinary* node) throw(NodeError) {}

void DefaultVisitor::visit(NExpressionBinary* node) throw(NodeError) {}

void DefaultVisitor::postVisit(NExpressionBinary* node) throw(NodeError) {}

void DefaultVisitor::betweenChildren(NExpressionBinary* node)
    throw(NodeError)
{}

IVisitor::VisitOrder DefaultVisitor::getChildVisitOrder(NExpressionBinary* node)
    throw(NodeError)
{
    return IVisitor::NORMAL;
}

void DefaultVisitor::preVisit(NVariable* node) throw(NodeError) {}

void DefaultVisitor::visit(NVariable* node) throw(NodeError) {}

void DefaultVisitor::postVisit(NVariable* node) throw(NodeError) {}

void DefaultVisitor::preVisit(NNumber* node) throw(NodeError) {}

void DefaultVisitor::visit(NNumber* node) throw(NodeError) {}

void DefaultVisitor::postVisit(NNumber* node) throw(NodeError) {}

NodeError::NodeError(void) {}

NodeError::NodeError(const std::string& message) : _message(message) {}

NodeError::~NodeError(void) throw() {}

const char* NodeError::what() const throw() {
    return _message.c_str();
}
