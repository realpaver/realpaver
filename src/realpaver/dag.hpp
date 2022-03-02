// This file is part of Realpaver. License: see COPYING file.

#ifndef REALPAVER_DAG_HPP
#define REALPAVER_DAG_HPP

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "realpaver/Bitset.hpp"
#include "realpaver/Constraint.hpp"
#include "realpaver/IntervalFunction.hpp"
#include "realpaver/RealFunction.hpp"

namespace realpaver {

// forward declarations
class Dag;
class DagVisitor;

// list of indexes
typedef std::vector<size_t> IndexList;

/*****************************************************************************
 * Base class of DAG nodes.
 *****************************************************************************/
class DagNode {
public:
   // constructor
   DagNode(Dag* dag, size_t index = 0);

   // destructor
   virtual ~DagNode();

   // returns the node index in the DAG
   size_t index() const;

   // assigns the node index
   void setIndex(size_t i);

   // returns the number of parent nodes
   size_t parArity() const;

   // returns the index if the i-th parent node
   size_t parNode(size_t i) const;

   // inserts a parent node with index i
   void addParNode(size_t i);
   
   // returns the number of sub-nodes
   size_t subArity() const;

   // returns the i-th sub-node
   size_t subIndex(size_t i) const;
   DagNode* subNode(size_t i) const;

   // inserts a sub-node with index i
   void addSubNode(size_t i);

   // left child for a binary operation
   DagNode* left() const;

   // right child for a binary operation
   DagNode* right() const;

   // child for a unary operation
   DagNode* child() const;

   // returns the DAG
   Dag* dag() const;

   // constant access to the bitset
   const Bitset& bitset() const;

   // tests if this node depends on the variables defined by a bitset
   bool dependsOn(const Bitset& bs) const;

   // tests if this node depends on a variable id
   bool dependsOn(size_t id) const;

   // getter/setter of the interval value
   const Interval& val() const;
   void setVal(const Interval& x);

   // getter/setter of the domain / projection value
   Interval dom() const;
   void setDom(const Interval& x);
   void reduceDom(const Interval& x);

   // getter/setter of the interval derivative
   const Interval& dv() const;
   void setDv(const Interval& x);
   void addDv(const Interval& x);

   // getter/setter of the point value
   const double& rval() const;
   void setRval(const double& x);

   // getter/setter of the point derivative
   const double& rdv() const;
   void setRdv(const double& x);
   void addRdv(const double& x);

   // returns the number of occurrences of the variable id in the sub-tree
   // rooted by this
   virtual size_t nbOcc(size_t id) const = 0;

   // visitor pattern
   virtual void acceptVisitor(DagVisitor& vis) const = 0;

   // output on a stream
   virtual void print(std::ostream& os) const = 0;

   // interval evaluation given the vector of variable domains X
   // assigns the result in val_
   virtual void eval(const IntervalVector& X) = 0;

   // interval evaluation given the vector of variable values P
   // assigns the result in val_
   virtual void eval(const RealVector& P) = 0;

   // interval evaluation given a new domain x for the variable id
   // this node is evaluated only if it depends on this variable
   // assigns the result in val_
   virtual void evalOnly(size_t id, const Interval& x) = 0;

   // interval projection
   // assumes that an interval evaluation has been done
   virtual void proj(IntervalVector& X) = 0;

   // interval differentiation in reverse mode
   // assumes that an interval evaluation has been done
   // returns false if some operation at this node is not continuous
   virtual bool diff() = 0;

   // interval differentiation in reverse mode
   // differentiates only the nodes depending on variable id
   // assumes that an interval evaluation has been done
   // returns false if some operation at this node is not continuous
   virtual bool diffOnly(size_t id) = 0;

   // point evaluation given the vector of variable values P
   // assigns the result in rval_
   virtual void reval(const RealVector& P) = 0;

