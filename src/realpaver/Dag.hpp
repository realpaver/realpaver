///////////////////////////////////////////////////////////////////////////////
// This file is part of Realpaver, an interval constraint and NLP solver.    //
//                                                                           //
// Copyright (c) 2017-2022 LS2N, Nantes                                      //
//                                                                           //
// Realpaver is a software distributed WITHOUT ANY WARRANTY; read the file   //
// COPYING for information.                                                  //
///////////////////////////////////////////////////////////////////////////////

#ifndef REALPAVER_DAG_HPP
#define REALPAVER_DAG_HPP

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "realpaver/Bitset.hpp"
#include "realpaver/Constraint.hpp"
#include "realpaver/IntervalFunction.hpp"
#include "realpaver/LPModel.hpp"
#include "realpaver/RealFunction.hpp"

namespace realpaver {

// forward declarations
class Dag;
class DagVisitor;

// list of indexes
typedef std::vector<size_t> IndexList;

///////////////////////////////////////////////////////////////////////////////
/// This is the base class of DAG nodes.
///
/// A node has an index in the DAG, a list a sub-nodes and a list of parent
/// nodes. It encloses real and interval numbers resulting from evaluation and
/// differentiation operations.
///////////////////////////////////////////////////////////////////////////////
class DagNode {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param index DAG index of this
   DagNode(Dag* dag, size_t index = 0);

   /// Virtual destructor
   virtual ~DagNode();

   /// @return the DAG index of this
   size_t index() const;

   /// Assigns the DAG index
   /// @param i DAG index of this node
   void setIndex(size_t i);

   /// @return the number of parent nodes
   size_t parArity() const;

   /// Gets a parent node
   /// @param i index of a parent node in the list of parent nodes
   ///          with 0 <= i < parArity()
   /// @return DAG index of the i-th parent node
   size_t parNode(size_t i) const;

   /// Inserts a parent node
   /// @param i DAG index of a parent node
   void addParNode(size_t i);
   
   /// @return the number of sub-nodes
   size_t subArity() const;

   /// Gets a sub-node
   /// @param i index of a sub-node in the list of sub-nodes
   ///        with 0 <= i < subArity()
   /// @return the DAG index of the i-th sub-node
   size_t subIndex(size_t i) const;

   /// Gets a sub-node
   /// @param i index of a sub-node in the list of sub-nodes
   ///        with 0 <= i < subArity()
   /// @return the i-th sub-node of this
   DagNode* subNode(size_t i) const;

   /// Inserts a sub-node
   /// @param i DAG index of a sub-node
   void addSubNode(size_t i);

   /// @return left child for a binary operation
   DagNode* left() const;

   /// @return right child for a binary operation
   DagNode* right() const;

   /// @return child for a unary operation
   DagNode* child() const;

   /// @return the DAG
   Dag* dag() const;

   /// @return the bitset this depends on
   const Bitset& bitset() const;

   /// Dependency test
   /// @param bs a bitset representing a set of variables
   /// @return true if this depends on bs
   bool dependsOn(const Bitset& bs) const;

   /// Dependency test
   /// @param v a variable
   /// @return true if v occurs in the tree rooted by this
   bool dependsOn(const Variable& v) const;

   /// @return the interval value
   Interval val() const;

   /// Assigns the interval value
   /// @param x new interval value
   void setVal(const Interval& x);

   /// @return the domain (projection)
   Interval dom() const;

   /// Assigns the domain (projection)
   /// @param x new interval value
   void setDom(const Interval& x);

   /// Reduces the domain (projection)
   /// @param x interval value intersected with the domain of this
   void reduceDom(const Interval& x);

   /// @return the interval derivative
   Interval dv() const;

   /// Assigns the interval derivative
   /// @param x new interval value
   void setDv(const Interval& x);

   /// Adds an interval derivative
   /// @param x interval value added in the interval derivative
   void addDv(const Interval& x);

   /// @return the real value
   double rval() const;

   /// Assigns the real value
   /// @param x new real value
   void setRval(double x);

   /// @return the real derivative
   double rdv() const;

   /// Assigns the real derivative
   /// @param x new real value
   void setRdv(double x);

   /// Adds a real derivative
   /// @param x real value added in the real derivative
   void addRdv(double x);

   /// @return the index of a linear variable representing this in a LP model
   int indexLinVar() const;

