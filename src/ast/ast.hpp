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

#ifndef CEE_AST_AST__H
#define CEE_AST_AST__H

/**
 * @file
 * @brief Defines the classes needed for the AST.
 */

#include <exception>
#include <list>
#include <string>

/**
 * @brief Contains all classes and functions used for constructing an AST.
 *
 * The \c AST namespace contains all classes and functions that are needed for
 * creating an AST for a particular input source program.
 */
namespace AST {

class NStatementList;
class NStatement;
class NExpression;
class NVariable;
class IVisitor;

/**
 * Defines the operator types that can be used in an expression (this includes
 * both unary and binary expressions).
 */
enum ExpressionOperator {
    PLUS,
    MINUS,
    MUL,
    DIV
};

/**
 * \brief Node tree exception.
 *
 * Exception which may be thrown when the AST tree is traversed.
 */
class NodeError : public std::exception {
  public:    
    /**
     * Creates a node error with no error message.
     */
    NodeError(void);

    /**
     * Creates a node error with error message.
     *
     * @param message
     *        Error message.
     */
    NodeError(const std::string& message);

    /**
     * Destroys this exception.
     */
    virtual ~NodeError(void) throw();

    /**
     * Gets the error message.
     *
     * @returns Error message.
     */
    virtual const char* what() const throw();

  private:
    /**
     * Error message.
     */
    std::string _message;
};

/**
 * \brief Abstract base class for the AST nodes.
 *
 * The Node class provides the methods that will be needed for all nodes in the
 * AST. These include line and column access methods, but also declares the \c
 * accept(IVisitor*) method that must be implemented by all deriving subclasses.
 * Each implementation of accept(IVisitor*) must work as follows:
 *     -# Invoke IVisitor::preVisit()
 *     -# Invoke IVisitor::visit()
 *     -# Invoke accept() on all child nodes
 *         - Between each accept() to the children,
 *           invoke IVisitor::betweenChildren()
 *     -# Invoke IVisitor::postVisit()
 *
 * The order in which the child nodes are visited is determined by making a
 * call to the corresponding IVisitor::getChildVisitOrder() method. This is of
 * course only needed when the node contains more than one child.
 *
 * The IVisitor::preVisit() and IVisitor::postVisit() methods have its uses when
 * actions must be taken before or after a node is visited. For example, when
 * pretty-printing an AST, the IVisitor::preVisit() and IVisitor::postVisit()
 * methods can be used to control the level of indentation.
 */
class Node {
  public:
    /**
     * Creates a node.
     * 
     * @param line
     *        Line number at which this node was found.
     * @param column
     *        Column number at which this node was found.
     */
    Node(int line, int column);

    /**
     * Destroys this node and all of its child nodes.
     */
    virtual ~Node(void);

    /**
     * Get line number at which this node was declared.
     *
     * @returns Line number.
     */
    int getLine(void) const;

    /**
     * Get column number at which this node was declared.
     *
     * @returns Column number.
     */
    int getColumn(void) const;

    /**
     * Makes a visit to this node by the given visitor.
     *
     * @param visitor
     *        Visitor.
     * @throws NodeError
     *         When an error occurs.
     */
    virtual void accept(IVisitor* visitor) throw(NodeError) = 0;

  private:
    /**
     * Line number at which this node was declared.
     */
    int _line;

    /**
     * Column number at which this node was declared.
     */
    int _column;
};

/**
 * \brief Program node class.
 *
 * Class for the root node of the AST.
 */
class NProgram : public Node {
  public:
    /**
     * \copydoc Node::Node(int, int)
     *
     * @param statements
     *        Statement list node.
     */
    NProgram(
        NStatementList* statements,
        int line,
        int column);

    /**
     * \copydoc Node::~Node(void)
     */
    virtual ~NProgram(void);

    /**
     * Gets the statement list node.
     *
     * @returns Statement list node.
     */
    NStatementList* getStatementList(void) const;

    /**
     * \copydoc Node::accept(IVisitor*)
     */
    virtual void accept(IVisitor* visitor) throw(NodeError);

  private:
    /**
     * Statement list node.
     */
    NStatementList* _statements;
};

/**
 * \brief Statement list node class.
 *
 * Class for a list of statements.
 */
class NStatementList : public Node {
  public:
    /**
     * \copydoc Node::Node(int, int)
     */
    NStatementList(int line, int column);

    /**
     * \copydoc Node::~Node(void)
     */
    virtual ~NStatementList(void);

