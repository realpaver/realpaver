/*------------------------------------------------------------------------------
 * Realpaver -- Realpaver is a rigorous nonlinear constraint solver based on
 *              interval computations.
 *------------------------------------------------------------------------------
 * Copyright (c) 2004-2016 Laboratoire d'Informatique de Nantes Atlantique,
 *               France
 * Copyright (c) 2017-2024 Laboratoire des Sciences du Numérique de Nantes,
 *               France
 *------------------------------------------------------------------------------
 * Realpaver is a software distributed WITHOUT ANY WARRANTY. Read the COPYING
 * file for information.
 *----------------------------------------------------------------------------*/

/**
 * @file   Dag.hpp
 * @brief  DAG of constraints
 * @author Laurent Granvilliers
 * @date   2024-4-11
*/

#ifndef REALPAVER_DAG_HPP
#define REALPAVER_DAG_HPP

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "realpaver/Bitset.hpp"
#include "realpaver/Constraint.hpp"
#include "realpaver/IntervalMatrix.hpp"
#include "realpaver/RealMatrix.hpp"
#include "realpaver/TermLin.hpp"

namespace realpaver {

class Dag;
class DagVisitor;
class FlatFunction;

/// Type of lists of indexes
using IndexList = std::vector<size_t>;

/// Enumeration of dag node symbols
enum class DagSymbol {
   Cst,     ///< constant
   Var,     ///< variable
   Add,     ///< addition
   Sub,     ///< subtraction
   Mul,     ///< multiplication
   Div,     ///< division
   Min,     ///< minimum
   Max,     ///< maximum
   Usb,     ///< unary subtraction
   Abs,     ///< absolute value
   Sgn,     ///< sign
   Sqr,     ///< square
   Sqrt,    ///< square root
   Pow,     ///< power
   Exp,     ///< exponential
   Log,     ///< logarithm
   Cos,     ///< cosine
   Sin,     ///< sine
   Tan,     ///< tangent
   Cosh,    ///< hyperbolic cosine
   Sinh,    ///< hyperbolic sine
   Tanh,    ///< hyperbolic tangent
   Lin      ///< linear expression
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, DagSymbol op);

/**
 * @brief Base class of DAG nodes.
 *
 * A node has an index in the DAG, a list a sub-nodes and a list of parent
 * nodes. It encloses real and interval numbers resulting from evaluation and
 * differentiation operations.
*/
class DagNode {
public:
   /// Constructor given the symbol and node index
   DagNode(Dag* dag, DagSymbol symb, size_t index = 0);

   /// Virtual destructor
   virtual ~DagNode();

   /// Returns the DAG index of this
   size_t index() const;

   /// Assigns the DAG index
   void setIndex(size_t i);

   /// Returns the number of parent nodes
   size_t parArity() const;

   /// Returns the i-th parent node of this with 0 <= i < parArity()
   size_t parNode(size_t i) const;

   /// Inserts a parent node of index i
   void addParNode(size_t i);
   
   /// Returns the number of sub-nodes
   size_t subArity() const;

   /// Returns the i-th sub-node of this with 0 <= i < subArity()
   size_t subIndex(size_t i) const;

   /// Returns the i-th sub-node of this with 0 <= i < subArity()
   DagNode* subNode(size_t i) const;

   /// Inserts a sub-node of index i
   void addSubNode(size_t i);

   /// Returns the left child of a binary operation
   DagNode* left() const;

   /// Returns the right child of a binary operation
   DagNode* right() const;

   /// Returns the child of a unary operation
   DagNode* child() const;

   /// Returns the DAG
   Dag* dag() const;

   /// Returns the symbol
   DagSymbol symbol() const;

   /// Returns the bitset this depends on
   const Bitset& bitset() const;

   /// Returns true if this depends on bs
   bool dependsOn(const Bitset& bs) const;

   /// Returns true if this has more than one parent node
   bool isShared() const;

   /// Returns true if v belongs to the tree rooted by this
   bool dependsOn(const Variable& v) const;

   /// Returns true if this depends onthe interval value
   Interval val() const;

   /// Assigns the interval value
   void setVal(const Interval& x);

   /// Returns the interval derivative
   Interval dv() const;

   /// Assigns the interval derivative
   void setDv(const Interval& x);

   /// Adds an interval derivative
   void addDv(const Interval& x);

   /// Returnsthe real value
   double rval() const;

