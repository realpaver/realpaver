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
#include "realpaver/Term.hpp"

namespace realpaver {

/**
 * @brief Function used to implement a fast version of hc4Revise.
 * 
 * The nodes of the tree-representation of a function are stored in arrays
 * sorted by a topological ordering from the leaves to the root.
 */
class FlatFunction {
public:
   /// Creates a flat function representing t with image img
   FlatFunction(const Term& t, const Interval& img);

   /// Destructor
   ~FlatFunction();

   /// No copy
   FlatFunction(const FlatFunction&) = delete;

   /// No assignment
   FlatFunction& operator=(const FlatFunction&) = delete;

   /// Evaluates this on B
   Interval eval(const IntervalBox& B) const;

   /// Contracts B with respect to this using an hc4Revise algorithm
   Proof contract(IntervalBox& B) const;

   /// @name Creation methods
   ///@{
   
   /// Inserts a constant node and returns its index
   size_t insertCst(const Interval& val);

   /// Inserts a variable node and returns its index
   size_t insertVar(const Variable& v);

   /// Inserts a unary node and returns its index
   size_t insertUnary(NodeSymbol symb, size_t ic);

   /// Inserts a binary node and returns its index
   size_t insertBinary(NodeSymbol symb, size_t il, size_t ir);

   /// Inserts a power node and returns its index
   size_t insertPow(NodeSymbol symb, size_t ic, int e);
   ///@}

private:
   Scope scop_;         // set of variables occurring in this
   Interval img_;       // image

   NodeSymbol* symb_;   // node symbols sorted from the leaves to the root
   size_t nb_, capa_;   // number of symbols and capacity of symb_

   size_t** arg_;       // arguments representing the indexes of the child nodes

   Interval* itv_;      // used for evaluation and projection

   std::vector<Interval> cst_;   // list of constants
   std::vector<Variable> var_;   // list of variables (with multi-occurrences)

   Interval eval(const IntervalVector& V) const;
   Proof backward(IntervalBox& B) const;
   Proof backward(IntervalVector& V) const;
};

/*----------------------------------------------------------------------------*/

/// Visitor of terms that creates a flat function
class FlatFunctionCreator : public TermVisitor {
public:
   /// Constructor given the target function f
   FlatFunctionCreator(FlatFunction* f);

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

} // namespace

#endif