    /**
     * Appends a statement node as child of this node.
     * 
     * @param node
     *        Statement node.
     */
    void appendStatement(NStatement* node);

    /**
     * \copydoc appendStatement(NStatement*)
     *
     * @returns This node.
     *
     * Alias to appendStatement(NStatement*)
     */
    NStatementList& operator<<(NStatement* node);

    /**
     * Gets all statement nodes that are children of this node.
     *
     * @returns List of statement nodes.
     */
    std::list<NStatement*> getStatements(void);

    /**
     * \copydoc Node::accept(IVisitor*)
     *
     * In IVisitor::NORMAL visiting order, the child nodes are visited in the
     * order they appear in the list retrieved from getStatements(void).
     */
    virtual void accept(IVisitor* visitor) throw(NodeError);

  private:
    /**
     * List of statement nodes.
     */
    std::list<NStatement*> _statements;
};

/**
 * \brief Abstract statement node class.
 *
 * Abstract class for a statement.
 */
class NStatement : public Node {
  public:
    /**
     * \copydoc Node::Node(int, int)
     */
    NStatement(int line, int column);

    /**
     * \copydoc Node::~Node(void)
     */
    virtual ~NStatement(void);
};

/**
 * \brief Assignment node class.
 *
 * Class for an assignment.
 */
class NAssignment : public NStatement {
  public:
    /**
     * \copydoc Node::Node(int, int)
     *
     * @param variable
     *        Variable node
     * @param expr
     *        Expression node.
     */
    NAssignment(NVariable* variable, NExpression* expr, int line, int column);

    /**
     * \copydoc Node::~Node(void)
     */
    virtual ~NAssignment(void);

    /**
     * Gets the variable node.
     *
     * @returns Variable node.
     */
    NVariable* getVariable(void) const;

    /**
     * Gets the expression node.
     *
     * @returns Expression node.
     */
    NExpression* getExpression(void) const;

    /**
     * \copydoc Node::accept(IVisitor*)
     *
     * In IVisitor::NORMAL visiting order, the variable node is visited before
     * the expression node.
     */
    virtual void accept(IVisitor* visitor) throw(NodeError);

  private:
    /**
     * Variable node.
     */
    NVariable* _variable;

    /**
     * Expression node.
     */
    NExpression* _expr;
};

/**
 * \brief Print node class.
 *
 * Class for a print assignment.
 */
class NPrint : public NStatement {
  public:
    /**
     * \copydoc Node::Node(int, int)
     *
     * @param expr
     *        Expression node.
     */
    NPrint(NExpression* expr, int line, int column);

    /**
     * \copydoc Node::~Node(void)
     */
    virtual ~NPrint(void);

    /**
     * Gets the expression node.
     *
     * @returns Expression node.
     */
    NExpression* getExpression(void) const;

    /**
     * \copydoc Node::accept(IVisitor*)
     */
    virtual void accept(IVisitor* visitor) throw(NodeError);

  private:
    /**
     * Expression node.
     */
    NExpression* _expr;
};

/**
 * \brief Abstract expression node class.
 *
 * Abstract class for an expression.
 */
class NExpression : public Node {
  public:
    /**
     * \copydoc Node::Node(int, int)
     */
    NExpression(int line, int column);

    /**
     * \copydoc Node::~Node(void)
     */
    virtual ~NExpression(void);
};

/**
 * \brief Unary-operator expression node class.
 *
 * Class for the unary-operator expressions.
 */
class NExpressionUnary : public NExpression {
  public:
    /**
     * \copydoc Node::Node(int, int)
     *
     * @param op
     *        Unary operator.
     * @param expr
     *        Expression node.
     */
    NExpressionUnary(
        ExpressionOperator op,
        NExpression* expr,
        int line,
        int column);

    /**
     * \copydoc Node::~Node(void)
     */
    virtual ~NExpressionUnary(void);

    /**
     * Gets the expression node.
     *
     * @returns Expression node.
     */
    NExpression* getExpression(void) const;

    /**
     * Gets the operator.
     *
     * @returns ExpressionOperator.
     */
    ExpressionOperator getOperator(void) const;

    /**
     * \copydoc Node::accept(IVisitor*)
     */
    virtual void accept(IVisitor* visitor) throw(NodeError);

  private:
    /**
     * Expression operator.
     */
    ExpressionOperator _op;