   // point differentiation in reverse mode
   // assumes that a point evaluation has been done
   // returns false if some operation at this node is not continuous
   virtual bool rdiff() = 0;

protected:
   Bitset bitset_;   // variable dependance
   
private:
   Dag* dag_;        // the DAG
   size_t index_;    // index of this node in the DAG
   IndexList vpar_;  // indexes of parent nodes in the DAG
   IndexList vsub_;  // indexes of sub-nodes in the DAG
   Interval val_;    // result of an interval evaluation
   Interval dv_;     // interval derivative
   double rval_;     // result of a point evaluation
   double rdv_;      // point derivative
};

/*****************************************************************************
 * Class of constant DAG nodes.
 *****************************************************************************/
class DagConst : public DagNode {
public:
   // constructor
   DagConst(Dag* dag, size_t index, const Interval& x);

   // Override
   size_t nbOcc(size_t id) const;
   void print(std::ostream& os) const;
   void acceptVisitor(DagVisitor& vis) const;
   void eval(const IntervalVector& X);
   void eval(const RealVector& P);
   void evalOnly(size_t id, const Interval& x);
   void proj(IntervalVector& X);
   bool diff();
   bool diffOnly(size_t id);
   void reval(const RealVector& P);
   bool rdiff();

   // returns the constant interval value
   const Interval& getConst() const;

private:
   Interval x_;
};

inline const Interval& DagConst::getConst() const
{
   return x_;
}

/*****************************************************************************
 * Class of variable DAG nodes.
 *****************************************************************************/
class DagVar : public DagNode {
public:
   // constructor
   DagVar(Dag* dag, size_t index, const Variable& v);

   // returns the variable enclosed
   Variable getVar() const;

   // Override
   size_t nbOcc(size_t id) const;
   void print(std::ostream& os) const;
   void acceptVisitor(DagVisitor& vis) const;
   void eval(const IntervalVector& X);
   void eval(const RealVector& P);
   void evalOnly(size_t id, const Interval& x);
   void proj(IntervalVector& X);
   bool diff();
   bool diffOnly(size_t id);
   void reval(const RealVector& P);
   bool rdiff();

private:
   Variable v_;
};

inline Variable DagVar::getVar() const
{
   return v_;
}

/*****************************************************************************
 * Base class of DAG nodes representing an operation.
 *****************************************************************************/
class DagOp : public DagNode {
public:
   // constructor
   DagOp(Dag* dag, OpSymbol symb, const IndexList& lsub);

   // destructor
   virtual ~DagOp();

   // assigns the node index and make the links between its sub-nodes
   // and this node which becomes a parent
   void setIndex(size_t index);

   // returns the symbol enclosed
   OpSymbol getSymbol() const;

   // returns true if this node is equal to another node
   bool eq(const DagOp* other) const;

   // returns true if this and other have the same operation symbol
   virtual bool eqSymbol(const DagOp* other) const;

   // evaluates this node using the interval values of the children nodes
   virtual void eval() = 0;

   // evaluates this node using the point values of the children nodes
   virtual void reval() = 0;

   // Override
   size_t nbOcc(size_t id) const;
   virtual void print(std::ostream& os) const;
   void eval(const IntervalVector& X);
   void eval(const RealVector& P);
   void evalOnly(size_t id, const Interval& x);
   bool diffOnly(size_t id);
   void reval(const RealVector& P);

private:
   OpSymbol symb_;
};

inline OpSymbol DagOp::getSymbol() const
{
   return symb_;
}

inline void DagOp::eval(const IntervalVector& X)
{
   eval();
}

inline void DagOp::eval(const RealVector& P)
{
   eval();
}

inline void DagOp::reval(const RealVector& P)
{
   reval();
}

/*****************************************************************************
 * Class of DAG nodes: binary addition.
 *****************************************************************************/
class DagAdd : public DagOp {
public:
   // constructor
   DagAdd(Dag* dag, const IndexList& lsub);

   // Override
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();
};
 
/*****************************************************************************
 * Class of DAG nodes: subtraction.
 *****************************************************************************/
class DagSub : public DagOp {
public:
   // constructor
   DagSub(Dag* dag, const IndexList& lsub);