   /// Assigns the real value
   void setRval(double x);

   /// Returns the real derivative
   double rdv() const;

   /// Assigns the real derivative
   void setRdv(double x);

   /// Adds a real derivative
   void addRdv(double x);

   /// Counts the number of occurrences of a variable in the sub-tree
   virtual size_t nbOccurrences(const Variable& v) const;

   /// Visitor pattern
   virtual void acceptVisitor(DagVisitor& vis) const = 0;

   /// Output on a stream
   virtual void print(std::ostream& os) const = 0;

   /// Interval evaluation of this on B
   virtual void eval(const IntervalBox& B) = 0;

   /// Interval evaluation of this on pt
   virtual void eval(const RealPoint& pt) = 0;

   /**
    * @brief Restricted interval evaluation.
    *
    * This is evaluated only if it depends on v and the result is assigned
    * in the interval value enclosed in this.
    */
   virtual void evalOnly(const Variable& v, const Interval& x) = 0;

   /**
    * @brief Interval differentiation in reverse mode.
    * 
    * Returns false if some operation at this node is not continuous.
    * It assumes that an interval evaluation has been done.
    */
   virtual bool diff() = 0;

   /**
    * @brief Restricted interval differentiation in reverse mode.
    *
    * This is differentiated only if it depends on v.
    * It assumes that an interval evaluation has been done.
    */
   virtual bool diffOnly(const Variable& v) = 0;

   /**
    * @brief Real (point) evaluation on pt.
    *
    * The result is assigned in the real value enclosed.
    */
   virtual void reval(const RealPoint& pt) = 0;

   /**
    * @brief Real (point) differentiation in reverse mode.
    * 
    * Returns false if some operation at this node is not continuous.
    * It assumes that a point evaluation has been done.
    */
   virtual bool rdiff() = 0;

protected:
   Bitset bitset_;   // set of variables occurring in this

private:
   Dag* dag_;        // the DAG
   DagSymbol symb_; // symbol
   size_t index_;    // index of this node in the DAG
   IndexList vpar_;  // list of indexes of parent nodes in the DAG
   IndexList vsub_;  // list of indexes of sub-nodes in the DAG
   Interval val_;    // result of an interval evaluation
   Interval dv_;     // interval derivative
   double rval_;     // result of a point evaluation
   double rdv_;      // point derivative
};

/*----------------------------------------------------------------------------*/

/// DAG node representing an interval constant.
class DagConst : public DagNode {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param index node index in the DAG
    * @param x value enclosed
    */
   DagConst(Dag* dag, size_t index, const Interval& x);

   void print(std::ostream& os) const override;
   void acceptVisitor(DagVisitor& vis) const override;
   void eval(const IntervalBox& B) override;
   void eval(const RealPoint& pt) override;
   void evalOnly(const Variable& v, const Interval& x) override;
   bool diff() override;
   bool diffOnly(const Variable& v) override;
   void reval(const RealPoint& pt) override;
   bool rdiff() override;

   /// Returns the constant interval value
   Interval getConst() const;

private:
   Interval x_;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing a variable
class DagVar : public DagNode {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param index node index in the DAG
    * @param v variable enclosed
    */
   DagVar(Dag* dag, size_t index, Variable v);

   /// Returns the variable enclosed
   Variable getVar() const;

   size_t nbOccurrences(const Variable& v) const override;
   void print(std::ostream& os) const override;
   void acceptVisitor(DagVisitor& vis) const override;
   void eval(const IntervalBox& B) override;
   void eval(const RealPoint& pt) override;
   void evalOnly(const Variable& v, const Interval& x) override;
   bool diff() override;
   bool diffOnly(const Variable& v) override;
   void reval(const RealPoint& pt) override;
   bool rdiff() override;

private:
   Variable v_;
};

/*----------------------------------------------------------------------------*/

/// Base class of operation DAG nodes
class DagOp : public DagNode {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param symb operation symbol
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagOp(Dag* dag, DagSymbol symb, const IndexList& lsub);

   /// Virtual destructor
   virtual ~DagOp();

   /**
    * @brief Assigns the DAG index of this.
    *
    * It makes the links between this node and its sub-nodes.
    */
   void setIndex(size_t index);

   /// Equality test
   bool eq(const DagOp* other) const;

   /// Equality test of relation symbols
   virtual bool eqSymbol(const DagOp* other) const;