   /// Assigns the index of a linear variable representing this in a LP model
   /// @param i index of a linear variable
   void setIndexLinVar(int i);

   /// Linearizes this node
   /// @param lm output LP model
   void linearize(LPModel& lm);

   /// Visitor pattern
   /// @param vis a visitor
   virtual void acceptVisitor(DagVisitor& vis) const = 0;

   /// Output on a stream
   /// @param os an output stream
   virtual void print(std::ostream& os) const = 0;

   /// Interval evaluation of this
   /// @param reg the variable domains
   /// The result is assigned in the interval value enclosed in this.
   virtual void eval(const IntervalRegion& reg) = 0;

   /// Interval evaluation of this
   /// @param pt the variable values
   /// The result is assigned in the interval value enclosed in this.
   virtual void eval(const RealPoint& pt) = 0;

   /// Restricted interval evaluation
   /// @param v a variable
   /// @param x domain of v
   ///
   /// This is evaluated only if it depends on v and the result is assigned
   /// in the interval value enclosed in this.
   virtual void evalOnly(const Variable& v, const Interval& x) = 0;

   /// Interval projection
   /// @param reg the variable domains
   ///
   /// It assumes that an interval evaluation has been done.
   /// The new projections are assigned in reg.
   virtual void proj(IntervalRegion& reg) = 0;

   /// Interval differentiation in reverse mode
   /// @return false if some operation at this node is not continuous
   ///
   /// It assumes that an interval evaluation has been done.
   virtual bool diff() = 0;

   /// Restricted interval differentiation in reverse mode
   /// @param v a variable
   ///
   /// This is differentiated only if it depends on v.
   /// It assumes that an interval evaluation has been done.
   virtual bool diffOnly(const Variable& v) = 0;

   /// Real (point) evaluation
   /// @param pt variable values
   ///
   /// The result is assigned in the real value enclosed.
   virtual void reval(const RealPoint& pt) = 0;

   /// Real (point) differentiation in reverse mode
   /// @return false if some operation at this node is not continuous.
   ///
   /// It assumes that a point evaluation has been done.
   virtual bool rdiff() = 0;

protected:
   Bitset bitset_;   // set of variables occurring in this

   // inserts constraints for this node in a LP model
   virtual void linearizeImpl(LPModel& lm) = 0;

private:
   Dag* dag_;        // the DAG
   size_t index_;    // index of this node in the DAG
   IndexList vpar_;  // list of indexes of parent nodes in the DAG
   IndexList vsub_;  // list of indexes of sub-nodes in the DAG
   Interval val_;    // result of an interval evaluation
   Interval dv_;     // interval derivative
   double rval_;     // result of a point evaluation
   double rdv_;      // point derivative
   int ilv_;         // index of a linear variable in a LP model
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing an interval constant.
///////////////////////////////////////////////////////////////////////////////
class DagConst : public DagNode {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param DAG index
   /// @param x value enclosed
   DagConst(Dag* dag, size_t index, const Interval& x);

   ///@{
   /// Overrides (DagNode)
   void print(std::ostream& os) const;
   void acceptVisitor(DagVisitor& vis) const;
   void eval(const IntervalRegion& reg);
   void eval(const RealPoint& pt);
   void evalOnly(const Variable& v, const Interval& x);
   void proj(IntervalRegion& reg);
   bool diff();
   bool diffOnly(const Variable& v);
   void reval(const RealPoint& pt);
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}

   /// @return the constant interval value
   Interval getConst() const;

private:
   Interval x_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing a variable.
///////////////////////////////////////////////////////////////////////////////
class DagVar : public DagNode {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param DAG index
   /// @param v variable enclosed
   DagVar(Dag* dag, size_t index, Variable v);

   /// @return the variable enclosed
   Variable getVar() const;

   ///@{
   /// Overrides (DagNode)
   void print(std::ostream& os) const;
   void acceptVisitor(DagVisitor& vis) const;
   void eval(const IntervalRegion& reg);
   void eval(const RealPoint& pt);
   void evalOnly(const Variable& v, const Interval& x);
   void proj(IntervalRegion& reg);
   bool diff();
   bool diffOnly(const Variable& v);
   void reval(const RealPoint& pt);
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}

private:
   Variable v_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is the base class of operation DAG nodes.
///////////////////////////////////////////////////////////////////////////////
class DagOp : public DagNode {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param symb operation symbol
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagOp(Dag* dag, OpSymbol symb, const IndexList& lsub);