   // Override
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();
};

/*****************************************************************************
 * Class of DAG nodes: binary multiplication.
 *****************************************************************************/
class DagMul : public DagOp {
public:
   // constructor
   DagMul(Dag* dag, const IndexList& lsub);

   // Override
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();
};

/*****************************************************************************
 * Class of DAG nodes: division.
 *****************************************************************************/
class DagDiv : public DagOp {
public:
   // constructor
   DagDiv(Dag* dag, const IndexList& lsub);

   // Override
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();
};

/*****************************************************************************
 * Class of DAG nodes: binary minimum.
 *****************************************************************************/
class DagMin : public DagOp {
public:
   // constructor
   DagMin(Dag* dag, const IndexList& lsub);

   // Override
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();
};

/*****************************************************************************
 * Class of DAG nodes: binary maximum.
 *****************************************************************************/
class DagMax : public DagOp {
public:
   // constructor
   DagMax(Dag* dag, const IndexList& lsub);

   // Override
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();
};

/*****************************************************************************
 * Class of DAG nodes: unary subtraction.
 *****************************************************************************/
class DagUsb : public DagOp {
public:
   // constructor
   DagUsb(Dag* dag, const IndexList& lsub);

   // Override
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();
};

/*****************************************************************************
 * Class of DAG nodes: absolute value.
 *****************************************************************************/
class DagAbs : public DagOp {
public:
   // constructor
   DagAbs(Dag* dag, const IndexList& lsub);

   // Override
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();
};

/*****************************************************************************
 * Class of DAG nodes: sign function.
 *****************************************************************************/
class DagSgn : public DagOp {
public:
   // constructor
   DagSgn(Dag* dag, const IndexList& lsub);

   // Override
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();
};

/*****************************************************************************
 * Class of DAG nodes: square.
 *****************************************************************************/
class DagSqr : public DagOp {
public:
   // constructor
   DagSqr(Dag* dag, const IndexList& lsub);

   // Override
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();
};

/*****************************************************************************
 * Class of DAG nodes: square root.
 *****************************************************************************/
class DagSqrt : public DagOp {
public:
   // constructor
   DagSqrt(Dag* dag, const IndexList& lsub);

   // Override
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();
};

/*****************************************************************************
 * Class of DAG nodes: power function with an integral exponent.
 *****************************************************************************/
class DagPow : public DagOp {
public:
   // constructor
   DagPow(Dag* dag, int n, const IndexList& lsub);

   // returns the exponent
   int exponent() const;

   // Override
   bool eqSymbol(const DagOp* other) const;
   void print(std::ostream& os) const;
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();

private:
   int n_;
};

inline int DagPow::exponent() const
{
   return n_;
}

/*****************************************************************************
 * Class of DAG nodes: exponential function.
 *****************************************************************************/
class DagExp : public DagOp {
public:
   // constructor
   DagExp(Dag* dag, const IndexList& lsub);

   // Override
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();
};

/*****************************************************************************
 * Class of DAG nodes: logarithm function.
 *****************************************************************************/
class DagLog : public DagOp {
public:
   // constructor
   DagLog(Dag* dag, const IndexList& lsub);

   // Override
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();
};

/*****************************************************************************
 * Class of DAG nodes: cosine function.
 *****************************************************************************/
class DagCos : public DagOp {
public:
   // constructor
   DagCos(Dag* dag, const IndexList& lsub);

   // Override
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();
};

/*****************************************************************************
 * Class of DAG nodes: sine function.
 *****************************************************************************/
class DagSin : public DagOp {
public:
   // constructor
   DagSin(Dag* dag, const IndexList& lsub);

   // Override
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();
};

/*****************************************************************************
 * Class of DAG nodes: tangent function.
 *****************************************************************************/
class DagTan : public DagOp {
public:
   // constructor
   DagTan(Dag* dag, const IndexList& lsub);