    /**
     * Expression node.
     */
    NExpression* _expr;
};

/**
 * \brief Binary-operator expression node class.
 *
 * Class for the binary-operator expressions.
 */
class NExpressionBinary : public NExpression {
  public:
    /**
     * \copydoc Node::Node(int, int)
     *
     * @param lhs
     *        Left-hand side expression node.
     * @param op
     *        Binary operator.
     * @param rhs
     *        Right-hand side expression node.
     */
    NExpressionBinary(
        NExpression* lhs,
        ExpressionOperator op,
        NExpression* rhs,
        int line,
        int column);

    /**
     * \copydoc Node::~Node(void)
     */
    virtual ~NExpressionBinary(void);

    /**
     * Gets the left-hand side expression node.
     *
     * @returns Expression node.
     */
    NExpression* getLhsExpression(void) const;

    /**
     * Gets the operator.
     *
     * @returns ExpressionOperator.
     */
    ExpressionOperator getOperator(void) const;

    /**
     * Gets the right-hand side expression node.
     *
     * @returns Expression node.
     */
    NExpression* getRhsExpression(void) const;

    /**
     * \copydoc Node::accept(IVisitor*)
     *
     * In IVisitor::NORMAL visiting order, the left-hand side expression node is
     * visited before the right-hand side expression node.
     */
    virtual void accept(IVisitor* visitor) throw(NodeError);

  private:
    /**
     * Left-hand side expression node.
     */
    NExpression* _lhs;

    /**
     * Expression operator.
     */
    ExpressionOperator _op;

    /**
     * Right-hand side expression node.
     */
    NExpression* _rhs;
};

/**
 * \brief Number node class
 *
 * Class for a number.
 */
class NNumber : public NExpression {
  public:
    /**
     * \copydoc Node::Node(int, int)
     *
     * @param number
     *        Number value.
     */
    NNumber(const std::string& number, int line, int column);

    /**
     * \copydoc Node::~Node(void)
     */
    virtual ~NNumber(void);

    /**
     * Gets the number value.
     *
     * @returns Number value.
     */
    std::string getNumber(void) const;

    /**
     * \copydoc Node::accept(IVisitor*)
     */
    virtual void accept(IVisitor* visitor) throw(NodeError);

  private:
    /**
     * Number as a string.
     */
    std::string _number;
};

/**
 * \brief Variable node class.
 *
 * Class for a variable.
 */
class NVariable : public NExpression {
  public:
    /**
     * \copydoc Node::Node(int, int)
     *
     * @param name
     *        Variable name.
     */
    NVariable(const std::string& name, int line, int column);

    /**
     * \copydoc Node::~Node(void)
     */
    virtual ~NVariable(void);

    /**
     * Gets the variable name.
     *
     * @returns Variable name.
     */
    std::string getName(void) const;

    /**
     * \copydoc Node::accept(IVisitor*)
     */
    virtual void accept(IVisitor* visitor) throw(NodeError);

  private:
    /**
     * Variable name.
     */
    std::string _name;
};

/**
 * \brief AST visitor interface.
 *
 * Defines the interface of a visitor object (according to the <a
 * href="http://en.wikipedia.org/wiki/Visitor_pattern"><em>Visitor</em></a>
 * design pattern).
 * 
 * The core part of a visitor is the \c visit() function. It decides what to do
 * with the current node. \c visit() is always invoked on the current node prior
 * to visiting its children. Two additional methods - \c preVisit() and \c
 * postVisit() - are used to take actions before visiting the node, and after
 * having visited the node and all of its children. There is also another method
 * - betweenChildren() - which is invoked after one child has been visited but
 * before visiting the next child (this only applies if the node has more than
 * one child node).
 */
class IVisitor {
  public:
    /**
     * Defines the possible orders in which child nodes can be visited.
     */
    enum VisitOrder {
        /**
         * Child nodes are visited in normal order.
         */
        NORMAL,

        /**
         * Child nodes are visited in reversed order (compared to the normal
         * order).
         */
        REVERSED
    };

  public:
    /**
     * Destroys this visitor.
     */
    virtual ~IVisitor(void);

    /**
     * Hook that is invoked \e before the node is visited.
     *
     * @param node
     *        Node about to visit.
     * @throws NodeError
     *         When an error occurs.
     */
    virtual void preVisit(Node* node) throw(NodeError) = 0;

    /**
     * Hook that is invoked when the node is to be visited.
     *
     * @param node
     *        Node to visit.
     * @throws NodeError
     *         When an error occurs.
     */
    virtual void visit(Node* node) throw(NodeError) = 0;