   /// Virtual destructor
   virtual ~DagOp();

   /// Assigns the DAG index of this
   /// @param index DAG index of this
   ///
   /// It makes the links between this node and its sub-nodes.
   void setIndex(size_t index);

   /// @return the symbol enclosed
   OpSymbol getSymbol() const;

   /// Equality test
   /// @param other another operation DAG node
   /// @return true if this and other are equal
   bool eq(const DagOp* other) const;

   /// Equality test of relation symbols
   /// @param other another operation DAG node
   /// @return true if this and other have the same operation symbol
   virtual bool eqSymbol(const DagOp* other) const;

   /// Interval evaluation of this node using the interval values of the
   /// children nodes
   virtual void eval() = 0;

   /// Real evaluation of this node using the real values of the
   /// children nodes
   virtual void reval() = 0;

   ///@{
   /// Overrides (DagNode)
   virtual void print(std::ostream& os) const;
   void eval(const IntervalRegion& reg);
   void eval(const RealPoint& pt);
   void evalOnly(const Variable& v, const Interval& x);
   bool diffOnly(const Variable& v);
   void reval(const RealPoint& pt);
   ///@}

private:
   OpSymbol symb_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing a binary addition.
///////////////////////////////////////////////////////////////////////////////
class DagAdd : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagAdd(Dag* dag, const IndexList& lsub);

   ///@{
   /// Overrides (DagOp)
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}
};
 
///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing a binary subtraction.
///////////////////////////////////////////////////////////////////////////////
class DagSub : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagSub(Dag* dag, const IndexList& lsub);

   ///@{
   /// Overrides (DagOp)
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing a binary multiplication.
///////////////////////////////////////////////////////////////////////////////
class DagMul : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagMul(Dag* dag, const IndexList& lsub);

   ///@{
   /// Overrides (DagOp)
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing a division.
///////////////////////////////////////////////////////////////////////////////
class DagDiv : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagDiv(Dag* dag, const IndexList& lsub);

   ///@{
   /// Overrides (DagOp)
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing a binary minimum.
///////////////////////////////////////////////////////////////////////////////
class DagMin : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagMin(Dag* dag, const IndexList& lsub);

   ///@{
   /// Overrides (DagOp)
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing a binary maximum.
///////////////////////////////////////////////////////////////////////////////
class DagMax : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagMax(Dag* dag, const IndexList& lsub);

   ///@{
   /// Overrides (DagOp)
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing a unary subtraction.
///////////////////////////////////////////////////////////////////////////////
class DagUsb : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagUsb(Dag* dag, const IndexList& lsub);

   ///@{
   /// Overrides (DagOp)
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing an absolute value.
///////////////////////////////////////////////////////////////////////////////
class DagAbs : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagAbs(Dag* dag, const IndexList& lsub);

   ///@{
   /// Overrides (DagOp)
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing a sign function.
///////////////////////////////////////////////////////////////////////////////
class DagSgn : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagSgn(Dag* dag, const IndexList& lsub);

   ///@{
   /// Overrides (DagOp)
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing a square.
///////////////////////////////////////////////////////////////////////////////
class DagSqr : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagSqr(Dag* dag, const IndexList& lsub);

   ///@{
   /// Overrides (DagOp)
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing a square root.
///////////////////////////////////////////////////////////////////////////////
class DagSqrt : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagSqrt(Dag* dag, const IndexList& lsub);

   ///@{
   /// Overrides (DagOp)
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing the power operation.
///////////////////////////////////////////////////////////////////////////////
class DagPow : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param n exponent
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagPow(Dag* dag, int n, const IndexList& lsub);

   /// @return the exponent
   int exponent() const;

   ///@{
   /// Overrides (DagOp)
   bool eqSymbol(const DagOp* other) const;
   void print(std::ostream& os) const;
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}

private:
   int n_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing the exponential function.