   // Override
   void acceptVisitor(DagVisitor& vis) const;
   void eval();
   void proj(IntervalVector& X);
   bool diff();
   void reval();
   bool rdiff();
};

/*****************************************************************************
 * Class of functions.
 * 
 * To every function is associated an image, i.e. we have L <= f(x) <= U.
 *****************************************************************************/
class DagFun : public IntervalFunction, public RealFunction {
public:
   // constructors
   DagFun(Dag* dag, size_t root, const Interval& image);

   // image of the function
   const Interval& getImage() const;
   void setImage(const Interval& x);

   // returns the dag
   Dag* dag() const;

   // insertion of nodes
   void insertConstNode(DagConst* node);
   void insertVarNode(DagVar* node);
   void insertOpNode(DagOp* node);

   // access to the nodes
   size_t nbNode() const;
   DagNode* node(size_t i) const;
   DagNode* rootNode() const;

   // access to the variables
   size_t nbVar() const;
   DagVar* varNode(size_t i) const;

   // tests if this function depends on the variables defined by a bitset
   bool dependsOn(const Bitset& bs) const;

   // tests if this function depends on a variable
   bool dependsOn(const Variable& v) const;

   // tests if this function depends on a variable of identifier id
   bool dependsOn(size_t id) const;

   // access to the bitset
   const Bitset& bitset() const;

   // returns the number of occurrences of the variable id in this
   size_t nbOcc(size_t id) const;

   // assigns the index of this function in the dag
   void setIndex(size_t i);

   // returns the index of this in the dag
   size_t index() const;

   // management of the scope
   Scope scope() const;
   void setScope(const Scope& s);

   // interval evaluation
   Interval eval(const IntervalVector& X);
   Interval eval(const RealVector& P);

   // access to the result of the interval evaluation
   Interval val() const;

   // interval evaluation given a new domain x for v
   // only the nodes depending on the variable id are evaluated
   Interval evalOnly(size_t id, const Interval& x);

   // Hc4Revise contractor
   Proof hc4Revise(IntervalVector& X);

   // Hc4Revise contractor on the constraint negation
   Proof hc4ReviseNeg(IntervalVector& X);

   // Hc4Revise contractor with node sharing
   // assumes that a DagContext has been initialized
   Proof sharedHc4Revise(IntervalVector& X);

   // interval differentiation in reverse mode
   // evaluates first this function
   // returns false if this function is discontinuous, true otherwise
   bool diff(const IntervalVector& X);

   // interval differentiation in reverse mode
   // assumes that this function has been evaluated
   // returns false if this function is discontinuous, true otherwise
   bool diff();

   // interval evaluation of a partial derivative in forward mode
   // only the nodes depending on the considered variable are evaluated
   bool diffOnly(size_t id);
   bool diffOnly(size_t id, const Interval& x);

   // access to the gradient
   IntervalVector grad() const;
   void toGrad(IntervalVector& G) const;

   // access to the partial derivative with respect to the i-th variable
   // in this function (i-th component of the gradient)
   // useful after a call to diff() and diffOnly(...)
   const Interval& deriv(size_t i) const;

   // access to the partial derivative with respect to v
   // useful after a call to diff() and diffOnly(...)
   const Interval& deriv(const Variable& v) const;

   // point evaluation
   double reval(const RealVector& P);

   // access to the result of the point evaluation
   double rval() const;

   // point differentiation in reverse mode
   // evaluates first this function
   // returns false if this function is discontinuous, true otherwise
   bool rdiff(const RealVector& P);

   // point differentiation in reverse mode
   // assumes that this function has been evaluated
   // returns false if this function is discontinuous, true otherwise
   bool rdiff();

   // access to the gradient
   RealVector rgrad() const;
   void toRgrad(RealVector& G) const;

   // access to the partial derivative with respect to the i-th variable
   // in this function (i-th component of the gradient)
   // useful after a call to rdiff()
   double rderiv(size_t i) const;

   // access to the partial derivative with respect to v
   // useful after a call to rdiff()
   double rderiv(const Variable& v) const;