    /**
     * Hook that is invoked \e after the node is visited.
     *
     * @param node
     *        Node which was visited.
     * @throws NodeError
     *         When an error occurs.
     */
    virtual void postVisit(Node* node) throw(NodeError) = 0;

    /**
     * \copydoc preVisit(Node*)
     */
    virtual void preVisit(NProgram* node) throw(NodeError) = 0;

    /**
     * \copydoc visit(Node*)
     */
    virtual void visit(NProgram* node) throw(NodeError) = 0;

    /**
     * \copydoc postVisit(Node*)
     */
    virtual void postVisit(NProgram* node) throw(NodeError) = 0;

    /**
     * \copydoc preVisit(Node*)
     */
    virtual void preVisit(NStatementList* node) throw(NodeError) = 0;

    /**
     * \copydoc visit(Node*)
     */
    virtual void visit(NStatementList* node) throw(NodeError) = 0;

    /**
     * \copydoc postVisit(Node*)
     */
    virtual void postVisit(NStatementList* node) throw(NodeError) = 0;

    /**
     * Hook that is invoked between the visiting of the child nodes.
     *
     * @param node
     *        Node that contains the child nodes.
     * @throws NodeError
     *         When an error occurs.
     */
    virtual void betweenChildren(NStatementList* node) throw(NodeError) = 0;

    /**
     * Determines in which order the child nodes are to be visited.
     *
     * @param node
     *        Node that contains the child nodes.
     * @throws NodeError
     *         When an error occurs.
     */
    virtual VisitOrder getChildVisitOrder(NStatementList* node)
        throw(NodeError) = 0;

    /**
     * \copydoc preVisit(Node*)
     */
    virtual void preVisit(NStatement* node) throw(NodeError) = 0;

    /**
     * \copydoc visit(Node*)
     */
    virtual void visit(NStatement* node) throw(NodeError) = 0;

    /**
     * \copydoc postVisit(Node*)
     */
    virtual void postVisit(NStatement* node) throw(NodeError) = 0;

    /**
     * \copydoc preVisit(Node*)
     */
    virtual void preVisit(NAssignment* node) throw(NodeError) = 0;

    /**
     * \copydoc visit(Node*)
     */
    virtual void visit(NAssignment* node) throw(NodeError) = 0;

    /**
     * \copydoc postVisit(Node*)
     */
    virtual void postVisit(NAssignment* node) throw(NodeError) = 0;

    /**
     * \copydoc betweenChildren(NStatementList*)
     */
    virtual void betweenChildren(NAssignment* node) throw(NodeError) = 0;

    /**
     * \copydoc getChildVisitOrder(NStatementList*)
     */
    virtual VisitOrder getChildVisitOrder(NAssignment* node)
        throw(NodeError) = 0;

    /**
     * \copydoc preVisit(Node*)
     */
    virtual void preVisit(NPrint* node) throw(NodeError) = 0;

    /**
     * \copydoc visit(Node*)
     */
    virtual void visit(NPrint* node) throw(NodeError) = 0;

    /**
     * \copydoc postVisit(Node*)
     */
    virtual void postVisit(NPrint* node) throw(NodeError) = 0;

    /**
     * \copydoc preVisit(Node*)
     */
    virtual void preVisit(NExpression* node) throw(NodeError) = 0;

    /**
     * \copydoc visit(Node*)
     */
    virtual void visit(NExpression* node) throw(NodeError) = 0;

    /**
     * \copydoc postVisit(Node*)
     */
    virtual void postVisit(NExpression* node) throw(NodeError) = 0;

    /**
     * \copydoc preVisit(Node*)
     */
    virtual void preVisit(NExpressionUnary* node) throw(NodeError) = 0;

    /**
     * \copydoc visit(Node*)
     */
    virtual void visit(NExpressionUnary* node) throw(NodeError) = 0;

    /**
     * \copydoc postVisit(Node*)
     */
    virtual void postVisit(NExpressionUnary* node) throw(NodeError) = 0;

    /**
     * \copydoc preVisit(Node*)
     */
    virtual void preVisit(NExpressionBinary* node) throw(NodeError) = 0;

    /**
     * \copydoc visit(Node*)
     */
    virtual void visit(NExpressionBinary* node) throw(NodeError) = 0;

    /**
     * \copydoc postVisit(Node*)
     */
    virtual void postVisit(NExpressionBinary* node) throw(NodeError) = 0;