///////////////////////////////////////////////////////////////////////////////
class DagExp : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagExp(Dag* dag, const IndexList& lsub);

   ///@{
   /// Overrides (DagOp)
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing the logarithm function.
///////////////////////////////////////////////////////////////////////////////
class DagLog : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagLog(Dag* dag, const IndexList& lsub);

   ///@{
   /// Overrides (DagOp)
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing the cosine function.
///////////////////////////////////////////////////////////////////////////////
class DagCos : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagCos(Dag* dag, const IndexList& lsub);

   ///@{
   /// Overrides (DagOp)
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing the sine function.
///////////////////////////////////////////////////////////////////////////////
class DagSin : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagSin(Dag* dag, const IndexList& lsub);

   ///@{
   /// Overrides (DagOp)
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG node representing the tangent function.
///////////////////////////////////////////////////////////////////////////////
class DagTan : public DagOp {
public:
   /// Creates a node
   /// @param dag owner of this
   /// @param lsub list of DAG indexes of the sub-nodes of this
   DagTan(Dag* dag, const IndexList& lsub);

   ///@{
   /// Overrides (DagOp)
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalRegion& reg);
   bool diff();
   void reval();
   bool rdiff();
   void linearizeImpl(LPModel& lm);
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a function in a DAG.
///
/// To every function is associated an image, i.e. we have L <= f(x) <= U.
///////////////////////////////////////////////////////////////////////////////
class DagFun : public DiffIntervalFunction, public DiffRealFunction {
public:
   /// Creates a function
   /// @param dag owner of this
   /// @param root DAG index of the root node
   /// @param image image of this
   DagFun(Dag* dag, size_t root, const Interval& image);

   /// @return the image of this
   Interval getImage() const;

   /// Assigns the image of this
   /// @param x new value
   void setImage(const Interval& x);

   /// @return the dag
   Dag* dag() const;

   /// Inserts a constant node
   /// @param node a node
   void insertConstNode(DagConst* node);

   /// Inserts a variable node
   /// @param node a node
   void insertVarNode(DagVar* node);

   /// Inserts an operation node
   /// @param node a node
   void insertOpNode(DagOp* node);

   /// @return the number of nodes of this
   size_t nbNodes() const;

   /// Gets a node of this
   /// @param i index of a node with 0 <= i < nbNodes()
   /// @return the i-th node of this
   DagNode* node(size_t i) const;

   /// @return the root node of this
   DagNode* rootNode() const;

   /// @return the number of variables occurring in this
   size_t nbVars() const;

   /// Gets a variable
   /// @param i index of a variable with 0 <= i < nbVars()
   /// @return the i-th variable of this
   DagVar* varNode(size_t i) const;

   /// Dependency test
   /// @param bs a bitset representing a set of variables
   /// @return true if this depends on bs
   bool dependsOn(const Bitset& bs) const;

   /// Dependency test
   /// @param v a variable
   /// @return true if v occurs in this
   bool dependsOn(const Variable& v) const;

   /// Gets the bitset representing the set of variables this depends on
   const Bitset& bitset() const;

   /// Assigns the index of this in the dag
   /// @param i new value
   void setIndex(size_t i);

   /// @return the index of this in the dag
   size_t index() const;

   /// @return the scope of this (set of variables)
   Scope scope() const;

   /// Assigns the scope of this
   /// @param s scope assigned
   void setScope(Scope s);

   /// Interval evaluation of this
   /// @param reg the variable domains
   /// @return the evaluation of this at reg
   Interval eval(const IntervalRegion& reg);

  /// Interval evaluation
   /// @param pt variable values
   /// @return the evaluation of this at pt
   Interval eval(const RealPoint& pt);

   /// @return the result of an interval evaluation
   Interval val() const;

   /// Restricted interval evaluation
   /// @param v a variable
   /// @param x domain of v
   /// @return the interval evaluation such that only the nodes depending
   ///         on v are evaluated.
   Interval evalOnly(const Variable& v, const Interval& x);

   /// HC4Revise contractor
   /// @param reg the variable domains
   /// @return a certificate of proof
   ///
   /// The projections of this onto its variables are assigned in
   /// the given region.
   Proof hc4Revise(IntervalRegion& reg);

   /// HC4Revise contractor on the constraint negation
   /// @param reg the variable domains
   /// @return a certificate of proof
   ///
   /// The projections of this onto its variables are assigned in
   /// the given region.
   Proof hc4ReviseNeg(IntervalRegion& reg);

   /// HC4Revise contractor with node sharing
   /// @param reg the variable domains
   /// @return a certificate of proof
   ///
   /// The projections of this onto its variables are assigned in
   /// the given region.
   ///
   /// It assumes that a DagContext has been initialized.
   Proof sharedHc4Revise(IntervalRegion& reg);

   /// Interval differentiation in reverse mode
   /// @param reg the variable domains
   /// @return false if this function is discontinuous, true otherwise
   ///
   /// It evaluates first this function and then calculates the derivatives.
   bool diff(const IntervalRegion& reg);