   ///@{
   /// Overrides the methods of IntervalFunction
   Scope ifunScope() const;
   size_t ifunArity() const;
   Interval ifunEval(const IntervalVector& x);
   void ifunDiff(const IntervalVector& x, IntervalVector& g);
   void ifunEvalDiff(const IntervalVector& x, IntervalVector& g,
                      Interval& valf);
   ///@}

   ///@{
   /// Overrides the methods of RealFunction
   Scope rfunScope() const;
   size_t rfunArity() const;
   double rfunEval(const RealVector& x);
   void rfunDiff(const RealVector& x, RealVector& g);
   void rfunEvalDiff(const RealVector& x, RealVector& g, double& valf);
   ///@}

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
   Proof hc4ReviseBack(IntervalVector& X);
};

inline const Interval& DagFun::getImage() const
{
   return image_;
}

inline void DagFun::setImage(const Interval& x)
{
   image_ = x;
}

inline Dag* DagFun::dag() const
{
   return dag_;
}

inline size_t DagFun::nbNode() const
{
   return node_.size();
}

inline DagNode* DagFun::node(size_t i) const
{
   return node_[i];
}

inline DagNode* DagFun::rootNode() const
{
   return node_.back();
}

inline size_t DagFun::nbVar() const
{
   return vnode_.size();
}

inline DagVar* DagFun::varNode(size_t i) const
{
   return vnode_[i];
}

inline const Bitset& DagFun::bitset() const
{
   return rootNode()->bitset();
}

inline bool DagFun::dependsOn(const Bitset& bs) const
{
   return rootNode()->dependsOn(bs);
}

inline bool DagFun::dependsOn(const Variable& v) const
{
   return rootNode()->dependsOn(v.getId());
}

inline bool DagFun::dependsOn(size_t id) const
{
   return rootNode()->dependsOn(id);
}

inline const Interval& DagFun::deriv(size_t i) const
{
   return varNode(i)->dv();
}

inline void DagFun::setIndex(size_t i)
{
   idx_ = i;
}

inline size_t DagFun::index() const
{
   return idx_;
}

inline size_t DagFun::nbOcc(size_t id) const
{
   return rootNode()->nbOcc(id);
}

inline Scope DagFun::scope() const
{
   return scope_;
}

inline void DagFun::setScope(const Scope& s)
{
   scope_ = s;
}

inline double DagFun::rderiv(size_t i) const
{
   return varNode(i)->rdv();
}

inline double DagFun::rval() const
{
   return rootNode()->rval();
}

inline Interval DagFun::val() const
{
   return rootNode()->val();
}

/*****************************************************************************
 * Class of contexts such that every dag node is associated with
 * an interval domain.
 *****************************************************************************/
struct DagContext
{
   IntervalVector dom;

   // constructors
   DagContext();
   DagContext(size_t n);
   DagContext(const DagContext&) = default;

   // extends the size of this context with n intervals
   void extend(size_t n = 1);

   // clones this context
   DagContext* clone() const;
};

/*****************************************************************************
 * Main class of DAGs, i.e. expression graphs.
 * 
 * An expression graph represents a constraint system L <= f(x) <= U with
 * a function vector f : Rn -> Rm and bounds U and L in Rm.
 *****************************************************************************/
class Dag {
public:
   // constructor
   Dag();

   // destructor
   ~Dag();

   // no copy
   Dag(const Dag&) =delete;
   Dag& operator=(const Dag&) =delete;

   // returns the number of nodes
   size_t nbNode() const;

   // returns the i-th node
   DagNode* node(size_t i) const;

   // returns the number of functions
   size_t nbFun() const;

   // returns the i-th function
   DagFun* fun(size_t i) const;

   // returns the number of variables
   size_t nbVar() const;

   // returns the i-th variable node
   DagVar* varNode(size_t i) const;

   // returns the node representing the variable id
   // nullptr if not found
   DagVar* findVarNode(size_t id) const;

   // inserts a constraint and returns its index in the DAG
   // throws an exception if the constraint cannot be handled
   size_t insert(const Constraint& c);