    /**
     * \copydoc betweenChildren(NStatementList*)
     */
    virtual void betweenChildren(NExpressionBinary* node) throw(NodeError) = 0;

    /**
     * \copydoc getChildVisitOrder(NStatementList*)
     */
    virtual VisitOrder getChildVisitOrder(NExpressionBinary* node)
        throw(NodeError) = 0;

    /**
     * \copydoc preVisit(Node*)
     */
    virtual void preVisit(NVariable* node) throw(NodeError) = 0;

    /**
     * \copydoc visit(Node*)
     */
    virtual void visit(NVariable* node) throw(NodeError) = 0;

    /**
     * \copydoc postVisit(Node*)
     */
    virtual void postVisit(NVariable* node) throw(NodeError) = 0;

    /**
     * \copydoc preVisit(Node*)
     */
    virtual void preVisit(NNumber* node) throw(NodeError) = 0;

    /**
     * \copydoc visit(Node*)
     */
    virtual void visit(NNumber* node) throw(NodeError) = 0;

    /**
     * \copydoc postVisit(Node*)
     */
    virtual void postVisit(NNumber* node) throw(NodeError) = 0;
};

/**
 * \brief Default implementation of an IVisitor.
 *
 * Implements a default visitor, whose implementation of all methods do nothing.
 * This is convenient when a new visitor only needs to handle a small set of
 * node classes.
 *
 * See IVisitor for more information.
 */
class DefaultVisitor : public IVisitor {
  public:
    /**
     * \copydoc IVisitor::~IVisitor()
     */
    virtual ~DefaultVisitor(void);

    /**
     * \copydoc IVisitor::preVisit(Node*)
     *
     * Does nothing by default.
     */
    virtual void preVisit(Node* node) throw(NodeError);

    /**
     * \copydoc IVisitor::visit(Node*)
     *
     * Does nothing by default.
     */
    virtual void visit(Node* node) throw(NodeError);

    /**
     * \copydoc IVisitor::postVisit(Node*)
     *
     * Does nothing by default.
     */
    virtual void postVisit(Node* node) throw(NodeError);

    /**
     * \copydoc IVisitor::preVisit(NProgram*)
     *
     * Does nothing by default.
     */
    virtual void preVisit(NProgram* node) throw(NodeError);

    /**
     * \copydoc IVisitor::visit(NProgram*)
     *
     * Does nothing by default.
     */
    virtual void visit(NProgram* node) throw(NodeError);

    /**
     * \copydoc IVisitor::postVisit(NProgram*)
     *
     * Does nothing by default.
     */
    virtual void postVisit(NProgram* node) throw(NodeError);

    /**
     * \copydoc IVisitor::preVisit(NStatementList*)
     *
     * Does nothing by default.
     */
    virtual void preVisit(NStatementList* node) throw(NodeError);

    /**
     * \copydoc IVisitor::visit(NStatementList*)
     *
     * Does nothing by default.
     */
    virtual void visit(NStatementList* node) throw(NodeError);

    /**
     * \copydoc IVisitor::postVisit(NStatementList*)
     *
     * Does nothing by default.
     */
    virtual void postVisit(NStatementList* node) throw(NodeError);

    /**
     * \copydoc IVisitor::betweenChildren(NStatementList*)
     *
     * Does nothing by default.
     */
    virtual void betweenChildren(NStatementList* node) throw(NodeError);

    /**
     * \copydoc IVisitor::getChildVisitOrder(NStatementList*)
     *
     * @returns IVisitor::NORMAL by default.
     */
    virtual IVisitor::VisitOrder getChildVisitOrder(NStatementList* node)
        throw(NodeError);

    /**
     * \copydoc IVisitor::preVisit(NStatement*)
     *
     * Does nothing by default.
     */
    virtual void preVisit(NStatement* node) throw(NodeError);

    /**
     * \copydoc IVisitor::visit(NStatement*)
     *
     * Does nothing by default.
     */
    virtual void visit(NStatement* node) throw(NodeError);

    /**
     * \copydoc IVisitor::postVisit(NStatement*)
     *
     * Does nothing by default.
     */
    virtual void postVisit(NStatement* node) throw(NodeError);

    /**
     * \copydoc IVisitor::preVisit(NAssignment*)
     *
     * Does nothing by default.
     */
    virtual void preVisit(NAssignment* node) throw(NodeError);

    /**
     * \copydoc IVisitor::visit(NAssignment*)
     *
     * Does nothing by default.
     */
    virtual void visit(NAssignment* node) throw(NodeError);