   /// Interval differentiation in reverse mode
   /// @return false if this function is discontinuous, true otherwise
   ///
   /// It assumes that this function has been evaluated.
   bool diff();

   /// Restricted interval differentiation in forward mode
   /// @param v a variable
   ///
   /// It assumes that this function has been evaluated.
   /// Only the nodes depending on the considered variable are differentiated.
   bool diffOnly(const Variable& v);

   /// Restricted interval differentiation in forward mode
   /// @param v a variable
   /// @param x domain of v
   ///
   /// This function is first evaluated.
   /// Only the nodes depending on the considered variable are differentiated.   
   bool diffOnly(const Variable& v, const Interval& x);

   /// Real (point) evaluation
   /// @param pt variable values
   /// @return the evaluation of this at pt
   double reval(const RealPoint& pt);

   /// @return the result of the point evaluation
   double rval() const;

   /// Real differentiation in reverse mode
   /// @param pt variable values
   /// @return false if this function is discontinuous, true otherwise
   ///
   /// It evaluates first this function and then calculates the derivatives.
   bool rdiff(const RealPoint& pt);

   /// Point differentiation in reverse mode
   /// @return false if this function is discontinuous, true otherwise
   ///
   /// It assumes that this function has been evaluated.
   bool rdiff();

   ///@{
   /// Overrides
   Scope  funScope() const;
   size_t funArity() const;

   Interval intervalEval     (const IntervalRegion& reg);
   Interval intervalPointEval(const RealPoint& pt);
   void     intervalDiff     (const IntervalRegion& reg, IntervalVector& g);
   Interval intervalEvalDiff (const IntervalRegion& reg, IntervalVector& g);

   double realEval    (const RealPoint& pt);
   void   realDiff    (const RealPoint& pt, RealVector& g);
   double realEvalDiff(const RealPoint& pt, RealVector& g);
   ///@}

   /// @return the interval gradient after a differentiation
   IntervalVector intervalGradient() const;

   /// Assigns the interval gradient after a differentiation
   /// @param G the gradient assigned
   void toIntervalGradient(IntervalVector& G) const;

   /// Gets an interval derivative after a differentiation
   /// @param i index of a variable in this with 0 <= i < nbVars()
   /// @return the partial derivative with respect to the i-th variable
   Interval intervalDeriv(size_t i) const;

   /// Gets an interval derivative after a differentiation
   /// @param v a variable occurring in this
   /// @return the partial derivative of this with respect to v
   Interval intervalDeriv(const Variable& v) const;

   /// @return the real gradient after a differentiation
   RealVector realGradient() const;

   /// Assigns the real gradient after a differentiation
   /// @param G the gradient assigned
   void toRealGradient(RealVector& G) const;

   /// Gets a real derivative after a differentiation
   /// @param i index of a variable in this with 0 <= i < nbVars()
   /// @return the partial derivative with respect to the i-th variable
   double realDeriv(size_t i) const;

   /// Gets a real derivative after a differentiation
   /// @param v a variable occurring in this
   /// @return the partial derivative of this with respect to v
   double realDeriv(const Variable& v) const;

   /// Linearizes this function
   /// @param lm output LP model
   ///
   /// Each node of this such that the index of the corresponding linear
   /// variable is less than 0 is linearized
   void linearize(LPModel& lm);

private:
   Dag* dag_;                    // the DAG
   std::vector<DagNode*> node_;  // vector of nodes of the function sorted by
                                 // a topological ordering from the leaves to
                                 // the root (back element of the vector)
   std::vector<DagVar*> vnode_;  // vector of variable nodes of the function
                                 // sorted by an ascending ordering of the
                                 // identifiers of the variables
   Scope scope_;                 // the set of variables
   Interval image_;              // bounds of the function, i.e. the co-domain
   size_t idx_;                  // index in the dag

   // tests if a node is already present in this function
   bool hasNode(DagNode* node) const;

   // set of node indexes used to implement hasNode in O(1)
   std::unordered_set<size_t> inode_;

   // backward phase of the Hc4Revise contractor
   // to be called after the assignment of the projection over the root node 
   Proof hc4ReviseBack(IntervalRegion& reg);
};

///////////////////////////////////////////////////////////////////////////////
/// This is a context such that every dag node is associated with an interval
/// domain.
///////////////////////////////////////////////////////////////////////////////
struct DagContext
{
   IntervalVector dom;

