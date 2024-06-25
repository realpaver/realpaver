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
 * @file   FlatFunction.hpp
 * @brief  Function used to implement a fast version of hc4Revise
 * @author Laurent Granvilliers
 * @date   2024-6-11
*/

#ifndef REALPAVER_FLAT_FUNCTION_HPP
#define REALPAVER_FLAT_FUNCTION_HPP

#include <vector>
#include "realpaver/Dag.hpp"
#include "realpaver/Term.hpp"

namespace realpaver {

/// Enumeration of flat function node symbols
enum class FlatSymbol {
   Cst,     ///< constant
   Var,     ///< variable
   Add,     ///< addition
   AddL,    ///< addition with a constant in the left node
   AddR,    ///< addition with a constant in the right node
   Sub,     ///< subtraction
   SubL,    ///< subtraction with a constant in the left node
   SubR,    ///< subtraction with a constant in the right node
   Mul,     ///< multiplication
   MulL,    ///< multiplication with a constant in the left node
   MulR,    ///< multiplication with a constant in the right node
   Div,     ///< division
   DivL,    ///< division with a constant in the left node
   DivR,    ///< division with a constant in the right node
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
   Tanh     ///< hyperbolic tangent
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, FlatSymbol op);

/*----------------------------------------------------------------------------*/

/**
 * @brief Function used to implement a fast version of HC4Revise.
 * 
 * The nodes of the tree-representation of a function are stored in arrays
 * sorted by a topological ordering from the leaves to the root.
 * 
 * Example : (x - sqr(y)) + 1 = 0
 * Topological ordering: x y sqr - 1 +
 * 
 * Suppose that the identifiers of x and y are respectively 28 and 43.
 * 
 * Attributes:
 * - Image img_ : [0, 0]
 * - Scope scop_ : {x, y}
 * - Number of nodes nb_ : 6
 * - Array symb_: Var Var Sqr Sub Cst AddR
 * - Constants cst_ : [1, 1] (one constant node)
 * - Variables var_ : x, y (two variable nodes)
 * - Values itv_ : array of nb_ intervals, one per node
 * - arg_[0] : 3 28 0 (node x, 3 is the size of arg_[0], 28 is the id of x
 *             and 0 is the index of the variable in var_)
 * - arg_[1] : 3 43 1 (node y)
 * - arg_[2] : 2 1 (node sqr, 2 is the size of arg_[2], 0 is the index of
 *             its argument, i.e. the node y)
 * - arg_[3] : 3 0 2 (node - between nodes at positions 0 and 2)
 * - arg_[4] : 2 0 (constant node, 0 is the index of the constant in cst_)
 * - arg_[5] : 3 3 4 (node + between nodes at positions 3 and 4)
 * 
 * The symbol AddR means that we have an addition between a left-hand non
 * constant expression and a right-hand constant expression (value 1). The
 * advantage is to save projection operations on constant nodes.
 */
class FlatFunction {
public:
   /// Creates a flat function representing t with image img
   FlatFunction(const Term& t, const Interval& img);

   /// Creates a flat function representing a function f from a DAG
   FlatFunction(const DagFun* f);

   /// Destructor
   ~FlatFunction();

   /// No copy
   FlatFunction(const FlatFunction&) = delete;

   /// No assignment
   FlatFunction& operator=(const FlatFunction&) = delete;

   /// Evaluates this on B
   Interval eval(const IntervalBox& B);

   /// Contracts B with respect to this using HC4Revise
   Proof contract(IntervalBox& B);

   /// Contracts B with respect to the negation of this using hc4Revise
   Proof contractNeg(IntervalBox& B);

   /// @name Creation methods
   ///@{
   
   /// Inserts a constant node and returns its index
   size_t insertCst(const Interval& val);

   /// Inserts a variable node and returns its index
   size_t insertVar(const Variable& v);

   /// Inserts a unary node and returns its index
   size_t insertUnary(FlatSymbol symb, size_t ic);

   /// Inserts a binary node and returns its index
   size_t insertBinary(FlatSymbol symb, size_t il, size_t ir);

   /// Inserts a power node and returns its index
   size_t insertPow(FlatSymbol symb, size_t ic, int e);

   /// Inserts a linear term x*v
   size_t insertLin(const Interval& x, Variable v);
   ///@}

   /// Output on a stream
   void print(std::ostream& os) const;

private:
   Scope scop_;         // set of variables occurring in this
   Interval img_;       // image

   FlatSymbol* symb_;   // node symbols sorted from the leaves to the root
   size_t nb_, capa_;   // number of symbols and capacity of symb_

   size_t** arg_;       // arguments representing the indexes of the child nodes

   Interval* itv_;      // used for evaluation and projection

   std::vector<Interval> cst_;   // list of constants
   std::vector<Variable> var_;   // list of variables (with multi-occurrences)

   // evaluation (forward phase)
   Interval eval(const IntervalVector& V);

   // backward phase
   Proof backward(IntervalBox& B);
   Proof backward(IntervalVector& V);

   // creation functions
   void make(const DagFun& f);

   // extends the capacity
   void extendCapacity();

   // deallocates the dynamic memory
   void destroy();
};

/// Output on a stream
std::ostream& operator<<(std::ostream& os, const FlatFunction& f);

/*----------------------------------------------------------------------------*/

/// Visitor of terms that creates a flat function
class FlatFunTermCreator : public TermVisitor {
public:
   /// Constructor given the target function f
   FlatFunTermCreator(FlatFunction* f);

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
   FlatFunction* f_;    // target function
   size_t idx_;         // index of node in f_ resulting from a visit
};

/*----------------------------------------------------------------------------*/

/// Visitor of dag functions that creates a flat function
class FlatFunDagCreator : public DagVisitor {
public:
   /// Constructor given the target function f
   FlatFunDagCreator(FlatFunction* f);

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
   FlatFunction* f_;    // target function
   size_t idx_;         // index of node in f_ resulting from a visit   
};

} // namespace

#endif