   // inserts a constraint and returns its index in the DAG
   // throws an exception if the constraint cannot be handled
   size_t insert(const Term& t, const Interval& img = Interval::universe());

   // inserts a constant node and returns its index
   size_t insertConstNode(const Interval& x);

   // inserts a node for a variable and returns its index
   size_t insertVarNode(const Variable& v);

   // inserts an operation node and returns its index
   size_t insertOpNode(size_t hcode, DagOp* node);

   // inserts a function and returns its index
   size_t insertFun(DagFun* f);

   // creates and returns the scope of this
   Scope scope() const;

   // output on a stream
   void print(std::ostream& os) const;

   // management of context
   void bindContext(DagContext* context);
   DagContext* unbindContext();
   DagContext* cloneDefaultContext() const;

   Interval dom(size_t i) const;
   void setDom(size_t i, const Interval& x);
   void reduceDom(size_t i, const Interval& x);

   // interval evaluation
   void eval(const IntervalVector& X);

   // point evaluation
   void reval(const RealVector& P);

private:
   // vector of nodes sorted by a topological ordering from the leaves
   // to the roots
   std::vector<DagNode*> node_;

   // vector of constant nodes
   std::vector<DagConst*> cnode_;

   // vector of nodes of variables sorted by an ascending ordering
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

// output on a stream
std::ostream& operator<<(std::ostream& os, const Dag& dag);

inline size_t Dag::nbNode() const
{
   return node_.size();
}

inline DagNode* Dag::node(size_t i) const
{
   return node_[i];
}

inline size_t Dag::nbFun() const
{
   return fun_.size();
}

inline void Dag::pushNode(DagNode* node)
{
   node_.push_back(node);
   defaultContext_->extend(1);
}

inline Interval Dag::dom(size_t i) const
{
   return context_->dom[i];
}

inline void Dag::setDom(size_t i, const Interval& x)
{
   context_->dom.set(i, x);
}

inline void Dag::reduceDom(size_t i, const Interval& x)
{
   Interval aux( x & context_->dom[i]);
   context_->dom.set(i, aux);
}

inline DagContext* Dag::cloneDefaultContext() const
{
   return defaultContext_->clone();
}

inline size_t Dag::nbVar() const
{
   return vnode_.size();
}

inline DagVar* Dag::varNode(size_t i) const
{
   return vnode_[i];
}

/*****************************************************************************
 * Class of visitors of DAG nodes.
 *****************************************************************************/
class DagVisitor {
public:
   // destructor
   virtual ~DagVisitor();

   // visit methods
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
};

/*****************************************************************************
 * Class of visitors used to create a function in a DAG.
 *****************************************************************************/
class DagFunCreator : public DagVisitor {
public:
   // constructor
   DagFunCreator(DagFun* f);

   // Override
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

private:
   DagFun* f_;

   void visitSubNodes(const DagOp* d);
};

/*****************************************************************************
 * Class of visitors used to insert a constraint in a DAG.
 *****************************************************************************/
class DagCreator : public ConstraintVisitor {
public:
   // constructor
   DagCreator(Dag* dag, const Constraint& c);

   // returns the constraint index in the DAG
   size_t index();

   // Override
   void apply(const ConstraintEq* c);
   void apply(const ConstraintLe* c);
   void apply(const ConstraintLt* c);
   void apply(const ConstraintGe* c);
   void apply(const ConstraintGt* c);
   void apply(const ConstraintIn* c);

private:
   Dag* dag_;
   Constraint c_;
   size_t index_;
};

inline size_t DagCreator::index()
{
   return index_;
}

/*****************************************************************************
 * Class of visitors used to insert a term in a DAG.
 *****************************************************************************/
class DagTermCreator : public TermVisitor {
public:
   // constructor
   DagTermCreator(Dag* dag);

   // returns the term index in the DAG
   size_t index();

   // Override
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

private:
   Dag* dag_;
   IndexList lsub_;
   size_t index_;

   void visitSubnodes(const TermOp* t);
};

inline size_t DagTermCreator::index()
{
   return index_;
}

} // namespace

#endif