    /**
     * \copydoc IVisitor::postVisit(NAssignment*)
     *
     * Does nothing by default.
     */
    virtual void postVisit(NAssignment* node) throw(NodeError);

    /**
     * \copydoc IVisitor::betweenChildren(NAssignment*)
     *
     * Does nothing by default.
     */
    virtual void betweenChildren(NAssignment* node) throw(NodeError);

    /**
     * \copydoc IVisitor::getChildVisitOrder(NAssignment*)
     *
     * @returns IVisitor::NORMAL by default.
     */
    virtual IVisitor::VisitOrder getChildVisitOrder(NAssignment* node)
        throw(NodeError);

    /**
     * \copydoc IVisitor::preVisit(NPrint*)
     *
     * Does nothing by default.
     */
    virtual void preVisit(NPrint* node) throw(NodeError);

    /**
     * \copydoc IVisitor::visit(NPrint*)
     *
     * Does nothing by default.
     */
    virtual void visit(NPrint* node) throw(NodeError);

    /**
     * \copydoc IVisitor::postVisit(NPrint*)
     *
     * Does nothing by default.
     */
    virtual void postVisit(NPrint* node) throw(NodeError);

    /**
     * \copydoc IVisitor::preVisit(NExpression*)
     *
     * Does nothing by default.
     */
    virtual void preVisit(NExpression* node) throw(NodeError);

    /**
     * \copydoc IVisitor::visit(NExpression*)
     *
     * Does nothing by default.
     */
    virtual void visit(NExpression* node) throw(NodeError);

    /**
     * \copydoc IVisitor::postVisit(NExpression*)
     *
     * Does nothing by default.
     */
    virtual void postVisit(NExpression* node) throw(NodeError);

    /**
     * \copydoc IVisitor::preVisit(NExpressionUnary*)
     *
     * Does nothing by default.
     */
    virtual void preVisit(NExpressionUnary* node) throw(NodeError);

    /**
     * \copydoc IVisitor::visit(NExpressionUnary*)
     *
     * Does nothing by default.
     */
    virtual void visit(NExpressionUnary* node) throw(NodeError);

    /**
     * \copydoc IVisitor::postVisit(NExpressionUnary*)
     *
     * Does nothing by default.
     */
    virtual void postVisit(NExpressionUnary* node) throw(NodeError);

    /**
     * \copydoc IVisitor::preVisit(NExpressionBinary*)
     *
     * Does nothing by default.
     */
    virtual void preVisit(NExpressionBinary* node) throw(NodeError);

    /**
     * \copydoc IVisitor::visit(NExpressionBinary*)
     *
     * Does nothing by default.
     */
    virtual void visit(NExpressionBinary* node) throw(NodeError);

    /**
     * \copydoc IVisitor::postVisit(NExpressionBinary*)
     *
     * Does nothing by default.
     */
    virtual void postVisit(NExpressionBinary* node) throw(NodeError);

    /**
     * \copydoc IVisitor::betweenChildren(NExpressionBinary*)
     *
     * Does nothing by default.
     */
    virtual void betweenChildren(NExpressionBinary* node) throw(NodeError);

    /**
     * \copydoc IVisitor::getChildVisitOrder(NExpressionBinary*)
     *
     * @returns IVisitor::NORMAL by default.
     */
    virtual IVisitor::VisitOrder getChildVisitOrder(NExpressionBinary* node)
        throw(NodeError);

    /**
     * \copydoc IVisitor::preVisit(NVariable*)
     *
     * Does nothing by default.
     */
    virtual void preVisit(NVariable* node) throw(NodeError);

    /**
     * \copydoc IVisitor::visit(NVariable*)
     *
     * Does nothing by default.
     */
    virtual void visit(NVariable* node) throw(NodeError);

    /**
     * \copydoc IVisitor::postVisit(NVariable*)
     *
     * Does nothing by default.
     */
    virtual void postVisit(NVariable* node) throw(NodeError);

    /**
     * \copydoc IVisitor::preVisit(NNumber*)
     *
     * Does nothing by default.
     */
    virtual void preVisit(NNumber* node) throw(NodeError);

    /**
     * \copydoc IVisitor::visit(NNumber*)
     *
     * Does nothing by default.
     */
    virtual void visit(NNumber* node) throw(NodeError);

    /**
     * \copydoc IVisitor::postVisit(NNumber*)
     *
     * Does nothing by default.
     */
    virtual void postVisit(NNumber* node) throw(NodeError);
};

}

#endif