   /// Creates an empty context
   DagContext();

   /// Creates a context
   /// @param n number of domains
   DagContext(size_t n);

   /// Default copy constructor
   DagContext(const DagContext&) = default;

   /// Extends a context
   /// @param n number of new domains inserted in this
   void extend(size_t n = 1);

   /// @return a clone
   DagContext* clone() const;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a DAG representing an expression graph.
///
/// An expression graph represents a constraint system L <= f(x) <= U with
/// a function vector f : Rn -> Rm and bounds U and L in Rm.
///////////////////////////////////////////////////////////////////////////////
class Dag {
public:
   /// Creates an empty DAG
   Dag();

   /// Destructor
   ~Dag();

   /// no copy
   Dag(const Dag&) =delete;

   /// No assignment
   Dag& operator=(const Dag&) =delete;

   /// @return the number of nodes
   size_t nbNodes() const;

   /// Gets a node
   /// @param i a node index with 0 <= i < nbNodes()
   /// @return the i-th node
   DagNode* node(size_t i) const;

   /// @return the number of functions
   size_t nbFuns() const;

   /// Gets a function
   /// @param i a function index with 0 <= i < nbFuns()
   /// @return the i-th function
   DagFun* fun(size_t i) const;

   /// @return the number of variables
   size_t nbVars() const;

   /// Gets a variable
   /// @param i an variable index with 0 <= i < nbVars()
   /// @return the i-th variable node
   DagVar* varNode(size_t i) const;

   /// Gets a variable node
   /// @param id identifier of a variable
   /// @returns the node representing the variable id if it exists,
   ///          nullptr otherwise
   DagVar* findVarNode(size_t id) const;

   /// Inserts a constraint
   /// @param c a constraint
   /// @return the index of c in the DAG
   ///
   /// Throws an exception if the constraint cannot be handled.
   size_t insert(Constraint c);

   /// Inserts a constraint given a term and an image
   /// @param t expression of a function
   /// @param image image of the function
   /// @return the index of the constraint in the DAG
   ///
   /// Throws an exception if the constraint cannot be handled.
   size_t insert(Term t, const Interval& img = Interval::universe());

   /// Inserts a node representing an interval
   /// @param x an interval
   /// @return the index of the node in the DAG
   ///
   /// A new node is created only if it does not already exists in the DAG.
   size_t insertConstNode(const Interval& x);

   /// Inserts a node representing a variable
   /// @param v a variable
   /// @return the index of the node in the DAG
   ///
   /// A new node is created only if it does not already exists in the DAG.
   size_t insertVarNode(const Variable& v);

   /// Inserts a node representing an operation
   /// @param hcode hash code of tje input node
   /// @param node an operation node
   /// @returns the index of node in the DAG
   ///
   /// A new node is created only if it does not already exists in the DAG.
   /// Otherwise, node is deleted.
   size_t insertOpNode(size_t hcode, DagOp* node);

   /// Inserts a function
   /// @param f a function
   /// @return the function index in the DAG
   size_t insertFun(DagFun* f);

   /// @return the scope of this
   ///
   /// The scope is created.
   Scope scope() const;

   /// Output on a stream
   /// @param os an output stream
   void print(std::ostream& os) const;

   // management of context
   void bindContext(DagContext* context);
   DagContext* unbindContext();
   DagContext* cloneDefaultContext() const;

   /// Gets the domain of a node
   /// @param i a node index
   /// @return the domain of the i-th node
   Interval dom(size_t i) const;

   /// Assigns the domain of a node
   /// @param i a node index
   /// @patam x new domain for the i-th node
   void setDom(size_t i, const Interval& x);

   /// Reducesthe domain of a node
   /// @param i a node index
   /// @patam x an interval intersected with the domain of the i-th node
   void reduceDom(size_t i, const Interval& x);

   /// Interval evaluation
   /// @param reg domains of variables
   void eval(const IntervalRegion& reg);

   /// Real (point) evaluation
   /// @param pt values of variables
   void reval(const RealPoint& pt);

   /// Linearizes the DAG
   /// @param lm output LP model
   ///
   /// Assumes that the DAG has been evaluated.
   void linearize(LPModel& lm);