   /// Interval evaluation using the interval values of the children nodes
   virtual void eval() = 0;

   /// Real evaluation using the real values of the children nodes
   virtual void reval() = 0;

   virtual size_t nbOccurrences(const Variable& v) const override;
   virtual void print(std::ostream& os) const override;
   void eval(const IntervalBox& B) override;
   void eval(const RealPoint& pt) override;
   void evalOnly(const Variable& v, const Interval& x) override;
   bool diffOnly(const Variable& v) override;
   void reval(const RealPoint& pt) override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing a binary addition
class DagAdd : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagAdd(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing a binary subtraction
class DagSub : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagSub(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing a binary multiplication
class DagMul : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagMul(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing a division
class DagDiv : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagDiv(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing a binary minimum
class DagMin : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagMin(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing a binary maximum
class DagMax : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagMax(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing a unary subtraction
class DagUsb : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagUsb(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing an absolute value
class DagAbs : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagAbs(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing a sign function
class DagSgn : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagSgn(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing a square
class DagSqr : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagSqr(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing a square root
class DagSqrt : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagSqrt(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing the power operation
class DagPow : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param n exponent
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagPow(Dag* dag, int n, const IndexList& lsub);

   /// Returns the exponent
   int exponent() const;

   bool eqSymbol(const DagOp* other) const override;
   void print(std::ostream& os) const override;
   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;

private:
   int n_;  // exponent
};

/*----------------------------------------------------------------------------*/

/// DAG node representing the exponential function
class DagExp : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagExp(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing the logarithm function
class DagLog : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagLog(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing the cosine function
class DagCos : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagCos(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing the sine function
class DagSin : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagSin(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing the tangent function
class DagTan : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagTan(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing the hyperbolic cosine function
class DagCosh : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagCosh(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing the hyperbolic sine function
class DagSinh : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagSinh(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing the hyperbolic tangent function
class DagTanh : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param lsub list of DAG indexes of the sub-nodes of this
    */
   DagTanh(Dag* dag, const IndexList& lsub);

   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;
};

/*----------------------------------------------------------------------------*/

/// DAG node representing a (non-constant) linear expression
class DagLin : public DagOp {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param t linear term
    * @param lsub list of DAG indexes of the sub-nodes (variables) of this
    */
   DagLin(Dag* dag, const TermLin& tl, const IndexList& lsub);

   bool eqSymbol(const DagOp* other) const override;
   size_t nbOccurrences(const Variable& v) const override;
   void print(std::ostream& os) const override;
   void acceptVisitor(DagVisitor& vis) const override;
   void eval() override;
   bool diff() override;
   void reval() override;
   bool rdiff() override;

   /// Returns the constant value of this linear expression
   Interval getCst() const;

   /// Returns the number of linear terms
   size_t nbTerms() const;

   /// Returns the variable node of the i-th linear term
   DagVar* varNode(size_t i) const;

   /// Returns the factor of the i-th linear term
   Interval coef(size_t i) const;

private:
   struct Item {
      Interval coef;    // coefficient
      DagVar* node;     // variable node
      Interval ival;    // used for the evaluation
   };

   struct CompItem {
      bool operator()(const Item& i1, const Item& i2) const
      {
         return i1.node->getVar().id() < i2.node->getVar().id();
      }
   };

   Interval cst_;                      // constant value
   std::set<Item, CompItem> terms_;    // set of linear terms

public:
   /// Type of iterators on the list of linear terms
   typedef std::set<Item, CompItem>::const_iterator const_iterator;

   /// Returns an iterator on the beginning of the list of linear terms
   const_iterator begin() const;

   /// Returns an iterator on the end of the list of linear terms
   const_iterator end() const;

   /// Returns the coefficient of a linear term pointed by an iterator
   Interval getCoefSub(const_iterator it) const;

   /// Returns the variable of a linear term pointed by an iterator
   DagVar* getNodeSub(const_iterator it) const;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief Function in a DAG.
 *
 * To every function is associated an image, i.e. we have L <= f(x) <= U.
*/
class DagFun {
public:
   /**
    * @brief Constructor.
    * @param dag owner of this
    * @param root DAG index of the root node
    * @param scop set of variables
    * @param image image of this
    */
   DagFun(Dag* dag, size_t root, Scope scop, const Interval& image);

   /// Destructor
   ~DagFun();

   /// Returns the image of this
   Interval getImage() const;

   /// Assigns the image of this
   void setImage(const Interval& x);

   /// Returns the dag
   Dag* dag() const;

   /// Inserts a constant node
   void insertConstNode(DagConst* node);

   /// Inserts a variable node
   void insertVarNode(DagVar* node);

   /// Inserts an operation node
   void insertOpNode(DagOp* node);

   /// Returns the number of nodes of this
   size_t nbNodes() const;

   /// Gets the i-th node of this with 0 <= i < nbNodes()
   DagNode* node(size_t i) const;

   /// Returns the root node of this
   DagNode* rootNode() const;

   /// Returns the number of variables occurring in this
   size_t nbVars() const;

   /// Gets the i-th variable of this with 0 <= i < nbVars()
   DagVar* varNode(size_t i) const;

   /// Returns true if v occurs in this
   bool dependsOn(const Variable& v) const;

   /// Counts the number of occurrences of v in this
   size_t nbOccurrences(const Variable& v) const;

   /// Assigns the index of this in the dag
   void setIndex(size_t i);

   /// Returns the index of this in the dag
   size_t index() const;

   /// Returns the scope of this (set of variables)
   Scope scope() const;

   /// Returns the interval evaluation of this on B
   Interval intervalEval(const IntervalBox& B);

  ///Returns the interval evaluation of this at pt
   Interval intervalEval(const RealPoint& pt);

   /// Returns the result of an interval evaluation
   Interval intervalValue() const;

   /**
    * @brief Restricted interval evaluation.
    * 
    * Returns the interval evaluation of this such that only the nodes depending
    * on v with domain x are evaluated.
    */
   Interval intervalEvalOnly(const Variable& v, const Interval& x);

   /**
    * @brief HC4Revise contractor.
    * 
    * Applies the contractor on B and returns a certificate of proof.
    * The projections of this onto its variables are assigned in B.
    */
   Proof hc4Revise(IntervalBox& B);

   /**
    * @brief HC4Revise contractor on the constraint negation.
    * 
    * Applies the contractor on B and returns a certificate of proof.
    * The projections of this onto its variables are assigned in B.
    */
   Proof hc4ReviseNeg(IntervalBox& B);

   /**
    * @brief Calculates the violation of the underlying constraint.
    * 
    * Returns
    * - 0.0 if the constraint is satisfied,
    * - a positive real number therwise equal to the width of the gap between
    *   the image of the function and the result of its evaluation.
    *
    * It assumes that this function has been evaluated using intervalEval().
    */
   double intervalViolation();

   /**
    * @brief Calculates the violation of the underlying constraint.
    * 
    * Returns
    * - 0.0 if the constraint is satisfied,
    * - a positive real number therwise equal to the width of the gap between
    *   the image of the function and the result of its evaluation.
    *
    *
    * It assumes that this function has been evaluated using realEval().
    */
   double realViolation();

   /**
    * @brief Interval differentiation in reverse mode.
    *
    * It assumes that this function has been evaluated.
    */
   void intervalDiff();

   /**
    * @brief Interval differentiation in reverse mode.
    * 
    * The derivatives are assigned to the gradient vector grad.
    *
    * It assumes that this function has been evaluated.
    */
   void intervalDiff(IntervalVector& grad);

   /**
    * @brief Restricted interval differentiation in forward mode.
    *
    * Returns false if some operation at this node is not continuous.
    * It assumes that this function has been evaluated.
    * Only the nodes depending on the considered variable are differentiated.
    */
   bool intervalDiffOnly(const Variable& v);

   /**
    * @brief Restricted interval differentiation in forward mode.
    *
    * Returns false if some operation at this node is not continuous.
    * It assumes that this function has been evaluated.
    * Only the nodes depending on the considered variable are differentiated.
    */
   bool intervalDiffOnly(const Variable& v, const Interval& x);

   /// Gets the i-th interval derivative with 0 <= i < nbVars()
   Interval intervalDeriv(size_t i) const;

   /// Gets the derivative with respect to v
   Interval intervalDeriv(const Variable& v) const;

   /// Returns the real (point) evaluation of this at pt
   double realEval(const RealPoint& pt);

   /// Returns the result of the point evaluation
   double realValue() const;

   /**
    * @brief Point differentiation in reverse mode.
    *
    * It assumes that this function has been evaluated.
    */
   void realDiff();

   /**
    * @brief Point differentiation in reverse mode.
    * 
    * The derivatives are assigned to gradient vector grad.
    * It assumes that this function has been evaluated.
    */
   void realDiff(RealVector& grad);

   /// Gets the i-th real derivative with 0 <= i < nbVars()
   double realDeriv(size_t i) const;

   /// Gets the real derivative with respect to v
   double realDeriv(const Variable& v) const;

   /// Returns the interval evaluation of this on B
   Interval iEval(const IntervalBox& B);

   /**
    * @brief Interval differentiation method.
    * 
    * Calculates the gradient of this on B (reverse mode). G[i] is the partial
    * derivative of this with respect to the i-th variable of its scope.
    */
   void iDiff(const IntervalBox& B, IntervalVector& G);

   /**
    * @brief Interval differentiation method using Hansen's strategy.
    * 
    * Calculates the gradient of this on B (reverse mode). G[i] is the partial
    * derivative of this with respect to the i-th variable of its scope.
    */
   void iDiffHansen(const IntervalBox& B, IntervalVector& G);

private:
   Dag* dag_;                    // the DAG
   std::vector<DagNode*> node_;  // vector of nodes of the function sorted by
                                 // a topological ordering from the leaves to
                                 // the root (back element of the vector)
   std::vector<DagVar*> vnode_;  // vector of variable nodes of the function
                                 // sorted by an ascending ordering of the
                                 // identifiers of the variables
   Scope scop_;                  // the set of variables
   Interval image_;              // bounds of the function, i.e. the co-domain
   size_t idx_;                  // index in the dag
   FlatFunction* flat_;          // implements hc4Revise

   // tests if a node is already present in this function
   bool hasNode(DagNode* node) const;

   // set of node indexes used to implement hasNode in O(1)
   std::unordered_set<size_t> inode_;
};

/*----------------------------------------------------------------------------*/

/**
 * @brief DAG representing an expression graph.
 *
 * An expression graph represents a constraint system L <= f(x) <= U with
 * a function vector f : Rn -> Rm and bounds U and L in Rm.
 */
class Dag {
public:
   /// Creates an empty DAG
   Dag();

   /// Destructor
   ~Dag();

   /// No copy
   Dag(const Dag&) =delete;

   /// No assignment
   Dag& operator=(const Dag&) =delete;

   /// Returns true if this dag is empty
   bool isEmpty() const;

   /// Returns the number of nodes
   size_t nbNodes() const;

   /// Gets the i-th node with 0 <= i < nbNodes()
   DagNode* node(size_t i) const;

   /// Returns the number of functions
   size_t nbFuns() const;

   /// Gets the i-th function with 0 <= i < nbFuns()
   DagFun* fun(size_t i) const;

   /// Returns the number of variables
   size_t nbVars() const;

   /// Gets the i-th variable with 0 <= i < nbVars()
   DagVar* varNode(size_t i) const;

   /**
    * @brief Gets a variable node given the identifier of a variable.
    * 
    * Returns the node representing the variable id if it exists,
    * nullptr otherwise.
    */
   DagVar* findVarNode(size_t id) const;

   /**
    * @brief Inserts a constraint in this and returns its index.
    *
    * Throws an exception if the constraint cannot be handled.
    */
   size_t insert(Constraint c);

   /**
    * @brief Inserts a constraint in this and returns its index.
    * 
    * The constraint is given by a term and its image.
    * Throws an exception if the constraint cannot be handled.
    */
   size_t insert(Term t, const Interval& img = Interval::universe());

   /**
    * @brief Inserts a node representing an interval.
    * 
    * Returns the node index. A new node is created only if it does not already
    * exists in the DAG.
    */
   size_t insertConstNode(const Interval& x);

   /**
    * @brief Inserts a node representing a variable.
    * 
    * Returns the node index. A new node is created only if it does not already
    * exists in the DAG.
    */
   size_t insertVarNode(const Variable& v);

   /**
    * @brief Inserts a node representing an operation.
    * 
    * Returns the node index. A new node is created only if it does not already
    * exists in the DAG, otherwise the input node is deleted.
    */
   size_t insertOpNode(size_t hcode, DagOp* node);

   /// Inserts a function and returns its index
   size_t insertFun(DagFun* f);

   /// Returns the scope of this
   Scope scope() const;

   /// Output on a stream
   void print(std::ostream& os) const;

   /**
    * @brief Interval evaluation on B.
    *
    * Returns false if an empty interval occurs in the computation
    */
   bool intervalEval(const IntervalBox& B);

   /**
    * @brief Interval evaluation on B.
    * 
    * Returns false if the output vector is empty, othertwise val[i] is
    * the value of the i-th function.
    */
   bool intervalEval(const IntervalBox& B, IntervalVector& val);

   /**
    * @brief Interval evaluation at pt.
    * 
    * Returns false if the output vector is empty, othertwise val[i] is
    * the value of the i-th function.
    */
   bool intervalPointEval(const RealPoint& pt, IntervalVector& val);

   /**
    * @brief Interval differentiation in reverse mode.
    * 
    * To be called after an evaluation.
    * The derivatives are assigned to the Jacobian matrix jac.
    */
   void intervalDiff(IntervalMatrix& jac);

   /**
    * @brief Interval differentiation for square systems using Hansen's method.
    * 
    * The derivatives are assigned to the Jacobian matrix jac.
    */
   void hansenDiff(const IntervalBox& B, IntervalMatrix& jac);

   /**
    * @brief Calculates the violation of the constraints.
    * 
    * The output vector is such that viol[i] is the violation of the i-th
    * constraint. It asumes that this dag has been evaluated using
    * intervalEval() or intervalPointEval().
    */
   void intervalViolation(RealVector& viol);

   /**
    * @brief Real (point) evaluation at pt.
    * 
    * Return false if a NaN occurs in the computation, true othewise.
    */
   bool realEval(const RealPoint& pt);

   /**
    * @brief Real (point) evaluation at pt.
    * 
    * The output vector is such that viol[i] is the violation of the i-th
    * function. Returns false if a NaN occurs, true othewise.
    */
   bool realEval(const RealPoint& pt, RealVector& val);

   /**
    * @brief Point differentiation in reverse mode.
    * 
    * To be called after an evaluation
    * The derivatives are assigned to the Jacobian matrix jac.
    */
   void realDiff(RealMatrix& jac);

   /**
    * @brief Calculates the violation of the constraints.
    * 
    * The output vector is such that viol[i] is the violation of the i-th
    * constraint. It assumes that this dag has been evaluated using realEval().
    */
   void realViolation(RealVector& viol);

   /// Prints the interval values at all nodes on a stream
   void printIntervalValues(std::ostream& os) const;



   void iEval(const IntervalBox& B, IntervalVector& V);

   void iDiff(const IntervalBox& B, IntervalMatrix& J);

   void iDiffHansen(const IntervalBox& B, IntervalMatrix& H);


private:
   // vector of nodes sorted by a topological ordering from the leaves
   // to the roots
   std::vector<DagNode*> node_;

   // vector of constant nodes
   std::vector<DagConst*> cnode_;

   // vector of variable nodes sorted by an ascending ordering
   // of the variable identifiers
   std::vector<DagVar*> vnode_;

   // vector of operation nodes sorted by a topological ordering from the leaves
   // to the roots
   std::vector<DagOp*> onode_;

   // vector of functions
   std::vector<DagFun*> fun_;

   // map (variable id -> node index)
   std::unordered_map<size_t, size_t> vmap_;

   // map (hash code of an operation node -> list of node indexes)
   std::unordered_map<size_t, IndexList> omap_;

   // scope
   Scope scop_;

   // find methods
   DagConst* findConstNode(const Interval& x) const;
   DagOp* findOpNode(size_t hcode, DagOp* node) const;

   // pushes a new node in the vector of nodes
   void pushNode(DagNode* node);
};

/// Type of shared pointers to dags
using SharedDag = std::shared_ptr<Dag>;

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const Dag& dag);

/*----------------------------------------------------------------------------*/

/// Base class of visitors of DAG nodes
class DagVisitor {
public:
   /// Virtual destructor
   virtual ~DagVisitor();

   /// @name Visit methods
   ///@{
   virtual void apply(const DagConst* d);
   virtual void apply(const DagVar* d);
   virtual void apply(const DagAdd* d);
   virtual void apply(const DagSub* d);
   virtual void apply(const DagMul* d);
   virtual void apply(const DagDiv* d);
   virtual void apply(const DagMin* d);
   virtual void apply(const DagMax* d);
   virtual void apply(const DagUsb* d);
   virtual void apply(const DagAbs* d);
   virtual void apply(const DagSgn* d);
   virtual void apply(const DagSqr* d);
   virtual void apply(const DagSqrt* d);
   virtual void apply(const DagPow* d);
   virtual void apply(const DagExp* d);
   virtual void apply(const DagLog* d);
   virtual void apply(const DagCos* d);
   virtual void apply(const DagSin* d);
   virtual void apply(const DagTan* d);
   virtual void apply(const DagCosh* d);
   virtual void apply(const DagSinh* d);
   virtual void apply(const DagTanh* d);
   virtual void apply(const DagLin* d);
   ///@}
};

/*----------------------------------------------------------------------------*/

/// Visitor used to create a function in a DAG
class DagFunCreator : public DagVisitor {
public:
   /// Constructor given f the function built by the visit
   DagFunCreator(DagFun* f);

   /// @name Visit methods
   ///@{
   void apply(const DagConst* d) override;
   void apply(const DagVar* d) override;
   void apply(const DagAdd* d) override;
   void apply(const DagSub* d) override;
   void apply(const DagMul* d) override;
   void apply(const DagDiv* d) override;
   void apply(const DagMin* d) override;
   void apply(const DagMax* d) override;
   void apply(const DagUsb* d) override;
   void apply(const DagAbs* d) override;
   void apply(const DagSgn* d) override;
   void apply(const DagSqr* d) override;
   void apply(const DagSqrt* d) override;
   void apply(const DagPow* d) override;
   void apply(const DagExp* d) override;
   void apply(const DagLog* d) override;
   void apply(const DagCos* d) override;
   void apply(const DagSin* d) override;
   void apply(const DagTan* d) override;
   void apply(const DagCosh* d) override;
   void apply(const DagSinh* d) override;
   void apply(const DagTanh* d) override;
   void apply(const DagLin* d) override;
   ///@}

private:
   DagFun* f_;

   void visitSubNodes(const DagOp* d);
};

/*----------------------------------------------------------------------------*/

/// Visitor used to insert an arithmetic constraint in a DAG
class DagCreator : public ConstraintVisitor {
public:
   /**
    * @brief Constructor.
    * 
    * @param dag DAG modified by a visit
    * @param c constraint visited
    */
   DagCreator(Dag* dag, Constraint c);

   /// Returns the constraint index in the DAG
   size_t index();

   void apply(const ArithCtrEq* c) override;
   void apply(const ArithCtrLe* c) override;
   void apply(const ArithCtrLt* c) override;
   void apply(const ArithCtrGe* c) override;
   void apply(const ArithCtrGt* c) override;
   void apply(const ArithCtrIn* c) override;

private:
   Dag* dag_;        // DAG
   Constraint c_;    // constraint visited
   size_t index_;    // resulting constraint index in the DAG

   void make(const Term& t, Scope scop, const Interval& img);
};

/*----------------------------------------------------------------------------*/

/// Visitor used to insert a term in a DAG
class DagTermCreator : public TermVisitor {
public:
   /// Constructor
   DagTermCreator(Dag* dag);

   /// Returns the DAG index of the root node of the visited term
   size_t index();

   void apply(const TermCst* t) override;
   void apply(const TermVar* t) override;
   void apply(const TermAdd* t) override;
   void apply(const TermSub* t) override;
   void apply(const TermMul* t) override;
   void apply(const TermDiv* t) override;
   void apply(const TermMin* t) override;
   void apply(const TermMax* t) override;
   void apply(const TermUsb* t) override;
   void apply(const TermAbs* t) override;
   void apply(const TermSgn* t) override;
   void apply(const TermSqr* t) override;
   void apply(const TermSqrt* t) override;
   void apply(const TermPow* t) override;
   void apply(const TermExp* t) override;
   void apply(const TermLog* t) override;
   void apply(const TermCos* t) override;
   void apply(const TermSin* t) override;
   void apply(const TermTan* t) override;
   void apply(const TermCosh* t) override;
   void apply(const TermSinh* t) override;
   void apply(const TermTanh* t) override;

private:
   Dag* dag_;           // DAG
   IndexList lsub_;     // list of DAG indexes of sub-nodes
   size_t index_;       // DAG index of the root node

   void visitSubnodes(const TermOp* t);
};

} // namespace

#endif