   /// Linearizes a part of the DAG
   /// @param lm output LP model
   /// @param bs bitset such that bs[i] = 1 if the i-th function is linearized
   ///
   /// Assumes that the functions in the DAG have been evaluated.
   void linearize(LPModel& lm, const Bitset& bs);

private:
   // vector of nodes sorted by a topological ordering from the leaves
   // to the roots
   std::vector<DagNode*> node_;

   // vector of constant nodes
   std::vector<DagConst*> cnode_;

   // vector of variable nodes sorted by an ascending ordering
   // of the variable identifiers
   std::vector<DagVar*> vnode_;

   // vector of operation nodes by a topological ordering from the leaves
   // to the roots
   std::vector<DagOp*> onode_;

   // vector of functions
   std::vector<DagFun*> fun_;

   // map (variable id -> node index)
   std::unordered_map<size_t, size_t> vmap_;

   // map (hash code of an operation node -> list of node indexes)
   std::unordered_map<size_t, IndexList> omap_;

   // current context that stores an interval domain for each node of this dag
   DagContext* context_;

   // default context
   DagContext* defaultContext_;

   // find methods
   DagConst* findConstNode(const Interval& x) const;
   DagOp* findOpNode(size_t hcode, DagOp* node) const;

   // pushes a new node in the vector of nodes
   // extends the size of the default context
   void pushNode(DagNode* node);
};

/// Output on a stream
/// @param os an output stream
/// @param a dag
/// @return a reference to os
std::ostream& operator<<(std::ostream& os, const Dag& dag);

///////////////////////////////////////////////////////////////////////////////
/// This is the base class of visitors of DAG nodes.
///////////////////////////////////////////////////////////////////////////////
class DagVisitor {
public:
   /// Virtual destructor
   virtual ~DagVisitor();

   ///@{
   /// Visit methods
   /// The default impementations raise exceptions.
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
   ///@}
};

///////////////////////////////////////////////////////////////////////////////
/// This is a visitor used to create a function in a DAG.
///////////////////////////////////////////////////////////////////////////////
class DagFunCreator : public DagVisitor {
public:
   /// Creates a visitor
   /// @param f function built by the visit
   DagFunCreator(DagFun* f);

   ///@{
   /// Overrides
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
   ///@}

private:
   DagFun* f_;

   void visitSubNodes(const DagOp* d);
};

///////////////////////////////////////////////////////////////////////////////
/// This is a visitor used to insert a constraint in a DAG.
///////////////////////////////////////////////////////////////////////////////
class DagCreator : public ConstraintVisitor {
public:
   /// Creates a visitor
   /// @param dag DAG modified by a visit
   /// @param c constraint visited
   DagCreator(Dag* dag, Constraint c);

   /// @return the constraint index in the DAG
   size_t index();

   ///@{
   /// Overrides
   void apply(const ConstraintEq* c);
   void apply(const ConstraintLe* c);
   void apply(const ConstraintLt* c);
   void apply(const ConstraintGe* c);
   void apply(const ConstraintGt* c);
   void apply(const ConstraintIn* c);
   ///@}

private:
   Dag* dag_;
   Constraint c_;
   size_t index_;
};

///////////////////////////////////////////////////////////////////////////////
/// This is a visitor used to insert a term in a DAG.
///////////////////////////////////////////////////////////////////////////////
class DagTermCreator : public TermVisitor {
public:
   /// Creates a visitor
   /// @param dag DAG modified by a visit
   DagTermCreator(Dag* dag);

   /// @returns the DAG index of the root node of the visited term
   size_t index();

   ///@{
   /// Overrides
   void apply(const TermConst* t);
   void apply(const TermVar* t);
   void apply(const TermAdd* t);
   void apply(const TermSub* t);
   void apply(const TermMul* t);
   void apply(const TermDiv* t);
   void apply(const TermMin* t);
   void apply(const TermMax* t);
   void apply(const TermUsb* t);
   void apply(const TermAbs* t);
   void apply(const TermSgn* t);
   void apply(const TermSqr* t);
   void apply(const TermSqrt* t);
   void apply(const TermPow* t);
   void apply(const TermExp* t);
   void apply(const TermLog* t);
   void apply(const TermCos* t);
   void apply(const TermSin* t);
   void apply(const TermTan* t);
   ///@}

private:
   Dag* dag_;
   IndexList lsub_;     // list of DAG indexes of sub-nodes
   size_t index_;       // DAG index of the root node

   void visitSubnodes(const TermOp* t);
};

} // namespace

#endif
